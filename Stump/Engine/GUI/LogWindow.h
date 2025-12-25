#ifndef ST_LOG_WINDOW_H
#define ST_LOG_WINDOW_H

#include "GUI/GuiWindow.h"
#include "Debug/Logger.h"

namespace GUI {
	class LogWindow : public GuiWindow {
    public:
        LogWindow(const std::string& r_name)
            : GuiWindow(r_name)
        {
        }

    protected:
        void OnDraw() override;

    private:
        bool showInfo = true;
        bool showWarning = true;
        bool showError = true;
        bool showDebug = true;

        bool autoScroll = true;

        ImVec4 GetColor(Debug::LogLevel level) const;
        const char* GetLevelName(Debug::LogLevel level) const;
    };
}

#endif //ST_LOG_WINDOW_H