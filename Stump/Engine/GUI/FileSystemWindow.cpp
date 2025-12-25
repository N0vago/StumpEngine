#include "GUI/FileSystemWindow.h"
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

namespace GUI {

    FileSystemWindow::FileSystemWindow(const std::string& r_name, fs::path p_rootPath)
        : GuiWindow(r_name),
        rootPath(fs::absolute(p_rootPath)),
        currentPath(fs::absolute(p_rootPath))
    {
    }

    void FileSystemWindow::OnDraw()
    {
        DrawToolbar();
        ImGui::Separator();
        DrawDirectoryContent(currentPath);
    }

    // Toolbar
    void FileSystemWindow::DrawToolbar()
    {
        if (currentPath != rootPath)
        {
            if (ImGui::Button(".."))
            {
                currentPath = currentPath.parent_path();
            }
        }

        ImGui::SameLine();
        ImGui::TextUnformatted(currentPath.string().c_str());
    }

    // Directory content
    void FileSystemWindow::DrawDirectoryContent(const fs::path& path)
    {
        if (!fs::exists(path))
            return;

        std::vector<fs::directory_entry> directories;
        std::vector<fs::directory_entry> files;

        for (auto& entry : fs::directory_iterator(path))
        {
            if (entry.is_directory())
                directories.push_back(entry);
            else
                files.push_back(entry);
        }

        auto sortByName = [](const fs::directory_entry& a,
            const fs::directory_entry& b)
            {
                return a.path().filename().string() <
                    b.path().filename().string();
            };

        std::sort(directories.begin(), directories.end(), sortByName);
        std::sort(files.begin(), files.end(), sortByName);

        // Directories
        for (auto& dir : directories)
        {
            const fs::path& p = dir.path();
            bool selected = (selectedPath == p);

            ImGui::Selectable(
                ("[DIR] " + p.filename().string()).c_str(),
                selected
            );

            if (ImGui::IsItemClicked())
                selectedPath = p;

            if (ImGui::IsItemHovered() &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                currentPath = p;
                selectedPath.clear();
                return;
            }
        }

        // Files
        for (auto& file : files)
        {
            const fs::path& p = file.path();
            bool selected = (selectedPath == p);

            ImGui::Selectable(
                p.filename().string().c_str(),
                selected
            );

            if (ImGui::IsItemClicked())
                selectedPath = p;

            // Drag & Drop source
            if (ImGui::BeginDragDropSource())
            {
                static std::filesystem::path draggedPath;
                draggedPath = p;

                ImGui::SetDragDropPayload(
                    "FILE_PATH",
                    &draggedPath,
                    sizeof(std::filesystem::path)
                );

                ImGui::TextUnformatted(p.filename().string().c_str());
                ImGui::EndDragDropSource();
            }
        }
    }

}
