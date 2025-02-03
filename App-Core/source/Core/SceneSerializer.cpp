#include "Core/SceneSerializer.h"
#include "Core/AssetManager.h"
#include "Core/Base.h"
#include "Core/Components.h"
#include "Core/Log.h"
#include "Core/Scene.h"

#include "Graphics/Camera.h"

#include <fstream>
#include <sstream>
#include <yaml-cpp/yaml.h>

namespace YAML
{
    template <>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& v)
        {
            Node node;
            node.push_back(v.x);
            node.push_back(v.y);
            node.push_back(v.z);

            return node;
        }

        static bool decode(const Node& node, glm::vec3& v)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            v.x = node[0].as<float>();
            v.y = node[1].as<float>();
            v.z = node[2].as<float>();

            return true;
        }
    };

    template <>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& v)
        {
            Node node;
            node.push_back(v.x);
            node.push_back(v.y);
            node.push_back(v.z);
            node.push_back(v.w);

            return node;
        }

        static bool decode(const Node& node, glm::vec4& v)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            v.x = node[0].as<float>();
            v.y = node[1].as<float>();
            v.z = node[2].as<float>();
            v.w = node[3].as<float>();

            return true;
        }
    };
}

namespace Core
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

        return out;
    }

    static void SerializeEntity(YAML::Emitter& out, std::shared_ptr<Entity>& entity)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Entity" << YAML::Value << "12837192873";
        out << YAML::Key << "Tag" << YAML::Value << entity->GetTag();

        if (entity->HasComponent<TransformComponent>())
        {
            out << YAML::Key << "Transform Component" << YAML::Value;
            out << YAML::BeginMap;

            auto& tc = entity->GetComponent<TransformComponent>();
            out << YAML::Key << "Position" << YAML::Value << tc.position;
            out << YAML::Key << "Rotation" << YAML::Value << tc.rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.scale;

            out << YAML::EndMap;
        }

        if (entity->HasComponent<CameraComponent>())
        {
            out << YAML::Key << "Camera Component" << YAML::Value;
            out << YAML::BeginMap;

            auto& cc = entity->GetComponent<CameraComponent>();
            Graphics::Camera& camera = cc.camera;

            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "FOV" << YAML::Value << camera.fov;
            out << YAML::Key << "Speed" << YAML::Value << camera.speed;
            out << YAML::Key << "Direction" << YAML::Value << camera.direction;
            out << YAML::Key << "Up" << YAML::Value << camera.up;
            out << YAML::Key << "Is Locked?" << YAML::Value << camera.isLocked;
            out << YAML::EndMap;

            out << YAML::Key << "Is Primary?" << YAML::Value << cc.isPrimary;
            out << YAML::EndMap;
        }

        if (entity->HasComponent<DirectionalLightComponent>())
        {
            out << YAML::Key << "Directional Light Component" << YAML::Value;
            out << YAML::BeginMap;

            auto& dlc = entity->GetComponent<DirectionalLightComponent>();
            out << YAML::Key << "Intensity" << YAML::Value << dlc.light.intensity;
            out << YAML::Key << "Direction" << YAML::Value << dlc.light.direction;
            out << YAML::Key << "Color" << YAML::Value << dlc.light.color;

            out << YAML::EndMap;
        }

        if (entity->HasComponent<MeshComponent>())
        {
            out << YAML::Key << "Mesh Component" << YAML::Value;
            out << YAML::BeginMap;

            auto& mc = entity->GetComponent<MeshComponent>();
            out << YAML::Key << "Name" << YAML::Value << mc.mesh.name;
            out << YAML::Key << "Path" << YAML::Value << mc.mesh.path;

            out << YAML::Key << "Material" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "Diffuse" << YAML::Value << mc.mesh.material.diffuse;
            out << YAML::EndMap;

            out << YAML::EndMap;
        }

        out << YAML::EndMap;
    }

    void SceneSerializer::Serialize(const char* path)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        for (auto& entity : m_context->GetEntityManager().GetEntities())
        {
            if (!entity)
                return;

            SerializeEntity(out, entity);
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(path);
        if (!fout.is_open())
            WARN("Failed to serialize scene to %s!", path);

        fout << out.c_str() << "\n";
        fout.close();

        INFO("Successfully serialized scene to %s!", path);
    }

    void SceneSerializer::Deserialize(const char* path)
    {
        std::ifstream in(path);
        if (!in.is_open())
        {
            WARN("Failed to deserialize scene because %s couldn't be found!", path);
            return;
        }
        std::stringstream stream;
        stream << in.rdbuf();

        YAML::Node data = YAML::Load(stream.str());
        if (!data["Scene"])
            return;

        std::string sceneName = data["Scene"].as<std::string>();

        EntityManager& entityManager = m_context->GetEntityManager();
        YAML::Node entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                u64 uuid = entity["Entity"].as<u64>();
                std::string name = entity["Tag"].as<std::string>();

                std::shared_ptr<Entity> deserializedEntity = entityManager.AddEntity(name.c_str());
                auto transformComponent = entity["Transform Component"];
                if (transformComponent)
                {
                    auto& tc = deserializedEntity->GetComponent<TransformComponent>();

                    tc.position = transformComponent["Position"].as<glm::vec3>();
                    tc.rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.scale = transformComponent["Scale"].as<glm::vec3>();
                }

                auto cameraComponent = entity["Camera Component"];
                if (cameraComponent)
                {
                    const auto& cameraProperties = cameraComponent["Camera"];
                    auto& cc = deserializedEntity->AddComponent<CameraComponent>();

                    cc.camera.fov = cameraProperties["FOV"].as<float>();
                    cc.camera.speed = cameraProperties["Speed"].as<float>();
                    cc.camera.up = cameraProperties["Up"].as<glm::vec3>();
                    cc.camera.position = transformComponent["Position"].as<glm::vec3>();
                    cc.camera.rotation = transformComponent["Rotation"].as<glm::vec3>();
                    cc.isPrimary = cameraComponent["Is Primary?"].as<bool>();
                }

                auto directionalLightComponent = entity["Directional Light Component"];
                if (directionalLightComponent)
                {
                    auto& dlc = deserializedEntity->AddComponent<DirectionalLightComponent>();

                    dlc.light.intensity = directionalLightComponent["Intensity"].as<float>();
                    dlc.light.color = directionalLightComponent["Color"].as<glm::vec3>();
                    dlc.light.direction = directionalLightComponent["Direction"].as<glm::vec3>();
                }

                auto meshComponent = entity["Mesh Component"];
                if (meshComponent)
                {
                    const auto& materialProperties = meshComponent["Material"];
                    auto& mc = deserializedEntity->AddComponent<MeshComponent>();
                    std::string name = meshComponent["Name"].as<std::string>();
                    std::string path = meshComponent["Path"].as<std::string>();

                    Graphics::Mesh* mesh = AssetManager->GetMesh(name.c_str());
                    if (mesh == NULL)
                    {
                        AssetManager->AddMesh(name.c_str(), path.c_str());
                        mesh = AssetManager->GetMesh(name.c_str());
                    }

                    mc.mesh = *mesh;
                    mc.mesh.material.diffuse = materialProperties["Diffuse"].as<glm::vec3>();
                }
            }
        }

        INFO("Successfully deserialized scene %s!", path);
    }
}
