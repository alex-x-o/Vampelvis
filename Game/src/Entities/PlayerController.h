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
        bool Update(float dt, Engine::EntityManager* entityManager_);

    private:
        bool gameOver{ false };
    };
}