#pragma once

class MainController;


class OptionsPanel {
public:
    explicit OptionsPanel(MainController& controller);

    void Render() const;

    static const char* WindowName()
        { return "Options"; }

private:
    MainController& controller;

    void RenderStartStopButton(bool simRuns) const;
    void RenderSteeringCubeOptions() const;
    void RenderVisualizationOptions() const;
    void RenderDisturbOptions() const;
    void RenderSimulationOptions(bool simulationRunning) const;

    static void RenderSimulationInfo();
};
