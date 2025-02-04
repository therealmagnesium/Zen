#include "Graphics/Material.h"
#include "Graphics/Mesh.h"

#include "Core/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Graphics
{
    static std::vector<Texture> loadedTextures;

    Material LoadMaterial(const char* path)
    {
        Material material;
        Assimp::Importer importer;
        const aiScene* model = importer.ReadFile(path, Mesh::LoadFlags);

        if (model == NULL || model->mRootNode == NULL)
        {
            WARN("Failed to load material from mesh %s...", path);
            return material;
        }

        const aiMesh* aMesh = model->mMeshes[0];
        aiMaterial* aMaterial = model->mMaterials[aMesh->mMaterialIndex];

        bool skipTextureLoad = false;
        aiString aPath;
        aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aPath);

        // The texture has already been loaded, so don't load it again.
        for (auto& texture : loadedTextures)
        {
            if (strcmp(texture.path.c_str(), aPath.C_Str()) == 0)
            {
                skipTextureLoad = true;
                material.diffuseMap = texture;
                break;
            }
        }

        // The texture hasn't been loaded yet, so load it.
        if (!skipTextureLoad && aPath.length > 3)
        {
            Texture texture = LoadTexture(aPath.C_Str(), TextureFormat::RGBA, true);
            material.diffuseMap = texture;
            loadedTextures.push_back(texture);
        }

        aiColor3D diffuseColor(0.f, 0.f, 0.f);
        if (aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
            material.diffuse = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

        return material;
    }

    void UnloadMaterial(Material& material)
    {
        UnloadTexture(material.diffuseMap);
    }
}
