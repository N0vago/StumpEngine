#ifndef ST_FILE_SYSTEM_WINDOW_H
#define ST_FILE_SYSTEM_WINDOW_H

#include "GUI/GuiWindow.h"
#include <filesystem>

namespace GUI {

    class FileSystemWindow : public GuiWindow
    {
    public:
        FileSystemWindow(
            const std::string& r_name,
            std::filesystem::path p_rootPath
        );

        const std::filesystem::path& GetSelectedPath() const
        {
            return selectedPath;
        }

    protected:
        void OnDraw() override;

    private:
        std::filesystem::path rootPath;
        std::filesystem::path currentPath;
        std::filesystem::path selectedPath;

        void DrawToolbar();
        void DrawDirectoryContent(const std::filesystem::path& path);
    };

}

#endif // ST_FILE_SYSTEM_WINDOW_H
