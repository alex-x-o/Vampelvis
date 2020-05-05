#pragma once

namespace Engine
{
    class EntityManager;
}

namespace Game
{

    class CameraController
    {
    public:
        bool Init(Engine::EntityManager* entityManager_);
        void Update(float dt, Engine::EntityManager* entityManager_);
        void UpdateSpeed(float speedCoef_);
    private:
        float m_BaseSpeed{ 100.0f };
        float m_CurrentSpeed{ 100.0f };
    };
}
