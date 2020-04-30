#pragma once

namespace Game
{
    class Level
    {
    public:
        bool m_ShouldGenerate{ true };

        bool Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_);
        void Update(float dt, Engine::EntityManager* entityManager_, Engine::Texture* texture_);

    private:
        float m_LastObstaclePos{ 0 };
        float m_MinObstacleDist{ 200 };

        void RemovePastObstacles(Engine::EntityManager* entityManager_);
        void GenerateObstacles(Engine::EntityManager* entityManager_, Engine::Texture* texture_);
    };

}