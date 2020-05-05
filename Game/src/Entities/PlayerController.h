#pragma once

namespace Engine
{
    class EntityManager;
    class TextureManager;
}

namespace Game
{
    class PlayerController
    {
    public:
        bool Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_);
        bool Update(Engine::EntityManager* entityManager_);
        void UpdateSpeed(float speedCoef_);

        float GetPlayerPositionX() const;
        float GetPlayerStartingPositionX() const;

    private:
        float m_PlayerPositionX{};
        float m_PlayerStartingPositionX{ -300.f };

        float m_BaseSpeed{ 100.0f };
        float m_CurrentSpeed{ 100.0f };
    };
}