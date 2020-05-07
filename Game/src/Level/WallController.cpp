#include "precomp.h"
#include "WallController.h"
#include "Entities/GameComponents.h"
#include "Core/GameConstants.h"

namespace Game
{
    void WallController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        CreateWalls(entityManager_, textureManager_);
    }

    void WallController::CreateWalls(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        float wallPosition = (GameConstants::SCREEN_HEIGHT - GameConstants::WALL_HEIGHT) / 2;
        auto wall = std::make_unique<Engine::Entity>();

        // Down
        wall->AddComponent<Game::WallComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, wallPosition, GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::CollisionComponent>(GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("floor");
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));

        // Up
        wall = std::make_unique<Engine::Entity>();

        wall->AddComponent<Game::WallComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, -wallPosition, GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::CollisionComponent>(GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("ceiling");
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));
    }

    void WallController::MoveWalls(Engine::EntityManager* entityManager_, float speed_)
    {
        auto walls = entityManager_->GetAllEntitiesWithComponents<Game::WallComponent, Engine::MoverComponent>();

        for (auto& wall : walls)
        {
            auto move = wall->GetComponent<Engine::MoverComponent>();
            move->m_TranslationSpeed.x = speed_;
        }
    }
}

