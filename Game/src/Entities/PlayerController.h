#pragma once
#include "Core/GameConstants.h"

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
        bool Update(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
        void UpdateSpeed(float speedCoef_);

        float GetPlayerPositionX() const;
        float GetPlayerStartingPositionX() const;

        bool CheckIfCheated();

        const static float Height;
        const static float Width;

    private:
        float m_PlayerPositionX{};
        float m_PlayerStartingPositionX{ -300.f };

        float m_BaseSpeed{ 100.0f };
        float m_CurrentSpeed{ 100.0f };

        bool m_ReadyToShapeshift = false;
        bool m_HasCheated = false;

        void InitInventory(Engine::InventoryComponent* inventory_);
        void PickUpPowerups(Engine::CollisionComponent* collision_, Engine::InventoryComponent* inventory_);
        void PickUpItem(Engine::InventoryComponent* inventory_, Engine::PickupComponent* item_);
        bool UseItem(Engine::InventoryComponent* inventory_, int id_);
        void CastPowerups(Engine::PowerupComponent* activePowerups, Engine::InputComponent* input, Engine::InventoryComponent* inventory, float playerPositionX_);
        bool isActivePowerup(Engine::PowerupComponent* powerups, Game::Powerup power);
        void RemoveExpiredPowerups(Engine::PowerupComponent* powerups_, float playerPositionX_);
        void Shapeshift(Engine::TextureManager* textureManager_, Engine::CollisionComponent* collision_, Engine::TransformComponent* transformer_, Engine::SpriteComponent* sprite_);
        void KeepPlayerOnScreen(Engine::CollisionComponent* collisionComponent_, Engine::TransformComponent* transform_);
        bool CheckIfCollided(Engine::CollisionComponent* collisionComponent_, Engine::PowerupComponent* powerups_);
        void ToggleGodMode(Engine::InputComponent* input_, Engine::PowerupComponent* powerups_);
        void AnimatePlayer(Engine::SpriteComponent* sprite_);
    };
}