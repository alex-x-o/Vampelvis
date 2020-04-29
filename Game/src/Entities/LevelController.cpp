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
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));

        // Up
        wall = std::make_unique<Engine::Entity>();

        wall->AddComponent<WallComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, -300.f, 1300.f, 30.f);
        wall->AddComponent<Engine::CollisionComponent>(1300.f, 30.f);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = texture_;
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));

        return true;
    }

    void Level::Update(float dt, Engine::EntityManager* entityManager_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to Level::Update()");

        auto walls = entityManager_->GetAllEntitiesWithComponents<Game::WallComponent, Engine::MoverComponent>();

        for (auto& wall : walls)
        {
            auto move = wall->GetComponent<Engine::MoverComponent>();
            move->m_TranslationSpeed.x = 100.f;
        }
    }
}