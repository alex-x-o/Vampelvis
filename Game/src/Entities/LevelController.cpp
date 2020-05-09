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

        m_ObstacleController = std::make_unique<Game::ObstacleController>();
        m_EnemyController = std::make_unique<Game::EnemyController>();
        m_PickupController = std::make_unique<Game::PickupController>();

        float backgroundHeigt = GameConstants::SCREEN_HEIGHT - 2 * GameConstants::WALL_HEIGHT;
        m_Background = std::make_unique <ScrollingBackground>();
        m_Background->Init(entityManager_, textureManager_->GetTexture("background"), GameConstants::SCREEN_WIDTH, backgroundHeigt);

        m_WallController = std::make_unique<Game::WallController>();
        m_WallController->Init(entityManager_, textureManager_);

        return true;
    }

    void LevelController::Update(float dt, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to LevelController::Update()");
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to texturemanager to LevelController::Update()");

        Game::CameraBoundary boundary = getCurrentBoundaries(entityManager_);

        m_EnemyController->GenerateEnemies(entityManager_, textureManager_, boundary.right);
        m_ObstacleController->GenerateObstacles(entityManager_, textureManager_, boundary.right);
        m_PickupController->GeneratePickups(entityManager_, textureManager_, boundary.right, 300.f, m_ObstacleController->m_LastObstaclePos);

        MoveLevelObjects(entityManager_);
        
        m_Background->Update(dt, entityManager_, boundary.left);
        RemovePastLevelObjects(entityManager_, boundary.left);
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

    CameraBoundary LevelController::getCurrentBoundaries(Engine::EntityManager* entityManager_)
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