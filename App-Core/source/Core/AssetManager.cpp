#include "Core/AssetManager.h"
#include "Core/Log.h"

#include <stdlib.h>

namespace Core
{
    static bool isInitialized = false;
    AssetResourceManager* AssetManager = NULL;

    void AssetResourceManager::Initialize()
    {
        if (isInitialized)
        {
            WARN("Can't initialize asset manager more than once");
            return;
        }

        AssetManager = this;
        isInitialized = true;
    }

    void AssetResourceManager::Shutdown()
    {
        for (auto& [name, mesh] : m_meshes)
            Graphics::UnloadMesh(mesh);

        m_meshes.clear();
    }

    Graphics::Mesh* AssetResourceManager::GetMesh(const char* name)
    {
        Graphics::Mesh* mesh = NULL;

        if (m_meshes.find(name) == m_meshes.end())
        {
            WARN("Cannot get mesh %s from asset manager because it hasn't been loaded");
            return mesh;
        }

        mesh = &m_meshes[name];
        return mesh;
    }

    std::vector<std::string> AssetResourceManager::GetAllMeshNames()
    {
        std::vector<std::string> meshNames;
        meshNames.reserve(m_meshes.size());

        for (auto& [name, mesh] : m_meshes)
            meshNames.push_back(name);

        return meshNames;
    }

    void AssetResourceManager::AddMesh(const char* name, const char* path)
    {
        if (m_meshes.find(name) != m_meshes.end())
        {
            WARN("Skipping adding mesh %s to asset manager because it's already loaded");
            return;
        }

        Graphics::Mesh mesh = Graphics::LoadMesh(path);
        mesh.name = name;
        m_meshes[name] = mesh;
    }
}
