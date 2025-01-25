#include "ZenEditor.h"
#include "Panels/SceneViewportPanel.h"

#include <imgui.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glad/glad.h>

static Material cubeMaterial;
static Material miiMaterial;
static Material collectableMaterial;

static Mesh cubeMesh;
static Mesh miiMesh;
static Mesh collectableMesh;

static Shader postProcessingShader;
static Shader instancingShader;

ZenEditor::ZenEditor(const ApplicationSpecification& spec) : Application(spec)
{
    cubeMaterial = LoadMaterial("assets/models/cube.glb");
    cubeMaterial.diffuse = glm::vec3(0.8f, 0.7f, 0.2f);
    miiMaterial = LoadMaterial("assets/models/mii.fbx");
    collectableMaterial = LoadMaterial("assets/models/collectable.glb");

    cubeMesh = LoadMesh("assets/models/cube.glb", &cubeMaterial);
    miiMesh = LoadMesh("assets/models/mii.fbx", &miiMaterial);
    collectableMesh = LoadMesh("assets/models/collectable.glb", &collectableMaterial);

    m_framebuffer = CreateFramebuffer(2);
    m_framebuffer.attachments[0] = CreateEmptyTexture(spec.windowWidth, spec.windowHeight, TextureFormat::RGB16F);
    m_framebuffer.attachments[1] = CreateEmptyTexture(spec.windowWidth, spec.windowHeight, TextureFormat::DepthStencil);
    ApplyFramebufferAttachments(m_framebuffer);

    m_directionalLight.intensity = 3.f;
    m_directionalLight.direction = glm::vec3(0.2f, -0.86f, -0.95f);

    postProcessingShader = LoadShader("assets/shaders/PostProcessing_vs.glsl", "assets/shaders/PostProcessing_fs.glsl");
    postProcessingShader.CreateUniform("screenTexture");
    postProcessingShader.CreateUniform("projectionMatrix");
    postProcessingShader.CreateUniform("gamma");
    postProcessingShader.CreateUniform("exposure");

    instancingShader = LoadShader("assets/shaders/Instancing_vs.glsl", "assets/shaders/Default_fs.glsl");
    instancingShader.CreateMaterialUniform("material");
    instancingShader.CreateDirectionalLightUniform("directionalLight");
    instancingShader.CreateUniform("cameraPosition");
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
        entity = m_entityManager.AddEntity("Cube");
        entity->AddComponent<TransformComponent>(position, glm::vec3(0.f), glm::vec3(1.f));
        entity->AddComponent<MeshComponent>(&cubeMesh);
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

    for (u32 i = 0; i < LEN(m_miis); i++)
    {
        glm::vec3 position;
        position.x = rand() % 100 - 50;
        position.y = rand() % 50 - 25;
        position.z = rand() % 100 - 50;

        std::shared_ptr<Entity>& entity = m_collectables[i];
        entity = m_entityManager.AddEntity("Collectable");
        entity->AddComponent<TransformComponent>(position, glm::vec3(0.f), glm::vec3(1.f));
        entity->AddComponent<MeshComponent>(&collectableMesh);
    }

    SceneViewportPanel::SetPostFXShader(&postProcessingShader);
}

void ZenEditor::OnShutdown()
{
    DestroyFramebuffer(m_framebuffer);

    UnloadShader(instancingShader);
    UnloadShader(postProcessingShader);

    UnloadMesh(cubeMesh);
    UnloadMesh(miiMesh);
    UnloadMesh(collectableMesh);
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
    BindFramebuffer(m_framebuffer);
    Renderer->Clear();

    if (Renderer->GetPrimaryCamera() != NULL)
    {
        BindShader(instancingShader);

        Renderer->Prepare(m_directionalLight, instancingShader);

        for (auto& entity : m_entityManager.GetEntities())
            Renderer->ProcessEntity(entity);

        Renderer->DrawEntities(instancingShader);

        UnbindShader();
    }

    UnbindFramebuffer();
}

void ZenEditor::OnRenderUI()
{
    ImGui::DockSpaceOverViewport();
    m_sceneViewportPanel.Display(m_framebuffer);
}
