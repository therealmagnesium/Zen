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

        for (auto& [name, texture] : m_textures)
            Graphics::UnloadTexture(texture);

        m_meshes.clear();
        m_textures.clear();
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

    Graphics::Texture* AssetResourceManager::GetTexture(const char* name)
    {
        Graphics::Texture* texture = NULL;

        if (m_textures.find(name) != m_textures.end())
        {
            texture = &m_textures[name];
            return texture;
        }
        else
        {
            WARN("Failed to get texture %s from the asset manager", name);
            return NULL;
        }
    }

    std::vector<std::string> AssetResourceManager::GetAllTextureNames()
    {
        std::vector<std::string> textureNames;
        textureNames.reserve(m_textures.size());

        for (auto& [name, texture] : m_textures)
            textureNames.push_back(name);

        return textureNames;
    }

    void AssetResourceManager::AddTexture(const char* name, const char* path)
    {
        if (m_textures.find(name) != m_textures.end())
        {
            WARN("Cannot add texture %s to asset manager because it's already loaded", name);
            return;
        }

        Graphics::Texture texture = Graphics::LoadTexture(path, Graphics::TextureFormat::RGBA, true);
        std::string keyName = name;

        m_textures[keyName] = texture;

        INFO("Successfully loaded texture %s to asset manager", name);
    }

    std::unordered_map<std::string, Graphics::Texture> AssetResourceManager::GetAllTextures()
    {
        return m_textures;
    }
}
