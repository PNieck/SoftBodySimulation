#include <simulation/controllers/mainController.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <random>
#include <numbers>


MainController::MainController(GLFWwindow *window):
    optionsPanel(*this),
    visualization(1280, 920),
    bezierPointsIds(4, 4, 4),
    steeringPointsIds(2, 2, 2),
    model(InitialSpringGraph(), SimulationEnvironment())
{
    const auto glsl_version = "#version 410";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    visualization.GetSteeringCube().SetEdgeLength(steeringCubeEdgeLen);
    visualization.SetSimulationProperties(SimulationEnvironment());
}


MainController::~MainController()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void MainController::Render()
{
    // Creating new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    dockingSpace.Render();
    optionsPanel.Render();

    visualization.Render(model.StartReadingGraph(), bezierPointsIds);
    model.EndReadingGraph();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void MainController::MouseMoved(const int x, const int y)
{
    mouseState.Moved(x, y);

    if (WantToCaptureMouse())
        return;

    if (mouseState.IsButtonClicked(MouseButton::Middle)) {
        const auto offset = mouseState.TranslationGet();
        visualization.RotateCamera(
            offset.x * 0.02f, offset.y * 0.02f
        );
    }
}


void MainController::ScrollMoved(const int offset)
{
    if (WantToCaptureMouse())
        return;

    float val = offset * 0.7f;

    if (val < 0.0f) {
        val = -1.0f / val;
    }

    const auto camPos = visualization.GetCameraPosition();
    visualization.SetCameraPosition(camPos * val);

}

void MainController::SetSteeringCubePosition(const glm::vec3 &position) {
    visualization.GetSteeringCube().SetPosition(position);
    UpdateSteeringMaterialPoints();
}


void MainController::SetSteeringCubeRotation(const glm::quat &rotation) {
    visualization.GetSteeringCube().SetRotation(rotation);
    UpdateSteeringMaterialPoints();
}


void MainController::DisturbSoftBody(const float maxDisturb)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution zeroTo2Pi(0.f, 2.f*std::numbers::pi_v<float>);
    std::uniform_real_distribution minusOneToOne(-1.f, 1.f);
    std::uniform_real_distribution zeroToMaxDisturb(0.f, maxDisturb);

    auto& graph = model.StartWritingGraph();

    for (int x=0; x < bezierPointsIds.SizeX(); x++) {
        for (int y=0; y < bezierPointsIds.SizeY(); y++) {
            for (int z=0; z < bezierPointsIds.SizeZ(); z++) {
                const auto id = bezierPointsIds.At(x, y, z);

                const float alpha = zeroTo2Pi(gen);
                const float v = minusOneToOne(gen);
                const float len = zeroToMaxDisturb(gen);

                graph.GetMaterialPoint(id).velocity += glm::vec3(
                    std::sqrt(1-v*v) * std::cos(alpha),
                    std::sqrt(1-v*v) * std::sin(alpha),
                    v
                ) * len;
            }
        }
    }

    model.EndWritingGraph();
}


void MainController::SetBezierSpringsCoefficient(const float coefficient)
{
    auto& graph = model.StartWritingGraph();
    bezierSpringCoef = coefficient;

    for (const auto springId : bezierSprings) {
        graph.GetSpring(springId).springCoef = coefficient;
    }

    model.EndWritingGraph();
}


void MainController::SetSteeringSpringsCoefficient(const float coefficient)
{
    auto& graph = model.StartWritingGraph();
    steeringSpringCoef = coefficient;

    for (const auto springId : steeringSprings) {
        graph.GetSpring(springId).springCoef = coefficient;
    }

    model.EndWritingGraph();
}


void MainController::SetSimulationEnvironment(const SimulationEnvironment &environment)
{
    visualization.SetSimulationProperties(environment);
    model.SetEnvironment(environment);
}


