#pragma once

class MainController;


class OptionsPanel {
public:
    explicit OptionsPanel(MainController& controller);

    void Render();

    static const char* WindowName()
        { return "Options"; }

private:
    MainController& controller;

    void RenderStartStopButton(bool simRuns);
    void RenderSteeringCubeOptions() const;
    void RenderVisualizationOptions() const;
};
