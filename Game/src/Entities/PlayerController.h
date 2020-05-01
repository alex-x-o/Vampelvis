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
        float GetPlayerPositionX() const;

    private:
        float m_PlayerPositionX{};
    };
}