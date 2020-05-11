#include "precomp.h"
#include "LevelController.h"

#include "GameComponents.h"
#include <random>
#include "Core/GameConstants.h"
#include "Helper/CameraBoundry.h"

namespace Game
{

    bool LevelController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, int windowHeight_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to LevelController::Init()");
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to texturemanager to LevelController::Init()");

        InitLevels();
        Level* currentLevel = GetCurrentLevel();

        m_ObstacleController = std::make_unique<Game::ObstacleController>();
        m_EnemyController = std::make_unique<Game::EnemyController>();
        m_PickupController = std::make_unique<Game::PickupController>();

        float backgroundHeigt = GameConstants::SCREEN_HEIGHT - 2 * GameConstants::WALL_HEIGHT;
        m_Background = std::make_unique <ScrollingBackground>();
        m_Background->Init(entityManager_, textureManager_->GetLevelOrCommonTexture(currentLevel->levelId, TEX_BACKGROUND, "background"), GameConstants::SCREEN_WIDTH, backgroundHeigt);

        m_WallController = std::make_unique<Game::WallController>();
        m_WallController->Init(entityManager_, textureManager_, currentLevel);

        return true;
    }

    void LevelController::Update(float dt, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to LevelController::Update()");
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to texturemanager to LevelController::Update()");

        Game::CameraBoundary boundary = GetCurrentBoundaries(entityManager_);
        Level* currentLevel = GetCurrentLevel();


        if (currentLevel->levelStart + currentLevel->levelLength < floor(boundary.right))
        {
            ChangeLevel(entityManager_, textureManager_, boundary.right);
        }
        else
        {
            m_ObstacleController->GenerateObstacles(entityManager_, textureManager_, currentLevel, boundary.right);
            m_EnemyController->GenerateEnemies(entityManager_, textureManager_, currentLevel, boundary.right);
            m_PickupController->GeneratePickups(entityManager_, textureManager_, currentLevel, boundary.right);

            m_WallController->SwitchWalls(entityManager_, textureManager_, currentLevel, boundary.left);
        }

        m_Background->Update(entityManager_, boundary.left);

        MoveLevelObjects(entityManager_);
        RemovePastLevelObjects(entityManager_, boundary.left);
    }

    void LevelController::InitLevels()
    {
        auto level = std::make_unique<Game::Level>(LVL_CASTLE);
        level->spawnBats = false;
        level->levelLength = 2400.f;
        m_Levels.push_back(std::move(level));

        level = std::make_unique<Game::Level>(LVL_CAVE);
        level->spawnGhosts = false;
        level->levelLength = 2400.f;
        m_Levels.push_back(std::move(level));
    }

    Level* LevelController::GetCurrentLevel()
    {
        Level* level = m_Levels.at(m_CurrentLevelIndex).get();
        return level;
    }

    void LevelController::RemovePastLevelObjects(Engine::EntityManager* entityManager_, float boundary_)
    {
        m_EnemyController->RemovePastEnemies(entityManager_, boundary_);
        m_ObstacleController->RemovePastObstacles(entityManager_, boundary_);
        m_PickupController->RemovePastPickups(entityManager_, boundary_);
    }

    void LevelController::MoveLevelObjects(Engine::EntityManager* entityManager_)
    {
        m_WallController->MoveWalls(entityManager_, m_CurrentSpeed);
        m_EnemyController->MoveEnemies(entityManager_, m_BaseSpeed);
    }

    void LevelController::UpdateSpeed(float speedCoef_)
    {
        m_CurrentSpeed = m_BaseSpeed * speedCoef_;
    }

    void LevelController::ChangeLevel(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary_)
    {
        auto currentLevel = GetCurrentLevel();
        m_CurrentLevelIndex ? m_CurrentLevelIndex = 0 : m_CurrentLevelIndex = 1;
        auto newLevel = GetCurrentLevel();

        newLevel->levelStart = currentLevel->levelStart + currentLevel->levelLength;
        newLevel->enemyLastPosition = currentLevel->enemyLastPosition;
        newLevel->pickupLastPosition = currentLevel->pickupLastPosition;
        newLevel->obstacleLastPosition = currentLevel->obstacleLastPosition;
        
        auto newBackground = textureManager_->GetLevelOrCommonTexture(newLevel->levelId, TEX_BACKGROUND, "background");
        m_Background->ChangeAtX(entityManager_, newBackground, newLevel->levelStart);

        m_WallController->StartSwitchingWalls(entityManager_, textureManager_, newLevel);

        m_ObstacleController->GenerateLevelBoundary(entityManager_, textureManager_, newLevel);
    }

    CameraBoundary LevelController::GetCurrentBoundaries(Engine::EntityManager* entityManager_)
    {
        auto boundary = CameraBoundary();

        auto camera = entityManager_->GetAllEntitiesWithComponent<Engine::CameraComponent>()[0];
        auto cameraTransform = camera->GetComponent<Engine::TransformComponent>();
        float posX = cameraTransform->m_Position.x;

        boundary.right = posX + cameraTransform->m_Size.x / 2;
        boundary.left = posX - cameraTransform->m_Size.x / 2;

        return boundary;
    }

}