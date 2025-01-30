#include "ZenEditor.h"
#include "Panels/SceneViewportPanel.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

static Shader postProcessingShader;
static Shader skyboxShder;
static Shader instancingShader;

static const char* skyboxTexturePaths[6];

ZenEditor::ZenEditor(const ApplicationSpecification& spec) : Application(spec)
{
    this->SetupAssets();

    m_activeScene.Initialize();

    m_directionalLight.intensity = 3.f;
    m_directionalLight.direction = glm::vec3(0.2f, -0.86f, -0.95f);

    m_framebuffer = CreateFramebuffer(2);
    m_framebuffer.attachments[0] = CreateEmptyTexture(spec.windowWidth, spec.windowHeight, TextureFormat::RGB16F);
    m_framebuffer.attachments[1] = CreateEmptyTexture(spec.windowWidth, spec.windowHeight, TextureFormat::DepthStencil);
    ApplyFramebufferAttachments(m_framebuffer);

    this->SetupShaders();
    this->SetupSkybox();
    this->SetupGameObjects();

    m_sceneHeirarchyPanel.SetContext(&m_activeScene);
    m_sceneViewportPanel.SetContext(&m_activeScene);

    SceneViewportPanel::SetPostFXShader(&postProcessingShader);
}

void ZenEditor::OnShutdown()
{
    DestroyFramebuffer(m_framebuffer);

    UnloadSkybox(m_skybox);

    UnloadShader(instancingShader);
    UnloadShader(skyboxShder);
    UnloadShader(postProcessingShader);
}

void ZenEditor::OnUpdate()
{
    m_activeScene.Update();
}

void ZenEditor::OnRender()
{
    BindFramebuffer(m_framebuffer);
    Renderer->Clear(true, true, false);

    if (Renderer->GetPrimaryCamera() != NULL)
    {
        Renderer->CullFace(FaceCull::Front);
        Renderer->DrawSkybox(m_skybox, AssetManager->GetMesh("Cube"), skyboxShder);

        BindShader(instancingShader);

        Renderer->Prepare(m_directionalLight, instancingShader);

        for (auto& entity : m_activeScene.GetEntities())
            Renderer->ProcessEntity(entity);

        Renderer->DrawEntities(instancingShader);

        UnbindShader();
    }

    UnbindFramebuffer();
}

void ZenEditor::OnRenderUI()
{
    UI::BeginFrame();

    ImGui::DockSpaceOverViewport();

    m_sceneViewportPanel.Display(m_framebuffer);
    m_sceneHeirarchyPanel.Display();

    UI::EndFrame();

    UI::RenderFrame();
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

void ZenEditor::SetupGameObjects()
{
    m_entity = m_activeScene.AddEntity("Entity");
    m_entity->AddComponent<MeshComponent>(AssetManager->GetMesh("Jupiter"));
}

void ZenEditor::SetupAssets()
{
    AssetManager->AddMesh("Collectable", "assets/models/collectable.glb");
    AssetManager->AddMesh("Cone", "assets/models/cone.glb");
    AssetManager->AddMesh("Cube", "assets/models/cube.glb");
    AssetManager->AddMesh("Jupiter", "assets/models/jupiter.fbx");
    AssetManager->AddMesh("Mii", "assets/models/mii.fbx");
    AssetManager->AddMesh("Sphere", "assets/models/sphere.glb");
}
