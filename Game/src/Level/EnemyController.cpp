#include "precomp.h"
#include "EnemyController.h"

#include "Entities/GameComponents.h"
#include "Core/GameConstants.h"
#include <random>


namespace Game
{
    void EnemyController::GenerateEnemies(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary)
    {
        std::random_device rd;
        std::uniform_int_distribution<> EnemyChance(0, 1000);

        if (EnemyChance(rd) == 0) {
            Engine::Texture* t = textureManager_->GetTexture("blank");
            float width = GameConstants::PLAYER_WIDTH;
            float height = GameConstants::PLAYER_HEIGHT;
            auto enemy = std::make_unique<Engine::Entity>();
            float availableSpace = ceil((GameConstants::SCREEN_HEIGHT - GameConstants::WALL_HEIGHT - height / 2) / 2);

            std::uniform_int_distribution<> yPositions(-availableSpace, availableSpace);

            enemy->AddComponent<Game::GlidingEnemyComponent>();
            enemy->AddComponent<Engine::TransformComponent>(boundary + width, yPositions(rd), width, height);
            enemy->AddComponent<Engine::CollisionComponent>(width, height);
            enemy->AddComponent<Engine::SpriteComponent>().m_Image = t;
            enemy->AddComponent<Engine::MoverComponent>();

            entityManager_->AddEntity(std::move(enemy));
        }

    }

    void EnemyController::MoveEnemies(Engine::EntityManager* entityManager_, float speed_)
    {
        auto enemies = entityManager_->GetAllEntitiesWithComponents<Game::GlidingEnemyComponent, Engine::MoverComponent>();

        for (auto& enemy : enemies)
        {
            auto move = enemy->GetComponent<Engine::MoverComponent>();
            move->m_TranslationSpeed.x = -speed_;
        }
    }

    void EnemyController::RemovePastEnemies(Engine::EntityManager* entityManager_, float boundary)
    {

        auto glidingEnemies = entityManager_->GetAllEntitiesWithComponent<Game::GlidingEnemyComponent>();

        for (auto& enemy : glidingEnemies)
        {
            auto enemyTransformer = enemy->GetComponent<Engine::TransformComponent>();
            float xPos = enemyTransformer->m_Position.x;
            float xWidth = enemyTransformer->m_Size.x;

            if (xWidth / 2 + xPos < boundary)
            {
                entityManager_->RemoveEntityById(enemy->GetId());
            }
        }
    }

}