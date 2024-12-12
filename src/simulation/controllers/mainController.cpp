#include <simulation/controllers/mainController.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


MainController::MainController(GLFWwindow *window):
    optionsPanel(*this),
    visualization(1280, 920),
    bezierPointsIds(4, 4, 4),
    steeringPointsIds(2, 2, 2),
    simulator(InitialSpringGraph())
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
}


MainController::~MainController()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void MainController::Update()
{
}


void MainController::Render()
{
    // Creating new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    dockingSpace.Render();
    optionsPanel.Render();
    visualization.Render();

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


bool MainController::WantToCaptureMouse() const
{
    return !visualization.IsMouseOverWindow();
}


SpringGraph MainController::InitialSpringGraph() {
    SpringGraph result(64 + 8, 396 + 8);

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

    return result;
}
