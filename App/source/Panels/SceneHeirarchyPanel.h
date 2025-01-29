#pragma once
#include <Zen.h>

using namespace Core;
using namespace Graphics;

class SceneHeirarchyPanel
{
public:
    SceneHeirarchyPanel() = default;

    void SetContext(Scene* scene);
    void Display();

private:
    void DrawEntityNode(std::shared_ptr<Entity>& entity);
    void DrawComponents(std::shared_ptr<Entity>& entity);

private:
    Scene* m_context = NULL;
    std::shared_ptr<Entity> m_selectionContext = NULL;
};
