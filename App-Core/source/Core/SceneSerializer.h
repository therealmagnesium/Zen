#pragma once
#include "Core/Scene.h"

namespace Core
{
    class SceneSerializer
    {
    public:
        SceneSerializer() = default;
        SceneSerializer(Scene* context) : m_context(context) {}

        inline void SetContext(Scene* context) { m_context = context; }

        void Serialize(const char* path);
        void Deserialize(const char* path);

    private:
        Scene* m_context = NULL;
    };
}
