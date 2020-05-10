#include "precomp.h"
#include "PlayerController.h"
#include "GameComponents.h"
#include "Core/GameConstants.h"


namespace Game
{
    const float PlayerController::Height = 100.f;
    const float PlayerController::Width = 80.f;

    bool PlayerController::Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to PlayerController::Init()");
        ASSERT(texture_ != nullptr, "Must pass valid pointer to texture to PlayerController::Init()");

        auto player = std::make_unique<Engine::Entity>();

        player->AddComponent<Engine::TransformComponent>(this->m_PlayerStartingPositionX, 0.f, Width, Height);
        player->AddComponent<Engine::CollisionComponent>(Width, Height);
        player->AddComponent<Engine::PlayerComponent>();
        player->AddComponent<Engine::InputComponent>();
        player->AddComponent<Engine::MoverComponent>();
        player->AddComponent<Engine::InventoryComponent>();
        player->AddComponent<Engine::PowerupComponent>();
        player->AddComponent<Engine::SpriteComponent>().m_Image = texture_;
        
        auto spriteComp = player->GetComponent<Engine::SpriteComponent>();
        spriteComp->m_Animation = true;
        spriteComp->m_AnimationFrames = 9;
        spriteComp->m_AnimationCurrentFrame = 0;
        spriteComp->m_Height = 64;

        auto inputComp = player->GetComponent<Engine::InputComponent>();
        inputComp->inputActions.push_back({ "MainGameBtn" });
        inputComp->inputActions.push_back({ "Player1BatMode" });
        inputComp->inputActions.push_back({ "Player1Immortality" });
        inputComp->inputActions.push_back({ "ToggleImmortality" });

        auto inventory = player->GetComponent<Engine::InventoryComponent>();
        InitInventory(inventory);

        entityManager_->AddEntity(std::move(player));

