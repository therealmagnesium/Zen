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

        INFO("Successfully initialized the asset manager");
    }

    void AssetResourceManager::Shutdown()
    {
        INFO("Shutting down the asset manager...");

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
        std::vector<Graphics::Mesh> meshes = Graphics::LoadMeshes(path);

        for (u32 i = 0; i < meshes.size(); i++)
        {
            Graphics::Mesh& mesh = meshes[i];

            if (m_meshes.find(mesh.name) != m_meshes.end())
                mesh.name = mesh.name + std::to_string(i);

            mesh.path = path;
            m_meshes[mesh.name] = mesh;
        }

        INFO("Successfully loaded model %s to asset manager", name);
    }
}