SpringGraph MainController::InitialSpringGraph() {
    constexpr int bezierSpringsCnt = 396;
    constexpr int steeringSpringsCnt = 8;

    SpringGraph result(64 + 8, bezierSpringsCnt * steeringSpringsCnt);

    steeringSprings.reserve(steeringSpringsCnt);
    bezierSprings.reserve(bezierSpringsCnt);

    constexpr glm::vec3 startPoint(-steeringCubeEdgeLen/2.f);
    constexpr glm::vec3 vecX(1.f, 0.f, 0.f);
    constexpr glm::vec3 vecY(0.f, 1.f, 0.f);
    constexpr glm::vec3 vecZ(0.f, 0.f, 1.f);

    const auto maxX = static_cast<float>(bezierPointsIds.SizeX() - 1);
    const auto maxY = static_cast<float>(bezierPointsIds.SizeY() - 1);
    const auto maxZ = static_cast<float>(bezierPointsIds.SizeZ() - 1);

    // Adding bezier cube points
    for (size_t x=0; x < bezierPointsIds.SizeX(); x++) {
        for (size_t y=0; y < bezierPointsIds.SizeY(); y++) {
            for (size_t z=0; z < bezierPointsIds.SizeZ(); z++) {
                glm::vec3 pos = startPoint +
                    static_cast<float>(x) / maxX * steeringCubeEdgeLen * vecX +
                    static_cast<float>(y) / maxY * steeringCubeEdgeLen * vecY +
                    static_cast<float>(z) / maxZ * steeringCubeEdgeLen * vecZ;

                MaterialPoint mp(initialMaterialPointMass, pos);

                bezierPointsIds.At(x, y, z) = result.AddMaterialPoint(mp);
            }
        }
    }

    // Adding steering points
    for (size_t x=0; x < steeringPointsIds.SizeX(); x++) {
        for (size_t y=0; y < steeringPointsIds.SizeY(); y++) {
            for (size_t z=0; z < steeringPointsIds.SizeZ(); z++) {
                glm::vec3 pos = startPoint +
                    static_cast<float>(x) * steeringCubeEdgeLen * vecX +
                    static_cast<float>(y) * steeringCubeEdgeLen * vecY +
                    static_cast<float>(z) * steeringCubeEdgeLen * vecZ;

                MaterialPoint mp(std::numeric_limits<float>::infinity(), pos);

                steeringPointsIds.At(x, y, z) = result.AddMaterialPoint(mp);
            }
        }
    }

    /*
     *      Adding springs
     */

    float stdSpringLen = steeringCubeEdgeLen / 3.f;
    float diagonalSpringLen = steeringCubeEdgeLen / 3.f * std::sqrt(2.f);

    for (size_t x=0; x < bezierPointsIds.SizeX(); x++) {
        for (size_t y=0; y < bezierPointsIds.SizeY(); y++) {
            for (size_t z=0; z < bezierPointsIds.SizeZ(); z++) {
                if (x != bezierPointsIds.SizeZ() - 1) {
                    SpringId end1 = bezierPointsIds.At(x, y, z);
                    SpringId end2 = bezierPointsIds.At(x + 1, y, z);

                    bezierSprings.push_back(
                        result.AddSpring(stdSpringLen, initialBezierSpringsCoef, end1, end2)
                    );
                }

                if (y != bezierPointsIds.SizeY() - 1) {
                    SpringId end1 = bezierPointsIds.At(x, y, z);
                    SpringId end2 = bezierPointsIds.At(x, y + 1, z);

                    bezierSprings.push_back(
                        result.AddSpring(stdSpringLen, initialBezierSpringsCoef, end1, end2)
                    );
                }

                if (z != bezierPointsIds.SizeZ() - 1) {
                    SpringId end1 = bezierPointsIds.At(x, y, z);
                    SpringId end2 = bezierPointsIds.At(x, y, z + 1);

                    bezierSprings.push_back(
                        result.AddSpring(stdSpringLen, initialBezierSpringsCoef, end1, end2)
                    );
                }

                if (x != bezierPointsIds.SizeX() - 1 && y != bezierPointsIds.SizeY() - 1) {
                    SpringId end1 = bezierPointsIds.At(x, y, z);
                    SpringId end2 = bezierPointsIds.At(x + 1, y + 1, z);

                    bezierSprings.push_back(
                        result.AddSpring(diagonalSpringLen, initialBezierSpringsCoef, end1, end2)
                    );
                }

                if (x != bezierPointsIds.SizeX() - 1 && z != bezierPointsIds.SizeZ() - 1) {
                    SpringId end1 = bezierPointsIds.At(x, y, z);
                    SpringId end2 = bezierPointsIds.At(x + 1, y, z + 1);

                    bezierSprings.push_back(
                        result.AddSpring(diagonalSpringLen, initialBezierSpringsCoef, end1, end2)
                    );
                }

                if (y != bezierPointsIds.SizeY() - 1 && z != bezierPointsIds.SizeZ() - 1) {
                    SpringId end1 = bezierPointsIds.At(x, y, z);
                    SpringId end2 = bezierPointsIds.At(x, y + 1, z + 1);

                    bezierSprings.push_back(
                        result.AddSpring(diagonalSpringLen, initialBezierSpringsCoef, end1, end2)
                    );
                }

                if (x != bezierPointsIds.SizeX() - 1 && y != 0) {
                    SpringId end1 = bezierPointsIds.At(x, y, z);
                    SpringId end2 = bezierPointsIds.At(x + 1, y - 1, z);

                    bezierSprings.push_back(
                        result.AddSpring(diagonalSpringLen, initialBezierSpringsCoef, end1, end2)
                    );
                }

                if (x != bezierPointsIds.SizeX() - 1 && z != 0) {
                    SpringId end1 = bezierPointsIds.At(x, y, z);
                    SpringId end2 = bezierPointsIds.At(x + 1, y, z - 1);

                    bezierSprings.push_back(
                        result.AddSpring(diagonalSpringLen, initialBezierSpringsCoef, end1, end2)
                    );
                }

                if (y != bezierPointsIds.SizeY() - 1 && z != 0) {
                    SpringId end1 = bezierPointsIds.At(x, y, z);
                    SpringId end2 = bezierPointsIds.At(x, y + 1, z - 1);

                    bezierSprings.push_back(
                        result.AddSpring(diagonalSpringLen, initialBezierSpringsCoef, end1, end2)
                    );
                }
            }
        }
    }

    for (size_t x=0; x < steeringPointsIds.SizeX(); x++) {
        for (size_t y=0; y < steeringPointsIds.SizeY(); y++) {
            for (size_t z=0; z < steeringPointsIds.SizeZ(); z++) {
                SpringId end1 = steeringPointsIds.At(x, y, z);
                SpringId end2 = bezierPointsIds.At(3*x, 3*y, 3*z);

                steeringSprings.push_back(
                    result.AddSpring(0.f, initialSteeringSpringsCoef, end1, end2)
                );
            }
        }
    }

    return result;
}


void MainController::UpdateSteeringMaterialPoints() {
    const auto& steeringCube = visualization.GetSteeringCube();
    const auto& cubePosition = steeringCube.GetPosition();

    const float len = steeringCube.GetEdgeLength() / 2.0f * std::sqrt(3.f);
    const auto& rotation = steeringCube.RotationQuat();

    auto& graph = model.StartWritingGraph();

    for (int x=0; x < 2; x++) {
        for (int y=0; y < 2; y++) {
            for (int z=0; z < 2; z++) {
                const auto id = steeringPointsIds.At(x, y, z);

                auto newPosition = normalize(
                    glm::vec3(
                        -1.0f + static_cast<float>(2 * x),  // Equals -1 if x == 0 and 1 if x == 1
                        -1.0f + static_cast<float>(2 * y),
                        -1.0f + static_cast<float>(2 * z)
                    )) * len;

                newPosition = rotate(rotation, newPosition);
                newPosition += cubePosition;
                graph.GetMaterialPoint(id).position = newPosition;
            }
        }
    }

    model.EndWritingGraph();
}
