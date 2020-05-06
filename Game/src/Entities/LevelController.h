#pragma once
#include "ScrollingBackground.h"

namespace Game
{
    struct CameraBoundary {
        float left;
        float right;
    };

    class Level
    {
    public:
        const static float WallHeight;
        bool m_ShouldGenerate{ true };

        bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, int windowHeight_);
        void Update(float dt, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
        void UpdateSpeed(float speedCoef_);

    private:
        float m_LastObstaclePos{ 0 };
        float m_MinObstacleDist{ 300 };
        float m_WindowHeight{};

        float m_BaseSpeed{ 100.0f };
        float m_CurrentSpeed{ 100.0f };
        void MoveLevelObjects(Engine::EntityManager* entityManager_);

        CameraBoundary getCurrentBoundaries(Engine::EntityManager* entityManager_);
        void GenerateObstacles(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary);
        void GenerateGlidingEnemies(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary);
        
        void CreateWalls(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
        void CreateFloorObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h);
        void CreateCeilingObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h);
        
        void RemovePastObstacles(Engine::EntityManager* entityManager_, float boundary);

        std::unique_ptr<ScrollingBackground> m_Background{};
    };

}