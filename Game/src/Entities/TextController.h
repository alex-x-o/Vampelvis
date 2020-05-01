#pragma once

namespace Engine
{
    class EntityManager;
    class TextureManager;
    struct Texture;
}

namespace Game
{
    class TextController
    {
    public:
        void Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_);
        bool Update(Engine::EntityManager* entityManager_, float playerPosition_);
    };

}