#include "precomp.h"
#include "TextController.h"

void Game::TextController::Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_)
{
    ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager in TextController::Init()");
    ASSERT(texture_ != nullptr, "Must pass valid pointer to texture in TextController::Init()");

    auto text = std::make_unique<Engine::Entity>();

    text->AddComponent<Engine::TransformComponent>(-1000.f, 0.f, 200.f, 100.f);
    text->AddComponent<Engine::InputComponent>();
    text->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

    auto inputComp = text->GetComponent<Engine::InputComponent>();
    inputComp->inputActions.push_back({ "MainGameBtn" });

    entityManager_->AddEntity(std::move(text));
}

bool Game::TextController::Update(Engine::EntityManager* entityManager_)
{
    LOG_INFO("GAME OVER. Game blocked until space is pressed");

    auto inputEntities = entityManager_->GetAllEntitiesWithComponents<Engine::TransformComponent, Engine::InputComponent, Engine::SpriteComponent>();
    for (auto& entity : inputEntities)
    {
        if (entity->HasComponent<Engine::PlayerComponent>()) continue;

        // Check if space is pressed
        auto input = entity->GetComponent<Engine::InputComponent>();
        return Engine::InputManager::IsActionActive(input, "MainGameBtn");
    }
}