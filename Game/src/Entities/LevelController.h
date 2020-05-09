#pragma once
#include "ScrollingBackground.h"
#include "Level/ObstacleController.h"
#include "Level/EnemyController.h"
#include "Level/WallController.h"
#include "Level/PickupController.h"
#include "Level/Level.h"

namespace Game
{
    struct CameraBoundary;

    class LevelController
    {
    public:
        bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, int windowHeight_);
        void Update(float dt, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
        void UpdateSpeed(float speedCoef_);

    private:
        float m_BaseSpeed{ 100.0f };
        float m_CurrentSpeed{ 100.0f };

        CameraBoundary getCurrentBoundaries(Engine::EntityManager* entityManager_);

        void MoveLevelObjects(Engine::EntityManager* entityManager_);
        void RemovePastLevelObjects(Engine::EntityManager* entityManager_, float boundary_);

        

        std::unique_ptr<ScrollingBackground> m_Background{};
        std::unique_ptr<WallController> m_WallController{};
        std::unique_ptr<ObstacleController> m_ObstacleController{};
        std::unique_ptr<EnemyController> m_EnemyController{};
        std::unique_ptr<PickupController> m_PickupController{};

    };

}