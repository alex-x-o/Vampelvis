#pragma once

namespace Game
{
    class Level
    {
    public:
        bool m_ShouldGenerate{ true };

        bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
        void Update(float dt, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
        void UpdateSpeed(float speedCoef_);

    private:
        float m_LastObstaclePos{ 0 };
        float m_MinObstacleDist{ 200 };

        float m_BaseSpeed{ 100.0f };
        float m_CurrentSpeed{ 100.0f };

        void RemovePastObstacles(Engine::EntityManager* entityManager_);
        void GenerateObstacles(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
    };

}