#include "Core/EntityManager.h"
#include "Core/Components.h"
#include "Core/Log.h"

#include <assert.h>
#include <memory>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Core
{
    std::shared_ptr<Entity> EntityManager::AddEntity(const char* tag)
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(m_totalEntities++, tag);
        entity->AddComponent<TransformComponent>();
        entity->SetActive(true);
        m_toAdd.push_back(entity);
        return entity;
    }

    void EntityManager::DestroyEntity(std::shared_ptr<Entity>& entity)
    {
        auto entityIndex = std::find(m_entities.begin(), m_entities.end(), entity);
        if (entityIndex == m_entities.end())
            return;

        entity->SetActive(false);
        m_toDestroy.push_back(entity);
    }

    void EntityManager::Flush()
    {
        for (auto& entity : m_toAdd)
        {
            const char* entityTag = entity->GetTag();
            m_entities.push_back(entity);
            // m_entityMap[entityTag].push_back(entity);
        }

        for (u64 i = 0; i < m_toDestroy.size(); i++)
        {
            auto& entity = m_toDestroy[i];

            m_entities.erase(m_entities.begin() + entity->GetID());
            /*
                m_entityMap[entity->GetTag()].erase(m_entityMap[entity->GetTag()].begin() +
                                                    entity->GetID());*/
        }

        for (u64 i = 0; i < m_entities.size(); i++)
        {
            auto& entity = m_entities[i];
            const char* entityTag = entity->GetTag();
            if (!entity->IsAlive())
            {
                m_entities.erase(m_entities.begin() + i);
                // m_entityMap[entityTag].erase(m_entityMap[entityTag].begin() + i);
            }
        }
    }
    void EntityManager::Update()
    {
        this->Flush();

        m_toAdd.clear();
        m_toDestroy.clear();

        for (auto& entity : m_entities)
        {
            if (entity->HasComponent<MeshComponent>())
            {
                auto& tc = entity->GetComponent<TransformComponent>();
                auto& mc = entity->GetComponent<MeshComponent>();

                tc.transform = glm::mat4(1.f);
                tc.transform = glm::translate(tc.transform, tc.position);
                tc.transform = glm::rotate(tc.transform, glm::radians(tc.rotation.x), glm::vec3(1.f, 0.f, 0.f));
                tc.transform = glm::rotate(tc.transform, glm::radians(tc.rotation.y), glm::vec3(0.f, 1.f, 0.f));
                tc.transform = glm::rotate(tc.transform, glm::radians(tc.rotation.z), glm::vec3(0.f, 0.f, 1.f));
                tc.transform = glm::scale(tc.transform, tc.scale);
            }
        }
    }

    EntityVec& EntityManager::GetEntities()
    {
        return m_entities;
    }

    EntityVec& EntityManager::GetEntities(const char* tag)
    {
        return m_entityMap[tag];
    }
}
