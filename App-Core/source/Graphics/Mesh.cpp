#include "Graphics/Mesh.h"
#include "Graphics/RenderCommand.h"

#include "Core/AssetManager.h"
#include "Core/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>
#include <string>
#include <vector>

namespace Graphics
{
    u32 Mesh::LoadFlags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices;

    static void UploadMeshData(Mesh& mesh)
    {
        mesh.vertexArray = CreateVertexArray();
        mesh.vertexBuffer = CreateVertexBuffer();
        mesh.instanceBuffer = CreateVertexBuffer();
        mesh.indexBuffer = CreateIndexBuffer();

        BindVertexBuffer(mesh.vertexBuffer);
        BindIndexBuffer(mesh.indexBuffer);

        RenderCommand::SendDataToBuffer(mesh.vertexBuffer, BufferType::Array, mesh.vertices.data(),
                                        mesh.vertexCount * sizeof(Vertex));

        RenderCommand::SendDataToBuffer(mesh.indexBuffer, BufferType::ElementArray, mesh.indices.data(),
                                        mesh.indexCount * sizeof(u32));

        UnbindVertexBuffer();
        UnbindIndexBuffer();

        BindVertexArray(mesh.vertexArray);
        BindVertexBuffer(mesh.vertexBuffer);

        RenderCommand::SetAttributeLocation(mesh.vertexArray, 0, 3, sizeof(Vertex), offsetof(Vertex, position));
        RenderCommand::SetAttributeLocation(mesh.vertexArray, 1, 2, sizeof(Vertex), offsetof(Vertex, uvCoord));
        RenderCommand::SetAttributeLocation(mesh.vertexArray, 2, 3, sizeof(Vertex), offsetof(Vertex, normal));

        UnbindVertexArray();
        UnbindVertexBuffer();
    }

    static void DeleteMeshData(Mesh& mesh)
    {
        DeleteVertexArray(mesh.vertexArray);
        DeleteBuffer(mesh.vertexBuffer);
        DeleteBuffer(mesh.instanceBuffer);
        DeleteBuffer(mesh.indexBuffer);
    }

    static Mesh ProcessMesh(aiMesh* aMesh, const aiScene* model)
    {
        Mesh mesh;
        mesh.name = aMesh->mName.C_Str();

        mesh.vertices.reserve(aMesh->mNumVertices);
        for (u32 i = 0; i < aMesh->mNumVertices; i++)
        {
            Vertex vertex;

            vertex.position.x = aMesh->mVertices[i].x;
            vertex.position.y = aMesh->mVertices[i].y;
            vertex.position.z = aMesh->mVertices[i].z;

            if (aMesh->HasTextureCoords(0))
            {
                vertex.uvCoord.x = aMesh->mTextureCoords[0][i].x;
                vertex.uvCoord.y = aMesh->mTextureCoords[0][i].y;
            }
            else
                vertex.uvCoord = glm::vec2(0.f);

            if (aMesh->HasNormals())
            {
                vertex.normal.x = aMesh->mNormals[i].x;
                vertex.normal.y = aMesh->mNormals[i].y;
                vertex.normal.z = aMesh->mNormals[i].z;
            }

            mesh.vertices.push_back(vertex);
        }
        mesh.vertexCount = mesh.vertices.size();

        mesh.indices.reserve(aMesh->mNumFaces * 3);
        for (u32 i = 0; i < aMesh->mNumFaces; i++)
        {
            aiFace face = aMesh->mFaces[i];
            for (u32 j = 0; j < face.mNumIndices; j++)
                mesh.indices.push_back(face.mIndices[j]);
        }
        mesh.indexCount = mesh.indices.size();

        aiMaterial* aMaterial = model->mMaterials[aMesh->mMaterialIndex];

        bool skipTextureLoad = false;
        aiString aPath;
        aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aPath);

        // The texture has already been loaded, so don't load it again.
        for (auto& [name, texture] : Core::AssetManager->GetAllTextures())
        {
            if (strcmp(texture.path.c_str(), aPath.C_Str()) == 0)
            {
                skipTextureLoad = true;
                mesh.material.diffuseMap = texture;
                break;
            }
        }

        // The texture hasn't been loaded yet, so load it.
        if (!skipTextureLoad && aPath.length > 3)
        {
            std::string meshName = aMesh->mName.C_Str();
            std::string pathString = aPath.C_Str();
            std::filesystem::path path(pathString);
            std::string name = meshName + "_" + path.filename().stem().c_str();

            Core::AssetManager->AddTexture(name.c_str(), aPath.C_Str());
            Texture* texture = Core::AssetManager->GetTexture(name.c_str());
            mesh.material.diffuseMap = *texture;
        }

        aiColor3D diffuseColor(0.f, 0.f, 0.f);
        if (aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
            mesh.material.diffuse = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

        UploadMeshData(mesh);

        return mesh;
    }

    static void ProcessNode(std::vector<Mesh>& meshes, aiNode* node, const aiScene* model)
    {
        for (u32 i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* aMesh = model->mMeshes[node->mMeshes[i]];
            Mesh mesh = ProcessMesh(aMesh, model);
            meshes.push_back(mesh);
        }

        for (u32 i = 0; i < node->mNumChildren; i++)
            ProcessNode(meshes, node->mChildren[i], model);
    }

    std::vector<Mesh> LoadMeshes(const char* path)
    {
        std::vector<Mesh> meshes;

        Assimp::Importer importer;
        const aiScene* model = importer.ReadFile(path, Mesh::LoadFlags);

        if (model == NULL || model->mRootNode == NULL)
        {
            WARN("Failed to load model %s...", path);
            return meshes;
        }

        meshes.reserve(model->mNumMeshes);
        ProcessNode(meshes, model->mRootNode, model);

        return meshes;
    }

    void UnloadMesh(Mesh& mesh)
    {
        mesh.vertexCount = 0;
        mesh.indexCount = 0;
        mesh.vertices.clear();
        mesh.indices.clear();

        DeleteMeshData(mesh);
    }
}
