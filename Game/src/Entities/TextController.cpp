#include "precomp.h"
#include "TextController.h"

void Game::TextController::Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_)
{
    ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager in TextController::Init()");
    ASSERT(texture_ != nullptr, "Must pass valid pointer to texture in TextController::Init()");

    auto text = std::make_unique<Engine::Entity>();

    text->AddComponent<Engine::TransformComponent>(-1000.f, 0.f, 200.f, 100.f);
    text->AddComponent<Engine::InputComponent>();
    text->AddComponent<Engine::MoverComponent>();
    text->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

    auto inputComp = text->GetComponent<Engine::InputComponent>();
    inputComp->inputActions.push_back({ "MainGameBtn" });

    entityManager_->AddEntity(std::move(text));
}

bool Game::TextController::Update(Engine::EntityManager* entityManager_, float playerPosition_)
{
    LOG_INFO("GAME OVER. Game blocked until space is pressed");

    auto inputEntities = entityManager_->GetAllEntitiesWithComponents<Engine::InputComponent, Engine::SpriteComponent, Engine::MoverComponent, Engine::TransformComponent>();
    LOG_INFO(inputEntities.size());
    for (auto& entity : inputEntities)
    {
        if (entity->HasComponent<Engine::PlayerComponent>()) continue;

        // Show text on screen
        auto position = entity->GetComponent<Engine::TransformComponent>();
        LOG_INFO(position->m_Position.x);
        if (abs(position->m_Position.x + 1000.f) < 0.001) position->m_Position.x = playerPosition_+300.f;

        // Move text with camera
        auto move = entity->GetComponent<Engine::MoverComponent>();
        move->m_TranslationSpeed.x = 100.f;

        // Check if space is pressed
        auto input = entity->GetComponent<Engine::InputComponent>();
        return Engine::InputManager::IsActionActive(input, "MainGameBtn");
    }
}