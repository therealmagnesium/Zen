#include "ZenEditor.h"
#include "Panels/SceneViewportPanel.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

static Shader instancingShader;
static Shader phongShader;
static Shader postProcessingShader;
static Shader skyboxShder;

static const char* skyboxTexturePaths[6];

ZenEditor::ZenEditor(const ApplicationSpecification& spec) : Application(spec)
{
    this->SetupAssets();
    this->CreateNewScene(true);

    m_contentBrowserPanel.Initialize();

    m_framebuffer = CreateFramebuffer(2);
    m_framebuffer.attachments[0] = CreateEmptyTexture(spec.windowWidth, spec.windowHeight, TextureFormat::RGB16F);
    m_framebuffer.attachments[1] = CreateEmptyTexture(spec.windowWidth, spec.windowHeight, TextureFormat::DepthStencil);
    ApplyFramebufferAttachments(m_framebuffer);

    this->SetupShaders();
    this->SetupSkybox();
}

void ZenEditor::OnShutdown()
{
    m_contentBrowserPanel.Shutdown();

    DestroyFramebuffer(m_framebuffer);

    UnloadSkybox(m_skybox);

    UnloadShader(instancingShader);
    UnloadShader(phongShader);
    UnloadShader(postProcessingShader);
    UnloadShader(skyboxShder);
}

void ZenEditor::OnUpdate()
{
    m_activeScene.Update();

    if (IsKeyDown(KEY_LEFT_CTRL))
    {
        if (IsKeyPressed(KEY_N))
            this->CreateNewScene(true);
        if (IsKeyPressed(KEY_O))
            this->OpenSceneDialog();
        if (IsKeyPressed(KEY_S))
            this->SaveSceneDialog();
    }
}

void ZenEditor::OnRender()
{
    BindFramebuffer(m_framebuffer);
    Renderer->Clear(true, true, false);

    if (Renderer->GetPrimaryCamera() != NULL)
    {
        Renderer->DrawSkybox(m_skybox, AssetManager->GetMesh("Cube"), skyboxShder);
        Renderer->Prepare(phongShader);
        m_activeScene.DrawEntities(phongShader);
    }

    UnbindFramebuffer();
}

void ZenEditor::OnRenderUI()
{
    ImGui::DockSpaceOverViewport();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N"))
                this->CreateNewScene(true);
            if (ImGui::MenuItem("Load", "Ctrl+O"))
                this->OpenSceneDialog();
            if (ImGui::MenuItem("Save", "Ctrl+S"))
                this->SaveSceneDialog();
            if (ImGui::MenuItem("Exit"))
                this->Quit();

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    m_contentBrowserPanel.Display();
    m_sceneHeirarchyPanel.Display();
    m_sceneViewportPanel.Display(m_framebuffer, postProcessingShader);

    ImGui::Begin("Assets");

    ImGui::SeparatorText("Meshes");
    for (std::string& name : AssetManager->GetAllMeshNames())
        ImGui::Text("%s", name.c_str());

    ImGui::SeparatorText("Textures");
    for (std::string& name : AssetManager->GetAllTextureNames())
        ImGui::Text("%s", name.c_str());

    ImGui::End();
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

    phongShader = LoadShader("assets/shaders/Default_vs.glsl", "assets/shaders/Default_fs.glsl");
    phongShader.CreateMaterialUniform("material");
    phongShader.CreateDirectionalLightUniform("directionalLight");
    phongShader.CreateUniform("cameraPosition");
    phongShader.CreateUniform("transformMatrix");
    phongShader.CreateUniform("normalMatrix");
    phongShader.CreateUniform("viewMatrix");
    phongShader.CreateUniform("projectionMatrix");

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

void ZenEditor::SetupAssets()
{
    AssetManager->AddMesh("Collectable", "assets/models/collectable.fbx");
    AssetManager->AddMesh("Cone", "assets/models/cone.fbx");
    AssetManager->AddMesh("Cube", "assets/models/cube.fbx");
    AssetManager->AddMesh("Jupiter", "assets/models/jupiter.fbx");
    AssetManager->AddMesh("Mii", "assets/models/mii.fbx");
    AssetManager->AddMesh("Sphere", "assets/models/sphere.fbx");
    AssetManager->AddMesh("Terrain", "assets/models/Terrain_1.fbx");
    AssetManager->AddMesh("X Bot", "assets/models/xbot.fbx");
    AssetManager->AddMesh("Y Bot", "assets/models/ybot.fbx");
}

void ZenEditor::CreateNewScene(bool addDefaultEntities)
{
    m_activeScene = Scene();
    m_activeScene.Initialize(addDefaultEntities);

    m_sceneHeirarchyPanel.SetContext(&m_activeScene);
    m_sceneViewportPanel.SetContext(&m_activeScene);
}

void ZenEditor::OpenSceneDialog()
{
    std::string path = FileDialogs::OpenFile("zen", "scenes/");

    if (!path.empty())
    {
        this->CreateNewScene(false);

        m_sceneSerializer.SetContext(&m_activeScene);
        m_sceneSerializer.Deserialize(path.c_str());
    }
}

void ZenEditor::SaveSceneDialog()
{
    std::string path = FileDialogs::SaveFile("zen", "scenes/");

    if (!path.empty())
    {
        m_sceneSerializer.SetContext(&m_activeScene);
        m_sceneSerializer.Serialize(path.c_str());
    }
}
