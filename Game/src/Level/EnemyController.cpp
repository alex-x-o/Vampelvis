#include "precomp.h"
#include "EnemyController.h"

#include "Entities/GameComponents.h"
#include "Core/GameConstants.h"
#include "Level.h"

#include <random>


namespace Game
{
    void EnemyController::GenerateEnemies(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_, float boundary_)
    {
        if (boundary_ < level_->enemyLastPosition + level_->enemyMinDistance)
            return;

        float enemyPosX = ceil(level_->obstacleLastPosition + level_->obstacleMinDistance / 2);
        level_->enemyLastPosition = enemyPosX;

        std::random_device rd;
        std::uniform_int_distribution<> EnemyChance(1, level_->enemySpawnChance);

        if (EnemyChance(rd) == 1) {
            
            bool spawnGhost = false;
            bool spawnBat = false;


            if (level_->spawnGhosts && level_->spawnBats)
            {
                std::uniform_int_distribution<> flipCoin(0, 1);
                flipCoin(rd) ? spawnGhost = true : spawnBat = true;
            }
            else if (level_->spawnGhosts)
            {
                spawnGhost = true;
            }
            else if (level_->spawnBats)
            {
                spawnBat = true;
            }
            else
            {
                return;
            }


            Engine::Texture* texture;
            if (spawnGhost)
            {
                texture = textureManager_->GetCommonTexture(Game::TEX_ENEMY, "ghost");
                float width = GameConstants::PLAYER_WIDTH;
                float height = GameConstants::PLAYER_HEIGHT;
                
                float availableSpace = ceil(GameConstants::SCREEN_HEIGHT / 2 - GameConstants::WALL_HEIGHT - height / 2);
                std::uniform_int_distribution<> yPositions(-availableSpace, availableSpace);
                float yPos = yPositions(rd);

                CreateGhost(entityManager_, texture, enemyPosX, yPos, width, height);
            }

            if (spawnBat)
            {
                texture = textureManager_->GetCommonTexture(Game::TEX_ENEMY, "bat");
                float width = GameConstants::BAT_WIDTH;
                float height = GameConstants::BAT_HEIGHT;

                float availableSpace = ceil(GameConstants::SCREEN_HEIGHT / 2 - GameConstants::WALL_HEIGHT - height / 2);
                std::uniform_int_distribution<> yPositions(-availableSpace, availableSpace);
                float yPos = yPositions(rd);

                CreateGhost(entityManager_, texture, enemyPosX, yPos, width, height);
            }
        }

    }

    void EnemyController::MoveEnemies(Engine::EntityManager* entityManager_, float speed_)
    {
        auto enemies = entityManager_->GetAllEntitiesWithComponents<Game::GlidingEnemyComponent, Engine::MoverComponent>();

        for (auto& enemy : enemies)
        {
            auto move = enemy->GetComponent<Engine::MoverComponent>();
            move->m_TranslationSpeed.x = -speed_;

            auto sprite = enemy->GetComponent<Engine::SpriteComponent>();
            AnimateEnemy(sprite);
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

    void EnemyController::AnimateEnemy(Engine::SpriteComponent* sprite_)
    {
        auto frameCurrent = sprite_->m_AnimationCurrentFrame;
        auto frameNum = sprite_->m_AnimationFrames;

        if (frameCurrent >= frameNum)
        {
            sprite_->m_AnimationCurrentFrame = 0;
        }
        else
        {
            sprite_->m_AnimationCurrentFrame = sprite_->m_AnimationCurrentFrame + 1;
        }
    }

    void EnemyController::CreateGhost(Engine::EntityManager* entityManager_, Engine::Texture* texture_,
                                      float x_, float y_, float w_, float h_)
    {

        auto enemy = std::make_unique<Engine::Entity>();
        enemy->AddComponent<Game::GlidingEnemyComponent>();
        enemy->AddComponent<Engine::TransformComponent>(x_, y_, w_, h_);
        enemy->AddComponent<Engine::CollisionComponent>(0.8f*w_, 0.8f * h_);
        enemy->AddComponent<Engine::SpriteComponent>().m_Image = texture_;
        enemy->AddComponent<Engine::MoverComponent>();

        auto spriteComp = enemy->GetComponent<Engine::SpriteComponent>();
        spriteComp->m_Animation = true;
        spriteComp->m_AnimationFrames = 2;
        spriteComp->m_AnimationCurrentFrame = 0;
        spriteComp->m_Height = 48;

        entityManager_->AddEntity(std::move(enemy));
    }

    void EnemyController::CreateBat(Engine::EntityManager* entityManager_, Engine::Texture* texture_,
                                    float x_, float y_, float w_, float h_)
    {

        auto enemy = std::make_unique<Engine::Entity>();
        enemy->AddComponent<Game::GlidingEnemyComponent>();
        enemy->AddComponent<Engine::TransformComponent>(x_, y_, w_, h_);
        enemy->AddComponent<Engine::CollisionComponent>(w_);
        enemy->AddComponent<Engine::SpriteComponent>().m_Image = texture_;
        enemy->AddComponent<Engine::MoverComponent>();

        auto spriteComp = enemy->GetComponent<Engine::SpriteComponent>();
        spriteComp->m_Animation = true;
        spriteComp->m_AnimationFrames = 2;
        spriteComp->m_AnimationCurrentFrame = 0;
        spriteComp->m_Height = 48;

        entityManager_->AddEntity(std::move(enemy));
    }
}