#include "ZenEditor.h"
#include "Panels/SceneViewportPanel.h"

#include <imgui.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>

static Mesh roomMesh;
static Mesh miiMesh;
static Shader phongShader;
static Shader postProcessingShader;

ZenEditor::ZenEditor(const ApplicationSpecification& spec) : Application(spec)
{
    roomMesh = LoadMesh("assets/models/room.fbx");
    miiMesh = LoadMesh("assets/models/mii.fbx");

    m_directionalLight.intensity = 1.5f;
    m_directionalLight.direction = glm::vec3(0.2f, -0.86f, -0.95f);

    phongShader = LoadShader("assets/shaders/Default_vs.glsl", "assets/shaders/Default_fs.glsl");
    phongShader.CreateMaterialUniform("material");
    phongShader.CreateDirectionalLightUniform("directionalLight");
    phongShader.CreateUniform("transformMatrix");
    phongShader.CreateUniform("normalMatrix");
    phongShader.CreateUniform("viewMatrix");
    phongShader.CreateUniform("projectionMatrix");

    postProcessingShader = LoadShader("assets/shaders/PostProcessing_vs.glsl", "assets/shaders/PostProcessing_fs.glsl");
    postProcessingShader.CreateUniform("screenTexture");
    postProcessingShader.CreateUniform("projectionMatrix");

    m_camera.position = glm::vec3(-8.5f, 4.2f, 10.f);
    m_camera.rotation = glm::vec3(-53.3f, -33.4f, 0.f);
    Renderer->SetPrimaryCamera(&m_camera);

    m_room = m_entityManager.AddEntity("Room");
    m_room->AddComponent<TransformComponent>();
    m_room->AddComponent<MeshComponent>(roomMesh);

    m_mii = m_entityManager.AddEntity("Mii");
    m_mii->AddComponent<TransformComponent>();
    m_mii->AddComponent<MeshComponent>(miiMesh);

    SceneViewportPanel::SetPostFXShader(postProcessingShader);
}

void ZenEditor::OnShutdown()
{
    UnloadShader(phongShader);
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
    BindShader(phongShader);

    Renderer->Prepare(m_directionalLight, phongShader);

    Renderer->CullFace(FaceCull::Front);
    Renderer->DrawEntity(m_room, phongShader);

    Renderer->CullFace(FaceCull::Back);
    Renderer->DrawEntity(m_mii, phongShader);

    UnbindShader();
}

void ZenEditor::OnRenderUI()
{
    ImGui::DockSpaceOverViewport();
    m_sceneViewportPanel.Display();
}
