#include "precomp.h"
#include "WallController.h"

#include "Level.h"
#include "Entities/GameComponents.h"
#include "Core/GameConstants.h"

namespace Game
{
    void WallController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_)
    {
        CreateWalls(entityManager_, textureManager_, level_);
    }

    void WallController::CreateWalls(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_)
    {
        float wallPosition = (GameConstants::SCREEN_HEIGHT - GameConstants::WALL_HEIGHT) / 2;
        auto wall = std::make_unique<Engine::Entity>();

        // Down
        wall->AddComponent<Game::WallComponent>();
        wall->AddComponent<Game::FloorComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, wallPosition, GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::CollisionComponent>(GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetLevelOrCommonTexture(level_->levelId, Game::TEX_WALL, "floor");
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));

        // Up
        wall = std::make_unique<Engine::Entity>();

        wall->AddComponent<Game::WallComponent>();
        wall->AddComponent<Game::CeilingComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, -wallPosition, GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::CollisionComponent>(GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetLevelOrCommonTexture(level_->levelId, Game::TEX_WALL, "ceiling");
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));


        //Add switcher walls
        
        // Down
        wall = std::make_unique<Engine::Entity>();

        wall->AddComponent<Game::WallComponent>();
        wall->AddComponent<Game::WallSwitcher>();
        wall->AddComponent<Game::FloorComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, wallPosition, GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::CollisionComponent>(GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetLevelOrCommonTexture(level_->levelId, Game::TEX_WALL, "floor");
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));

        // Up
        wall = std::make_unique<Engine::Entity>();

        wall->AddComponent<Game::WallComponent>();
        wall->AddComponent<Game::WallSwitcher>();
        wall->AddComponent<Game::CeilingComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, -wallPosition, GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::CollisionComponent>(GameConstants::SCREEN_WIDTH, GameConstants::WALL_HEIGHT);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetLevelOrCommonTexture(level_->levelId, Game::TEX_WALL, "ceiling");
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));
    }

    void WallController::MoveWalls(Engine::EntityManager* entityManager_, float speed_)
    {

        if (!m_CurrentlySwitching)
        {
            auto walls = entityManager_->GetAllEntitiesWithComponents<Game::WallComponent, Engine::MoverComponent>();

            for (auto& wall : walls)
            {
                auto move = wall->GetComponent<Engine::MoverComponent>();
                move->m_TranslationSpeed.x = speed_;
            }
        }

    }

    void WallController::StartSwitchingWalls(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_)
    {
        auto floorTexture = textureManager_->GetLevelOrCommonTexture(level_->levelId, Game::TEX_WALL, "floor");
        auto ceilingTexture = textureManager_->GetLevelOrCommonTexture(level_->levelId, Game::TEX_WALL, "ceiling");
        auto walls = entityManager_->GetAllEntitiesWithComponents<Game::WallComponent, Engine::MoverComponent>();

        for (auto& wall : walls)
        {
            auto move = wall->GetComponent<Engine::MoverComponent>();
            move->m_TranslationSpeed.x = 0;

            if (wall->HasComponent<Game::WallSwitcher>())
            { 
                auto wallTransformer = wall->GetComponent<Engine::TransformComponent>();
                wallTransformer->m_Position.x = level_->levelStart + wallTransformer->m_Size.x / 2;

                auto wallSprite = wall->GetComponent<Engine::SpriteComponent>();
                if (wall->HasComponent<Game::FloorComponent>())
                {
                    wallSprite->m_Image = floorTexture;
                }
                else
                {
                    wallSprite->m_Image = ceilingTexture;
                }
            }
        }

        m_CurrentlySwitching = true;
    }

    void WallController::SwitchWalls(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_, float boundary_)
    {
        if (m_CurrentlySwitching)
        {
            auto walls = entityManager_->GetAllEntitiesWithComponents<Game::WallComponent, Engine::TransformComponent>();

            bool switched = false;
            for (auto& wall : walls)
            {
                auto wallTransformer = wall->GetComponent<Engine::TransformComponent>();
                auto rightWallBoundary = wallTransformer->m_Position.x + wallTransformer->m_Size.x / 2;

                if (rightWallBoundary < boundary_)
                {
                    switched = true;
                }
            }

            if (switched)
            {
                auto floorTexture = textureManager_->GetLevelOrCommonTexture(level_->levelId, Game::TEX_WALL, "floor");
                auto ceilingTexture = textureManager_->GetLevelOrCommonTexture(level_->levelId, Game::TEX_WALL, "ceiling");

                m_CurrentlySwitching = false;
                for (auto& wall : walls)
                {
                    auto wallTransformer = wall->GetComponent<Engine::TransformComponent>();
                    wallTransformer->m_Position.x = boundary_ + wallTransformer->m_Size.x / 2;

                    auto wallSprite = wall->GetComponent<Engine::SpriteComponent>();
                    if (wall->HasComponent<Game::FloorComponent>())
                    {
                        wallSprite->m_Image = floorTexture;
                    }
                    else
                    {
                        wallSprite->m_Image = ceilingTexture;
                    }
                }
            }

        }
    }
}

