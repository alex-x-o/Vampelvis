#pragma once

namespace Engine
{
    class EntityManager;
    struct Texture;
}

namespace Game
{

    class ScrollingBackground
    {
    public:
        bool Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float w, float h);
		void Update(Engine::EntityManager* entityManager_, float boundary);

        void ChangeAtX(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x_);

    private:
        bool m_Synced{ true };
    };
}