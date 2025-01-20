#include "Graphics/Mesh.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderCommand.h"

#include "Core/Log.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

namespace Graphics
{
    static const u32 MeshLoadFlags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
    static std::vector<Texture> loadedTextures;

    static void PrepareMesh(Mesh& mesh)
    {
        mesh.vertexArray = CreateVertexArray();
        mesh.vertexBuffer = CreateVertexBuffer();
        mesh.indexBuffer = CreateIndexBuffer();

        RenderCommand::SendDataToBuffer(mesh.vertexBuffer, BufferType::Array, mesh.vertices.data(),
                                        mesh.vertexCount * sizeof(Vertex));

        RenderCommand::SendDataToBuffer(mesh.indexBuffer, BufferType::ElementArray, mesh.indices.data(),
                                        mesh.indexCount * sizeof(u32));

        BindVertexArray(mesh.vertexArray);

        RenderCommand::SetAttributeLocation(mesh.vertexArray, 0, 3, sizeof(Vertex), offsetof(Vertex, position));
        RenderCommand::SetAttributeLocation(mesh.vertexArray, 1, 2, sizeof(Vertex), offsetof(Vertex, uvCoord));
        RenderCommand::SetAttributeLocation(mesh.vertexArray, 2, 3, sizeof(Vertex), offsetof(Vertex, normal));

        UnbindVertexArray();
    }

    static void FinishMesh(Mesh& mesh)
    {
        DeleteVertexArray(mesh.vertexArray);
        DeleteBuffer(mesh.vertexBuffer);
        DeleteBuffer(mesh.indexBuffer);
    }

    Mesh CreateMesh(Vertex* vertices, u32 vertexCount, u32* indices, u32 indexCount)
    {
        Mesh mesh;
        mesh.vertexCount = vertexCount;
        mesh.indexCount = indexCount;
        mesh.vertices.resize(vertexCount);
        mesh.indices.resize(indexCount);

        for (u32 i = 0; i < vertexCount; i++)
            mesh.vertices[i] = vertices[i];

        for (u32 i = 0; i < indexCount; i++)
            mesh.indices[i] = indices[i];

        PrepareMesh(mesh);

        return mesh;
    }

    Mesh LoadMesh(const char* path)
    {
        Mesh mesh;
        mesh.vertexArray = CreateVertexArray();

        Assimp::Importer importer;
        const aiScene* model = importer.ReadFile(path, MeshLoadFlags);

        if (model == NULL || model->mRootNode == NULL)
        {
            WARN("Failed to load mesh %s...", path);
            return mesh;
        }

        const aiMesh* aMesh = model->mMeshes[0];

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
        for (auto& texture : loadedTextures)
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
            Texture texture = LoadTexture(aPath.C_Str(), TextureFormat::RGBA);
            mesh.material.diffuseMap = texture;
            loadedTextures.push_back(texture);
        }

        aiColor3D diffuseColor(0.f, 0.f, 0.f);
        if (aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
            mesh.material.diffuse = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

        PrepareMesh(mesh);

        return mesh;
    }

    void UnloadMesh(Mesh& mesh)
    {
        mesh.vertexCount = 0;
        mesh.indexCount = 0;
        mesh.vertices.clear();
        mesh.indices.clear();

        FinishMesh(mesh);
        UnloadTexture(mesh.material.diffuseMap);
    }
}
