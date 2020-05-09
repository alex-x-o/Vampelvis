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
        bool Update(Engine::EntityManager* entityManager_);
        void UpdateSpeed(float speedCoef_);

        float GetPlayerPositionX() const;
        float GetPlayerStartingPositionX() const;

        const static float Height;
        const static float Width;

    private:
        float m_PlayerPositionX{};
        float m_PlayerStartingPositionX{ -300.f };

        float m_BaseSpeed{ 100.0f };
        float m_CurrentSpeed{ 100.0f };

        void InitInventory(Engine::InventoryComponent* inventory_);
        void PickUpPowerups(Engine::CollisionComponent* collision_, Engine::InventoryComponent* inventory_);
        void PickUpItem(Engine::InventoryComponent* inventory_, Engine::PickupComponent* item_);
        bool UseItem(Engine::InventoryComponent* inventory_, int id_);
        void CastPowerups(Engine::PowerupComponent* activePowerups, Engine::InputComponent* input, Engine::InventoryComponent* inventory, float playerPositionX_);
        bool isActivePowerup(Engine::PowerupComponent* powerups, Game::Powerup power);
        void RemoveExpiredPowerups(Engine::PowerupComponent* powerups_, float playerPositionX_);
    };
}