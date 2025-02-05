#pragma once
#include "Graphics/Mesh.h"

#include <string>
#include <unordered_map>

namespace Core
{
    class AssetResourceManager
    {
    public:
        AssetResourceManager() = default;
        ~AssetResourceManager() = default;

        void Initialize();
        void Shutdown();

        Graphics::Mesh* GetMesh(const char* name);
        std::vector<std::string> GetAllMeshNames();
        void AddMesh(const char* name, const char* path);

        Graphics::Material* GetMaterial(const char* name);
        void AddMaterial(const char* name, const char* path);

    private:
        std::unordered_map<std::string, Graphics::Mesh> m_meshes;
        std::unordered_map<std::string, Graphics::Material> m_materials;
    };

    extern AssetResourceManager* AssetManager;
}
