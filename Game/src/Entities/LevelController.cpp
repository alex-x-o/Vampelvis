#include "precomp.h"
#include "LevelController.h"

#include "GameComponents.h"
#include <random>
#include "PlayerController.h"
#include "ScrollingBackground.h"

namespace Game
{
    const float Level::WallHeight = 30.f;

    bool Level::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, int windowHeight_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to Level::Init()");
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to texturemanager to Level::Init()");

        m_WindowHeight = static_cast<float>(windowHeight_);
        
        m_Background = std::make_unique <ScrollingBackground>();
        m_Background->Init(entityManager_, textureManager_->GetTexture("background"), 800, m_WindowHeight-2* WallHeight);
        CreateWalls(entityManager_, textureManager_);

        return true;
    }

    void Level::Update(float dt, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to Level::Update()");
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to texturemanager to Level::Update()");

        MoveWalls(entityManager_);
        

        Game::CameraBoundary boundary = getCurrentBoundaries(entityManager_);
        m_Background->Update(dt, entityManager_, boundary.left);
        if (m_ShouldGenerate)
        { 
            GenerateObstacles(entityManager_, textureManager_, boundary.right);
        }

        RemovePastObstacles(entityManager_, boundary.left);
    }

    void Level::RemovePastObstacles(Engine::EntityManager* entityManager_, float boundary)
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

    void Level::GenerateObstacles(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary)
    {

        if (boundary < this->m_LastObstaclePos + this->m_MinObstacleDist)
            return;

        float obstacleGap = PlayerController::Height * 2.5f;
        float obstacleWidth = 100.f;
        float windowHeight = m_WindowHeight;
        float availableSpace = windowHeight - 2 * Level::WallHeight;

        float minObstacleHeight = PlayerController::Height / 2;
        float maxObstacleHeight = availableSpace - minObstacleHeight - obstacleGap;

        std::random_device rd;
        std::uniform_real_distribution<float> heights(minObstacleHeight, maxObstacleHeight);


        float xObstacle = boundary + obstacleWidth/2;
        float floorObstacleHeight = heights(rd);
        float floorObstaclePos = (windowHeight - floorObstacleHeight) / 2 - Level::WallHeight;
        Engine::Texture* floorTex = textureManager_->GetTexture("chandelier");
        CreateFloorObstacle(entityManager_, floorTex, xObstacle, floorObstaclePos, obstacleWidth, floorObstacleHeight);


        float ceilingObstacleHeight = windowHeight - 2 * Level::WallHeight - obstacleGap - floorObstacleHeight;
        float ceilgPos = -(windowHeight - ceilingObstacleHeight) / 2 + Level::WallHeight;
        Engine::Texture* ceilingTex = textureManager_->GetTexture("chandelier");
        CreateCeilingObstacle(entityManager_, ceilingTex, xObstacle, ceilgPos, obstacleWidth, ceilingObstacleHeight);

        this->m_LastObstaclePos = xObstacle;
    }

    void Level::GenerateGlidingEnemies(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary)
    {

    }

    CameraBoundary Level::getCurrentBoundaries(Engine::EntityManager* entityManager_)
    {
        auto boundary = CameraBoundary();

        auto camera = entityManager_->GetAllEntitiesWithComponent<Engine::CameraComponent>()[0];
        auto cameraTransform = camera->GetComponent<Engine::TransformComponent>();
        float posX = cameraTransform->m_Position.x;

        boundary.right = posX + cameraTransform->m_Size.x / 2;
        boundary.left = posX - cameraTransform->m_Size.x / 2;

        return boundary;
    }

    void Level::CreateWalls(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        float wallPosition = (m_WindowHeight - Level::WallHeight) / 2;
        auto wall = std::make_unique<Engine::Entity>();

        // Down
        wall->AddComponent<WallComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, wallPosition, 1300.f, Level::WallHeight);
        wall->AddComponent<Engine::CollisionComponent>(1300.f, Level::WallHeight);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("floor");
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));

        // Up
        wall = std::make_unique<Engine::Entity>();

        wall->AddComponent<WallComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, -wallPosition, 1300.f, Level::WallHeight);
        wall->AddComponent<Engine::CollisionComponent>(1300.f, Level::WallHeight);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("ceiling");
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));
    }

    void Level::MoveWalls(Engine::EntityManager* entityManager_)
    {
        auto walls = entityManager_->GetAllEntitiesWithComponents<Game::WallComponent, Engine::MoverComponent>();

        for (auto& wall : walls)
        {
            auto move = wall->GetComponent<Engine::MoverComponent>();
            move->m_TranslationSpeed.x = m_CurrentSpeed;
        }
    }

    void Level::UpdateSpeed(float speedCoef_)
    {
        m_CurrentSpeed = m_BaseSpeed * speedCoef_;
    }

    void Level::CreateFloorObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h)
    {
        auto floorObstacle = std::make_unique<Engine::Entity>();

        floorObstacle->AddComponent<Game::FloorObstacleComponent>();
        floorObstacle->AddComponent<Engine::TransformComponent>(x, y, w, h);
        floorObstacle->AddComponent<Engine::CollisionComponent>(w, h);
        floorObstacle->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(floorObstacle));
    }

    void Level::CreateCeilingObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h)
    {
        auto ceilingObstacle = std::make_unique<Engine::Entity>();

        ceilingObstacle->AddComponent<Game::CeilingObstacleComponent>();
        ceilingObstacle->AddComponent<Engine::TransformComponent>(x, y, w, h);
        ceilingObstacle->AddComponent<Engine::CollisionComponent>(w, h);
        ceilingObstacle->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(ceilingObstacle));
    }
}