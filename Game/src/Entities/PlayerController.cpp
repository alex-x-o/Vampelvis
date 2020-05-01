#include "precomp.h"
#include "PlayerController.h"

namespace Game
{
    bool PlayerController::Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to PlayerController::Init()");
        ASSERT(texture_ != nullptr, "Must pass valid pointer to texture to PlayerController::Init()");

        auto player = std::make_unique<Engine::Entity>();

        player->AddComponent<Engine::TransformComponent>(-300.f, 0.f, 30.f, 100.f);
        player->AddComponent<Engine::CollisionComponent>(30.f, 100.f);
        player->AddComponent<Engine::PlayerComponent>();
        player->AddComponent<Engine::InputComponent>();
        player->AddComponent<Engine::MoverComponent>();
        player->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        auto inputComp = player->GetComponent<Engine::InputComponent>();
        inputComp->inputActions.push_back({ "MainGameBtn" });

        entityManager_->AddEntity(std::move(player));

        return !(entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>().empty());
    }

    bool PlayerController::Update(Engine::EntityManager* entityManager_)
    {
        auto entityToMove = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent, Engine::MoverComponent, Engine::InputComponent, Engine::TransformComponent>();
        ASSERT(entityToMove.size() == 1, "Must be only one entity with Player, Mover, Transform and Input Component in PlayerController::Update()");

        auto entity = entityToMove.front();
        auto move = entity->GetComponent<Engine::MoverComponent>();
        auto position = entity->GetComponent<Engine::TransformComponent>();
        auto input = entity->GetComponent<Engine::InputComponent>();
        auto speed = entity->GetComponent<Engine::PlayerComponent>()->m_PanSpeed;

        // Move player
        bool jumpInput = Engine::InputManager::IsActionActive(input, "MainGameBtn");
            
        move->m_TranslationSpeed.x = 100.f;
        move->m_TranslationSpeed.y = speed * (jumpInput ? -0.5f : 0.2f);

        // Check position
        m_PlayerPositionX = position->m_Position.x;

        // Check if hit
        if (entity->GetComponent<Engine::CollisionComponent>()->m_CollidedWith.size() > 0)
        {
            LOG_INFO("Player hit something in PlayerController::Update");

            move->m_TranslationSpeed.x = 0.f;
            move->m_TranslationSpeed.y = 0.f;

            return false;
        }

        return true;
    }

    float PlayerController::GetPlayerPositionX() const
    {
        return m_PlayerPositionX;
    }
}
