#include "ZenEditor.h"
#include "Panels/SceneViewportPanel.h"

#include <imgui.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>

static Mesh roomMesh;
static Mesh miiMesh;

static Shader postProcessingShader;
static Shader instancingShader;

ZenEditor::ZenEditor(const ApplicationSpecification& spec) : Application(spec)
{
    roomMesh = LoadMesh("assets/models/room.fbx");
    miiMesh = LoadMesh("assets/models/mii.fbx");

    m_directionalLight.intensity = 1.5f;
    m_directionalLight.direction = glm::vec3(0.2f, -0.86f, -0.95f);

    postProcessingShader = LoadShader("assets/shaders/PostProcessing_vs.glsl", "assets/shaders/PostProcessing_fs.glsl");
    postProcessingShader.CreateUniform("screenTexture");
    postProcessingShader.CreateUniform("projectionMatrix");

    instancingShader = LoadShader("assets/shaders/Instancing_vs.glsl", "assets/shaders/Default_fs.glsl");
    instancingShader.CreateMaterialUniform("material");
    instancingShader.CreateDirectionalLightUniform("directionalLight");
    instancingShader.CreateUniform("normalMatrix");
    instancingShader.CreateUniform("viewMatrix");
    instancingShader.CreateUniform("projectionMatrix");

    m_camera.position = glm::vec3(-8.5f, 4.2f, 10.f);
    m_camera.rotation = glm::vec3(-53.3f, -33.4f, 0.f);
    Renderer->SetPrimaryCamera(&m_camera);

    for (u32 i = 0; i < LEN(m_boxes); i++)
    {
        glm::vec3 position;
        position.x = rand() % 100 - 50;
        position.y = rand() % 50 - 25;
        position.z = rand() % 100 - 50;

        std::shared_ptr<Entity>& entity = m_boxes[i];
        entity = m_entityManager.AddEntity("Box");
        entity->AddComponent<TransformComponent>(position, glm::vec3(0.f), glm::vec3(0.4f));
        entity->AddComponent<MeshComponent>(&roomMesh);
    }

    for (u32 i = 0; i < LEN(m_miis); i++)
    {
        glm::vec3 position;
        position.x = rand() % 100 - 50;
        position.y = rand() % 50 - 25;
        position.z = rand() % 100 - 50;

        std::shared_ptr<Entity>& entity = m_miis[i];
        entity = m_entityManager.AddEntity("Mii");
        entity->AddComponent<TransformComponent>(position, glm::vec3(0.f), glm::vec3(1.f));
        entity->AddComponent<MeshComponent>(&miiMesh);
    }

    SceneViewportPanel::SetPostFXShader(postProcessingShader);
}

void ZenEditor::OnShutdown()
{
    UnloadShader(instancingShader);
    UnloadShader(postProcessingShader);

    UnloadMesh(roomMesh);
    UnloadMesh(miiMesh);
}

void ZenEditor::OnUpdate()
{
    m_entityManager.Update();

    if (IsKeyPressed(KEY_F8))
        LogCameraInfo(m_camera);

    UpdateCameraController(m_camera);
    UpdateCamera(m_camera);
}

void ZenEditor::OnRender()
{
    BindShader(instancingShader);

    Renderer->Prepare(m_directionalLight, instancingShader);

    for (auto& entity : m_entityManager.GetEntities())
        Renderer->ProcessEntity(entity);

    Renderer->DrawEntities(instancingShader);

    UnbindShader();
}

void ZenEditor::OnRenderUI()
{
    ImGui::DockSpaceOverViewport();
    m_sceneViewportPanel.Display();
}
