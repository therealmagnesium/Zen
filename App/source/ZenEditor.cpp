#include "ZenEditor.h"
#include "Panels/SceneViewportPanel.h"

#include <imgui.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

static Mesh cubeMesh;
static Mesh miiMesh;
static Mesh collectableMesh;

static Shader postProcessingShader;
static Shader skyboxShder;
static Shader instancingShader;

static const char* skyboxTexturePaths[6];

ZenEditor::ZenEditor(const ApplicationSpecification& spec) : Application(spec)
{
    cubeMesh = LoadMesh("assets/models/cube.glb");
    cubeMesh.material.diffuse = glm::vec3(0.8f, 0.7f, 0.2f);
    miiMesh = LoadMesh("assets/models/mii.fbx");
    collectableMesh = LoadMesh("assets/models/collectable.glb");

    m_framebuffer = CreateFramebuffer(2);
    m_framebuffer.attachments[0] = CreateEmptyTexture(spec.windowWidth, spec.windowHeight, TextureFormat::RGB16F);
    m_framebuffer.attachments[1] = CreateEmptyTexture(spec.windowWidth, spec.windowHeight, TextureFormat::DepthStencil);
    ApplyFramebufferAttachments(m_framebuffer);

    m_directionalLight.intensity = 3.f;
    m_directionalLight.direction = glm::vec3(0.2f, -0.86f, -0.95f);

    this->SetupShaders();
    this->SetupSkybox();

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

    for (u32 i = 0; i < LEN(m_collectables); i++)
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

    UnloadSkybox(m_skybox);

    UnloadShader(instancingShader);
    UnloadShader(skyboxShder);
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
    Renderer->Clear(true, true, false);

    if (Renderer->GetPrimaryCamera() != NULL)
    {
        Renderer->CullFace(FaceCull::Front);
        Renderer->DrawSkybox(m_skybox, cubeMesh, skyboxShder);

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

void ZenEditor::SetupShaders()
{
    postProcessingShader = LoadShader("assets/shaders/PostProcessing_vs.glsl", "assets/shaders/PostProcessing_fs.glsl");
    postProcessingShader.CreateUniform("screenTexture");
    postProcessingShader.CreateUniform("projectionMatrix");
    postProcessingShader.CreateUniform("gamma");
    postProcessingShader.CreateUniform("exposure");

    skyboxShder = LoadShader("assets/shaders/Skybox_vs.glsl", "assets/shaders/Skybox_fs.glsl");
    skyboxShder.CreateUniform("viewMatrix");
    skyboxShder.CreateUniform("projectionMatrix");
    skyboxShder.CreateUniform("skybox");

    instancingShader = LoadShader("assets/shaders/Instancing_vs.glsl", "assets/shaders/Default_fs.glsl");
    instancingShader.CreateMaterialUniform("material");
    instancingShader.CreateDirectionalLightUniform("directionalLight");
    instancingShader.CreateUniform("cameraPosition");
    instancingShader.CreateUniform("normalMatrix");
    instancingShader.CreateUniform("viewMatrix");
    instancingShader.CreateUniform("projectionMatrix");
}

void ZenEditor::SetupSkybox()
{
    skyboxTexturePaths[0] = "assets/textures/skybox5/posx.png";
    skyboxTexturePaths[1] = "assets/textures/skybox5/negx.png";
    skyboxTexturePaths[2] = "assets/textures/skybox5/posy.png";
    skyboxTexturePaths[3] = "assets/textures/skybox5/negy.png";
    skyboxTexturePaths[4] = "assets/textures/skybox5/posz.png";
    skyboxTexturePaths[5] = "assets/textures/skybox5/negz.png";

    m_skybox = LoadSkybox(skyboxTexturePaths, TextureFormat::RGBA);
}