        return !(entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>().empty());
    }

    bool PlayerController::Update(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        auto entityToMove = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent, Engine::MoverComponent, Engine::InputComponent, Engine::TransformComponent>();
        ASSERT(entityToMove.size() == 1, "Must be only one entity with Player, Mover, Input and Transform Component in PlayerController::Update()");

        auto entity = entityToMove.front();
        auto move = entity->GetComponent<Engine::MoverComponent>();
        auto position = entity->GetComponent<Engine::TransformComponent>();
        auto input = entity->GetComponent<Engine::InputComponent>();
        auto speed = entity->GetComponent<Engine::PlayerComponent>()->m_PanSpeed;
        auto sprite = entity->GetComponent<Engine::SpriteComponent>();
        auto inventory = entity->GetComponent<Engine::InventoryComponent>();
        auto powerups = entity->GetComponent<Engine::PowerupComponent>();
        auto collision = entity->GetComponent<Engine::CollisionComponent>();

        // Animate player
        auto frameCurrent = sprite->m_AnimationCurrentFrame;
        auto frameNum = sprite->m_AnimationFrames;

        if (frameCurrent < frameNum - 1)
        {
            sprite->m_AnimationCurrentFrame = sprite->m_AnimationCurrentFrame + 1;
        }
        else
        {
            sprite->m_AnimationCurrentFrame = 0;
        }

        // Move player
        bool jumpInput = Engine::InputManager::IsActionActive(input, "MainGameBtn");
        move->m_TranslationSpeed.x = m_CurrentSpeed;
        move->m_TranslationSpeed.y = speed * (jumpInput ? -0.5f : 0.2f);

        // Update position
        m_PlayerPositionX = position->m_Position.x;
        ToggleGodMode(input, powerups);
        PickUpPowerups(collision, inventory);
        RemoveExpiredPowerups(powerups, m_PlayerPositionX);
        Shapeshift(textureManager_, collision, position, sprite);

        CastPowerups(powerups, input, inventory, m_PlayerPositionX);
        Shapeshift(textureManager_, collision, position, sprite);
        
        KeepPlayerOnScreen(collision, position);

        bool shouldContinueGame = !CheckIfCollided(collision, powerups);
        return shouldContinueGame;
    }

    float PlayerController::GetPlayerPositionX() const
    {
        return m_PlayerPositionX;
    }

    float PlayerController::GetPlayerStartingPositionX() const
    {
        return m_PlayerStartingPositionX;
    }

    void PlayerController::UpdateSpeed(float speedCoef_)
    {
        m_CurrentSpeed = m_BaseSpeed * speedCoef_;
    }

    void PlayerController::InitInventory(Engine::InventoryComponent* inventory_)
    {
        inventory_->m_Inventory[Game::Immortality] = 1;
        inventory_->m_Inventory[Game::BatMode] = 1;
    }

    void PlayerController::PickUpPowerups(Engine::CollisionComponent* collision_, Engine::InventoryComponent* inventory_)
    {
        auto collisions = collision_->m_CollidedWith;
        for (auto& collision : collisions)
        {
            if (collision->HasComponent<Engine::PickupComponent>())
            {
                PickUpItem(inventory_, collision->GetComponent<Engine::PickupComponent>());
                
                collision_->m_CollidedWith.erase(collision);
            }
        }
    }

    void PlayerController::PickUpItem(Engine::InventoryComponent* inventory_, Engine::PickupComponent* item_)
    {
        int id = item_->m_Type;
        auto finder = inventory_->m_Inventory.find(id);
        if (finder != inventory_->m_Inventory.end())
        {
            finder->second++;
        }
        else
        {
            inventory_->m_Inventory[id] = 1;
        }

        item_->m_PickedUp = true;
    }

    bool PlayerController::UseItem(Engine::InventoryComponent* inventory_, int id_)
    {
        bool used = false;

        auto finder = inventory_->m_Inventory.find(id_);
        if (finder != inventory_->m_Inventory.end())
        {
            if (finder->second > 0)
            {
                finder->second--;
                used = true;
            }

        }
        else
        {
            LOG_WARNING("Trying to use DropItem on unknown item (id: {}).", id_);
        }

        return used;
    }


    void PlayerController::CastPowerups(Engine::PowerupComponent* activePowerups, Engine::InputComponent* input,
                                       Engine::InventoryComponent* inventory, float playerPositionX_)
    {
        if (Engine::InputManager::IsActionActive(input, "Player1Immortality"))
        {
            bool powerUsed = false;

            if (!isActivePowerup(activePowerups, Game::Immortality))
            {
                powerUsed = UseItem(inventory, Game::Immortality);
            }

            if (powerUsed)
            {
                activePowerups->m_ActivePowers[Game::Immortality] = playerPositionX_ + GameConstants::IMMORTALITY_DURATION;
            }
        }

        if (Engine::InputManager::IsActionActive(input, "Player1BatMode"))
        {
            bool powerUsed = false;
            if (!isActivePowerup(activePowerups, Game::BatMode))
            {
                powerUsed = UseItem(inventory, Game::BatMode);
            }

            if (powerUsed)
            {
                activePowerups->m_ActivePowers[Game::BatMode] = playerPositionX_ + GameConstants::BATMODE_DURATION;
                m_ReadyToShapeshift = true;
            }
        }

    }

    bool PlayerController::isActivePowerup(Engine::PowerupComponent* powerups, Game::Powerup power)
    {
        ASSERT(powerups != nullptr, "Must pass valid pointer to PowerupComponent to PlayerController::isActivePowerup()");
        bool isActive = false;

        auto finder = powerups->m_ActivePowers.find(power);
        
        if (finder != powerups->m_ActivePowers.end())
        {
            isActive = true;
        }
        return isActive;
    }

    void PlayerController::RemoveExpiredPowerups(Engine::PowerupComponent* powerups_, float playerPositionX_)
    {
        auto activePowers = powerups_->m_ActivePowers;
        for (auto& [power, expiry] : activePowers)
        {
            if (expiry < playerPositionX_)
            {
                powerups_->m_ActivePowers.erase(power);
                if (power == Game::BatMode)
                    m_ReadyToShapeshift = true;
            }
        }
    }

    void PlayerController::Shapeshift(Engine::TextureManager* textureManager_,  Engine::CollisionComponent* collision_,
                                      Engine::TransformComponent* transformer_, Engine::SpriteComponent* sprite_)
    {
        if (!m_ReadyToShapeshift)
        {
            return;
        }

        if (transformer_->m_Size.x == GameConstants::BAT_WIDTH)
        {
            transformer_->m_Size = vec2(GameConstants::PLAYER_WIDTH, GameConstants::PLAYER_HEIGHT);
            collision_->m_Size = vec2(GameConstants::PLAYER_WIDTH, GameConstants::PLAYER_HEIGHT);
            sprite_->m_Image = textureManager_->GetCommonTexture(Game::TEX_PLAYER, "vampire");
        }
        else
        {
            transformer_->m_Size = vec2(GameConstants::BAT_WIDTH, GameConstants::BAT_HEIGHT);
            collision_->m_Size = vec2(GameConstants::BAT_WIDTH, GameConstants::BAT_HEIGHT);
            sprite_->m_Image = textureManager_->GetCommonTexture(Game::TEX_PLAYER, "bat");
        }


        m_ReadyToShapeshift = false;
    }

    void PlayerController::KeepPlayerOnScreen(Engine::CollisionComponent* collisionComponent_, Engine::TransformComponent* transform)
    {
        float boundary = (GameConstants::SCREEN_HEIGHT - transform->m_Size.y) / 2 - GameConstants::WALL_HEIGHT;
        transform->m_Position.y = transform->m_Position.y < -boundary ? -boundary : transform->m_Position.y;
        transform->m_Position.y = transform->m_Position.y > boundary ? boundary : transform->m_Position.y;
    }

    bool PlayerController::CheckIfCollided(Engine::CollisionComponent* collisionComponent_, Engine::PowerupComponent* powerups_)
                                           
    {
        bool immortal = powerups_->m_ActivePowers.find(Game::Immortality) != powerups_->m_ActivePowers.end();

        if (collisionComponent_->m_CollidedWith.size() > 0 && !immortal)
        {
            LOG_INFO("Player hit something in PlayerController::Update");

            return true;
        }

        return false;
    }

    void PlayerController::ToggleGodMode(Engine::InputComponent* input_, Engine::PowerupComponent* powerups_)
    {
        if (Engine::InputManager::IsActionActive(input_, "ToggleImmortality"))
        {
            m_HasCheated = true;
            auto& activePowerups = powerups_->m_ActivePowers;
            auto finder = activePowerups.find(Game::Immortality);

            if (finder != activePowerups.end())
            {
                if (finder->second == std::numeric_limits<float>::infinity())
                {
                    activePowerups.erase(Game::Immortality);
                }
                else
                {
                    activePowerups[Game::Immortality] = std::numeric_limits<float>::infinity();
                }
            }
            else
            {
                activePowerups[Game::Immortality] = std::numeric_limits<float>::infinity();
            }
        }
    }

    bool PlayerController::CheckIfCheated()
    {
        return m_HasCheated;
    }
}
