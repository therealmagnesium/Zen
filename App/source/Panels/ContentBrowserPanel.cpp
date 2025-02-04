#include "ContentBrowserPanel.h"
#include <imgui.h>
#include <string>
#include <filesystem>

static const std::filesystem::path assetsPath = "assets";
static float thumbnailPadding = 32.f;
static float thumbnailSize = 64.f;

void ContentBrowserPanel::Initialize()
{
    m_currentDirectory = assetsPath;
    m_folderIcon = LoadTexture("assets/textures/folder.png", TextureFormat::RGBA, false);
    m_fileIcon = LoadTexture("assets/textures/file.png", TextureFormat::RGBA, false);
}

void ContentBrowserPanel::Shutdown()
{
    UnloadTexture(m_folderIcon);
    UnloadTexture(m_fileIcon);
}

void ContentBrowserPanel::Display()
{
    ImGui::Begin("Content Browser");

    if (m_currentDirectory != assetsPath)
    {
        if (ImGui::Button("<-"))
            m_currentDirectory = m_currentDirectory.parent_path();
    }

    float cellSize = thumbnailSize + thumbnailPadding;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);

    if (columnCount < 1)
        columnCount = 1;

    ImGui::Columns(columnCount, NULL, false);

    for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
    {
        const std::filesystem::path& path = directoryEntry.path();
        const Texture& icon = (directoryEntry.is_directory()) ? m_folderIcon : m_fileIcon;

        std::filesystem::path relativePath = std::filesystem::relative(path, assetsPath);
        std::string filenameString = relativePath.filename().string();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
        ImGui::ImageButton(filenameString.c_str(), (void*)(u64)icon.id, ImVec2(thumbnailSize, thumbnailSize),
                           ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (directoryEntry.is_directory())
                m_currentDirectory /= path.filename();
        }

        ImGui::TextWrapped("%s", filenameString.c_str());
        ImGui::NextColumn();
    }

    ImGui::Columns(1);

    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16.f, 200.f);
    ImGui::SliderFloat("Thumbnail Padding", &thumbnailPadding, 0.f, 100.f);

    ImGui::End();
}
