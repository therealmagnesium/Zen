#pragma once
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SceneHeirarchyPanel.h"
#include "Panels/SceneViewportPanel.h"

#include <Zen.h>
#include <imgui.h>

using namespace Core;
using namespace Graphics;

class ZenEditor : public Application
{
public:
    ZenEditor(const ApplicationSpecification& spec);

    void OnUpdate() override;
    void OnRender() override;
    void OnRenderUI() override;
    void OnShutdown() override;

private:
    void SetupAssets();
    void SetupShaders();
    void SetupSkybox();

    void CreateNewScene(bool addDefaultEntities);
    void OpenSceneDialog();
    void SaveSceneDialog();

private:
    Skybox m_skybox;
    Framebuffer m_framebuffer;
    Scene m_activeScene;
    SceneSerializer m_sceneSerializer;

    ContentBrowserPanel m_contentBrowserPanel;
    SceneHeirarchyPanel m_sceneHeirarchyPanel;
    SceneViewportPanel m_sceneViewportPanel;

    std::shared_ptr<Entity> m_entity;
};
