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
        
        imageEntity = std::make_unique<Engine::Entity>();
        imageEntity->AddComponent<Game::BackgroundSwitcher>();
        imageEntity->AddComponent<Game::Background>();
        imageEntity->AddComponent<Engine::TransformComponent>(w, 0.f, w, h);
        imageEntity->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(imageEntity));

        return true;
    }

    void ScrollingBackground::Update(Engine::EntityManager* entityManager_, float boundary_)
    {
        
        auto switcher = entityManager_->GetAllEntitiesWithComponents<Game::BackgroundSwitcher, Engine::TransformComponent>().front();
        auto switcherTransformer = switcher->GetComponent<Engine::TransformComponent>();
        auto switcherSprite = switcher->GetComponent<Engine::SpriteComponent>();
        float switcherPosX = switcherTransformer->m_Position.x;
        float switcherBoundary = switcherPosX + switcherTransformer->m_Size.x / 2;

        auto backgrounds = entityManager_->GetAllEntitiesWithComponents<Game::Background, Engine::TransformComponent>();
        for (auto& background : backgrounds) {
            auto backgroundSprite = background->GetComponent<Engine::SpriteComponent>();
            auto backgroundTransformer = background->GetComponent<Engine::TransformComponent>();
            float backgroundPosition = backgroundTransformer->m_Position.x;
            float backgroundWidth = backgroundTransformer->m_Size.x;
            float backgroundBoundary = backgroundPosition + backgroundWidth / 2;
            float backgroundPosX = backgroundTransformer->m_Position.x;

            if (backgroundBoundary <= boundary_) {
                if (backgroundPosX == switcherPosX || switcherPosX == backgroundPosX + backgroundWidth)
                {
                    backgroundTransformer->m_Position.x = backgroundPosition + 2 * backgroundWidth;

                }
                else {
                    float coef = m_Synced ? 1.f : 0.f;
                    backgroundTransformer->m_Position.x = switcherPosX + coef * backgroundWidth;

                    m_Synced = !m_Synced;
                }
                backgroundSprite->m_Image = switcherSprite->m_Image;
                 
            }
            else
            {
                if (switcherPosX <= boundary_ + backgroundWidth / 2)
                {
                    float coef = m_Synced ? 1.f : 0.f;
                    backgroundTransformer->m_Position.x = switcherPosX + coef * backgroundWidth;

                    m_Synced = !m_Synced;
                    backgroundSprite->m_Image = switcherSprite->m_Image;
                }     
            }

        }
    }
    void ScrollingBackground::ChangeAtX(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x_)
    {
        auto switcher = entityManager_->GetAllEntitiesWithComponents<Game::BackgroundSwitcher, Engine::TransformComponent>().front();
        auto switcherTransformer = switcher->GetComponent<Engine::TransformComponent>();
        auto switcherSprite = switcher->GetComponent<Engine::SpriteComponent>();
        switcherTransformer->m_Position.x = switcherTransformer->m_Size.x / 2 + floor(x_);

        m_Synced = false;
        switcherSprite->m_Image = texture_;
        
    }
}

