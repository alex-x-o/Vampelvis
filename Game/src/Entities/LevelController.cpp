#include "precomp.h"
#include "LevelController.h"

#include "Entities/GameComponents.h"
#include <random>

namespace Game
{
    bool Level::Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to BallController::Init()");


        auto wall = std::make_unique<Engine::Entity>();


        // Down

        wall->AddComponent<WallComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, 285.f, 1300.f, 30.f);
        wall->AddComponent<Engine::CollisionComponent>(1300.f, 30.f);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = texture_;
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));

        // Up
        wall = std::make_unique<Engine::Entity>();

        wall->AddComponent<WallComponent>();
        wall->AddComponent<Engine::TransformComponent>(0.f, -285, 1300.f, 30.f);
        wall->AddComponent<Engine::CollisionComponent>(1300.f, 30.f);
        wall->AddComponent<Engine::SpriteComponent>().m_Image = texture_;
        wall->AddComponent<Engine::MoverComponent>();

        entityManager_->AddEntity(std::move(wall));

        return true;
    }

    void Level::Update(float dt, Engine::EntityManager* entityManager_, Engine::Texture* texture_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to Level::Update()");

        auto walls = entityManager_->GetAllEntitiesWithComponents<Game::WallComponent, Engine::MoverComponent>();

        for (auto& wall : walls)
        {
            auto move = wall->GetComponent<Engine::MoverComponent>();
            move->m_TranslationSpeed.x = 100.f;
        }

        
        if(this->m_ShouldGenerate)
        {
            this->RemovePastObstacles(entityManager_);
            this->GenerateObstacles(entityManager_,  texture_);
        }
    }

    void Level::RemovePastObstacles(Engine::EntityManager* entityManager_)
    {
        float boundary = 0;

        auto camera = entityManager_->GetAllEntitiesWithComponent<Engine::CameraComponent>()[0];
        auto cameraTransform = camera->GetComponent<Engine::TransformComponent>();
        boundary = cameraTransform->m_Position.x - cameraTransform->m_Size.x / 2;

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

    void Level::GenerateObstacles(Engine::EntityManager* entityManager_, Engine::Texture* texture_)
    {
        float boundary = 0;

        auto camera = entityManager_->GetAllEntitiesWithComponent<Engine::CameraComponent>()[0];
        auto cameraTransform = camera->GetComponent<Engine::TransformComponent>();
        boundary = cameraTransform->m_Position.x + cameraTransform->m_Size.x / 2;

        if (boundary < this->m_LastObstaclePos + this->m_MinObstacleDist)
            return;
        
        float playerHeight = 0;

        auto player = entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>()[0];
        auto playerTransform = player->GetComponent<Engine::TransformComponent>();
        playerHeight = playerTransform->m_Size.y;
        
        float obstacleGap = playerHeight * 2.5;

        //TODO - refactor wall height to be constant & add min obstacle height
        float wallHeight = 30.f;
        int windowHeight = 600;
        float availableSpace = windowHeight - 2 * wallHeight;

        float minObstacleHeight = playerHeight / 2;
        float maxObstacleHeight = availableSpace - minObstacleHeight - obstacleGap;

        std::random_device rd;
        std::uniform_real_distribution<> heights(minObstacleHeight, maxObstacleHeight);


        float xObstacle = boundary + 10;
        float floorObstacleHeight = heights(rd);
       

        auto floorObstacle = std::make_unique<Engine::Entity>();

        floorObstacle->AddComponent<Game::FloorObstacleComponent>();
        floorObstacle->AddComponent<Engine::TransformComponent>(xObstacle, (windowHeight - floorObstacleHeight)/2 - wallHeight, 30.f, floorObstacleHeight);
        floorObstacle->AddComponent<Engine::CollisionComponent>(30.f, floorObstacleHeight);
        floorObstacle->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(floorObstacle));


        float ceilingObstacleHeight = windowHeight - 2* wallHeight - obstacleGap - floorObstacleHeight;
        auto ceilingObstacle = std::make_unique<Engine::Entity>();

        ceilingObstacle->AddComponent<Game::CeilingObstacleComponent>();
        ceilingObstacle->AddComponent<Engine::TransformComponent>(xObstacle, -(windowHeight - ceilingObstacleHeight) / 2 + wallHeight, 30.f, ceilingObstacleHeight);
        ceilingObstacle->AddComponent<Engine::CollisionComponent>(30.f, ceilingObstacleHeight);
        ceilingObstacle->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        entityManager_->AddEntity(std::move(ceilingObstacle));



        this->m_LastObstaclePos = xObstacle;
    }
}