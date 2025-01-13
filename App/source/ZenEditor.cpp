#include "ZenEditor.h"
#include "Panels/SceneViewportPanel.h"

#include <imgui.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>

static Mesh mesh;
static Shader phongShader;
static Shader postProcessingShader;

ZenEditor::ZenEditor(const ApplicationSpecification& spec) : Application(spec)
{
    mesh = LoadMesh("assets/models/mii.fbx");

    m_directionalLight.intensity = 1.5f;
    m_directionalLight.direction = glm::vec3(0.2f, -0.86f, -0.95f);

    phongShader = LoadShader("assets/shaders/Default_vs.glsl", "assets/shaders/Default_fs.glsl");
    phongShader.CreateMaterialUniform("material");
    phongShader.CreateDirectionalLightUniform("directionalLight");
    phongShader.CreateUniform("transformMatrix");
    phongShader.CreateUniform("normalMatrix");
    phongShader.CreateUniform("viewMatrix");
    phongShader.CreateUniform("projectionMatrix");
    Renderer->SetPrimaryShader(&phongShader);

    postProcessingShader = LoadShader("assets/shaders/PostProcessing_vs.glsl", "assets/shaders/PostProcessing_fs.glsl");
    postProcessingShader.CreateUniform("screenTexture");
    postProcessingShader.CreateUniform("projectionMatrix");

    m_camera.position = glm::vec3(-8.5f, 4.2f, 10.f);
    m_camera.rotation = glm::vec3(-53.3f, -33.4f, 0.f);
    Renderer->SetPrimaryCamera(&m_camera);

    for (s32 i = -5; i < 5; i++)
    {
        for (s32 j = -5; j < 5; j++)
        {
            m_entities[(i + 5) * 10 + (j + 5)] = m_entityManager.AddEntity("Entity");
            auto& tc = m_entities[(i + 5) * 10 + (j + 5)]->GetComponent<TransformComponent>();
            auto& mc = m_entities[(i + 5) * 10 + (j + 5)]->AddComponent<MeshComponent>(&mesh);

            tc.position.x = i * 1.8f;
            tc.position.y = 0.f;
            tc.position.z = j * 1.8f;
        }
    }

    SceneViewportPanel::SetPostFXShader(postProcessingShader);
}

void ZenEditor::OnShutdown()
{
    UnloadShader(phongShader);
    UnloadShader(postProcessingShader);
    UnloadMesh(mesh);
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
    for (u32 i = 0; i < LEN(m_entities); i++)
        Renderer->ProcessEntity(m_entities[i]);

    Renderer->DrawAllMeshes(m_directionalLight);
}

void ZenEditor::OnRenderUI()
{
    ImGui::DockSpaceOverViewport();
    m_sceneViewportPanel.Display();
}
