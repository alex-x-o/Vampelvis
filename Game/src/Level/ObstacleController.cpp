#include "precomp.h"
#include "ObstacleController.h"

#include "Entities/GameComponents.h"
#include "Core/GameConstants.h"
#include "Level.h"

#include <random>


namespace Game
{
    void ObstacleController::RemovePastObstacles(Engine::EntityManager* entityManager_, float boundary)
    {
        auto floorObstacles = entityManager_->GetAllEntitiesWithComponent<Game::FloorObstacleComponent>();

        for (auto& obstacle : floorObstacles)
        {
            auto obstacleTransformer = obstacle->GetComponent<Engine::TransformComponent>();
            float xPos = obstacleTransformer->m_Position.x;
            float xWidth = obstacleTransformer->m_Size.x;

            if (xWidth / 2 + xPos < boundary)
            {
                entityManager_->RemoveEntityById(obstacle->GetId());
            }
        }

        auto ceilingObstacles = entityManager_->GetAllEntitiesWithComponent<Game::CeilingObstacleComponent>();

        for (auto& obstacle : ceilingObstacles)
        {
            auto obstacleTransformer = obstacle->GetComponent<Engine::TransformComponent>();
            float xPos = obstacleTransformer->m_Position.x;
            float xWidth = obstacleTransformer->m_Size.x;

            if (xWidth / 2 + xPos < boundary)
            {
                entityManager_->RemoveEntityById(obstacle->GetId());
            }
        }

    }

    void ObstacleController::GenerateObstacles(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_, float boundary_)
    {

        if (boundary_ < level_->obstacleLastPosition + level_->obstacleMinDistance)
            return;

        float obstacleGap = GameConstants::PLAYER_HEIGHT * 2.5f;
        float obstacleWidth = 100.f;
        float windowHeight = GameConstants::SCREEN_HEIGHT;
        float availableSpace = windowHeight - 2 * GameConstants::WALL_HEIGHT;

        float minObstacleHeight = GameConstants::PLAYER_HEIGHT / 2;
        float maxObstacleHeight = availableSpace - minObstacleHeight - obstacleGap;

        std::random_device rd;
        std::uniform_real_distribution<float> heights(minObstacleHeight, maxObstacleHeight);


        float xObstacle = ceil(boundary_ + obstacleWidth / 2);
        float floorObstacleHeight = heights(rd);
        float floorObstaclePos = (windowHeight - floorObstacleHeight) / 2 - GameConstants::WALL_HEIGHT;
        Engine::Texture* floorTex = textureManager_->GetRandomTexture(level_->levelId, Game::TEX_FLOOR);
        CreateFloorObstacle(entityManager_, floorTex, xObstacle, floorObstaclePos, obstacleWidth, floorObstacleHeight);


        float ceilingObstacleHeight = windowHeight - 2 * GameConstants::WALL_HEIGHT - obstacleGap - floorObstacleHeight;
        float ceilgPos = -(windowHeight - ceilingObstacleHeight) / 2 + GameConstants::WALL_HEIGHT;
        Engine::Texture* ceilingTex = textureManager_->GetRandomTexture(level_->levelId, Game::TEX_CEILING);
        CreateCeilingObstacle(entityManager_, ceilingTex, xObstacle, ceilgPos, obstacleWidth, ceilingObstacleHeight);

        level_->obstacleLastPosition = xObstacle;
    }


    void ObstacleController::CreateFloorObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h)
    {
        auto floorObstacle = std::make_unique<Engine::Entity>();

        floorObstacle->AddComponent<Game::FloorObstacleComponent>();
        floorObstacle->AddComponent<Engine::TransformComponent>(x, y, w, h);
        floorObstacle->AddComponent<Engine::CollisionComponent>(w, h);
        floorObstacle->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(floorObstacle));
    }

    void ObstacleController::CreateCeilingObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h)
    {
        auto ceilingObstacle = std::make_unique<Engine::Entity>();

        ceilingObstacle->AddComponent<Game::CeilingObstacleComponent>();
        ceilingObstacle->AddComponent<Engine::TransformComponent>(x, y, w, h);
        ceilingObstacle->AddComponent<Engine::CollisionComponent>(w, h);
        ceilingObstacle->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(ceilingObstacle));
    }
}