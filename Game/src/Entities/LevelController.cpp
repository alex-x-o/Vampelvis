#include "precomp.h"
#include "LevelController.h"

#include "Entities/GameComponents.h"

namespace Game
{
    bool Level::Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to BallController::Init()");


        auto wall = std::make_unique<Engine::Entity>();


        // Down

        wall->AddComponent<WallComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, 300.f, 1300.f, 30.f);
        wall->AddComponent<Engine::CollisionComponent>(1300.f, 30.f);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(wall));

        // Up
        wall = std::make_unique<Engine::Entity>();

        wall->AddComponent<WallComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, -300.f, 1300.f, 30.f);
        wall->AddComponent<Engine::CollisionComponent>(1300.f, 30.f);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(wall));

        return true;
    }
}