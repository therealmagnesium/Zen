#pragma once
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
    Camera m_camera;
    DirectionalLight m_directionalLight;
    EntityManager m_entityManager;
    SceneViewportPanel m_sceneViewportPanel;
    Framebuffer m_framebuffer;

    std::shared_ptr<Entity> m_boxes[1000];
    std::shared_ptr<Entity> m_miis[1000];
    std::shared_ptr<Entity> m_collectables[1000];
};
