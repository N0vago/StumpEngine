#include "GUI/LogWindow.h"

namespace GUI {
	void LogWindow::OnDraw()
	{
        Debug::Logger* logger = Debug::Logger::Get();
        const auto& entries = logger->GetEntries();

        // ===== Toolbar =====
        if (ImGui::Button("Clear"))
            logger->Clear();

        ImGui::SameLine();
        ImGui::Checkbox("Auto-scroll", &autoScroll);

        ImGui::Separator();

        ImGui::Checkbox("Info", &showInfo); ImGui::SameLine();
        ImGui::Checkbox("Warning", &showWarning); ImGui::SameLine();
        ImGui::Checkbox("Error", &showError); ImGui::SameLine();
        ImGui::Checkbox("Debug", &showDebug);

        ImGui::Separator();

        // ===== Log list =====
        ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), true);

        for (const auto& entry : entries)
        {
            if (
                (entry.level == Debug::LogLevel::Info && !showInfo) ||
                (entry.level == Debug::LogLevel::Warning && !showWarning) ||
                (entry.level == Debug::LogLevel::Error && !showError) ||
                (entry.level == Debug::LogLevel::Debug && !showDebug)
                )
                continue;

            ImGui::PushStyleColor(ImGuiCol_Text, GetColor(entry.level));

            ImGui::Text("[%s] %s",
                GetLevelName(entry.level),
                entry.message.c_str());

            ImGui::PopStyleColor();
        }

        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
	}
	ImVec4 LogWindow::GetColor(Debug::LogLevel level) const
	{
        switch (level)
        {
        case Debug::LogLevel::Info:    return ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
        case Debug::LogLevel::Warning: return ImVec4(1.0f, 0.8f, 0.3f, 1.0f);
        case Debug::LogLevel::Error:   return ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
        case Debug::LogLevel::Debug:   return ImVec4(0.4f, 0.7f, 1.0f, 1.0f);
        default:                      return ImVec4(1, 1, 1, 1);
        }
	}
    const char* LogWindow::GetLevelName(Debug::LogLevel level) const
    {
        switch (level)
        {
        case Debug::LogLevel::Info:    return "INFO";
        case Debug::LogLevel::Warning: return "WARN";
        case Debug::LogLevel::Error:   return "ERROR";
        case Debug::LogLevel::Debug:   return "DEBUG";
        default:                       return "UNKNOWN";
        }
    }
}