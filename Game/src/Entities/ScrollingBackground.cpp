#include "precomp.h"

#include "Entities/GameComponents.h"
#include "ScrollingBackground.h"

namespace Game
{
    bool ScrollingBackground::Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float w, float h)
    {
        ASSERT(texture_ != nullptr, "nullptr passed as texture_");
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to CameraController::Init()");

        auto imageEntity = std::make_unique<Engine::Entity>();
        imageEntity->AddComponent<Game::Background>();
        imageEntity->AddComponent<Engine::TransformComponent>(0.f, 0.f, w, h);
        imageEntity->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(imageEntity));

        imageEntity = std::make_unique<Engine::Entity>();
        imageEntity->AddComponent<Game::Background>();
        imageEntity->AddComponent<Engine::TransformComponent>(w, 0.f, w, h);
        imageEntity->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(imageEntity));
        

        return true;
    }

    void ScrollingBackground::Update(float dt, Engine::EntityManager* entityManager_, float boundary_)
    {
        auto backgrounds = entityManager_->GetAllEntitiesWithComponents<Game::Background, Engine::TransformComponent>();
        for (auto& background : backgrounds) {
            auto backgroundTransformer = background->GetComponent<Engine::TransformComponent>();
            float backgroundPosition = backgroundTransformer->m_Position.x;
            float backgroundWidth = backgroundTransformer->m_Size.x;
            float backgroundBoundary = backgroundPosition + backgroundWidth / 2;

             if (backgroundBoundary <= boundary_) {
                backgroundTransformer->m_Position.x = backgroundPosition + 2 * backgroundWidth;
            }

        }
    }
}

