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
        void AddMesh(const char* name, const char* path);

    private:
        std::unordered_map<std::string, Graphics::Mesh> m_meshes;
    };

    extern AssetResourceManager* AssetManager;
}
