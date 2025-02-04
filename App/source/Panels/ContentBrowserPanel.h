#pragma once
#include <Zen.h>
#include <filesystem>

using namespace Core;
using namespace Graphics;

class ContentBrowserPanel
{
public:
    ContentBrowserPanel() = default;

    void Initialize();
    void Shutdown();
    void Display();

private:
    std::filesystem::path m_currentDirectory;
    Texture m_folderIcon;
    Texture m_fileIcon;
};
