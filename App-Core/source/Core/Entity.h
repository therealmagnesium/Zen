#pragma once
#include "Core/Base.h"
#include "Core/Components.h"

#include <assert.h>
#include <string>
#include <tuple>
#include <utility>

namespace Core
{
    typedef std::tuple<TransformComponent, MeshComponent, DirectionalLightComponent, CameraComponent> ComponentTuple;

    class Entity
    {
    public:
        Entity() = default;
        Entity(u64 id, const char* tag) : m_id(id), m_tag(tag) {}

        inline u64 GetID() const { return m_id; }
        inline bool IsActive() const { return m_isActive; }
        inline bool IsAlive() const { return m_isAlive; }
        inline const char* GetTag() { return m_tag.c_str(); }

        inline void SetActive(bool active) { m_isActive = active; }
        inline void SetTag(const char* tag) { m_tag = tag; }
        inline void Destroy() { m_isAlive = false; }

        template <typename T>
        T& GetComponent()
        {
            return std::get<T>(m_components);
        }
        template <typename T>
        const T& GetComponent() const
        {
            return std::get<T>(m_components);
        }
        template <typename T>
        bool HasComponent() const
        {
            return GetComponent<T>().isEnabled;
        }
        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args)
        {
            T& component = GetComponent<T>();
            component = T(std::forward<TArgs>(args)...);
            component.isEnabled = true;
            return component;
        }
        template <typename T>
        void RemoveComponent()
        {
            GetComponent<T>() = T();
        }

        bool operator==(const Entity& other) const { return m_id == other.m_id; }
        bool operator!=(const Entity& other) const { return !(operator==(other)); }

    private:
        friend class EntityManager;

        u64 m_id = 0;
        bool m_isActive = false;
        bool m_isAlive = true;
        std::string m_tag;
        ComponentTuple m_components;
    };

}
