#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"
#include "Entities/PlayerController.h"
#include "Entities/LevelController.h"
#include "Entities/StaticImage.h"

#include <Engine.h>
#include <Core/EntryPoint.h>

#include <SDL.h>

void Game::GameApp::GameSpecificWindowData()
{
    Engine::WindowData gameSpecificWindowData;
    gameSpecificWindowData.m_Title = "Vampelvis";
    gameSpecificWindowData.m_Width = 800;
    gameSpecificWindowData.m_Height = 600;
    gameSpecificWindowData.m_Vsync = true;
    SetWindowData(gameSpecificWindowData);
}

bool Game::GameApp::GameSpecificInit()
{
    m_RenderSystem->SetBackgroundColor(0, 0, 0, 1);

    m_CameraController = std::make_unique<CameraController>();
    m_CameraController->Init(m_EntityManager.get());

    // TestImage
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "blank", "blank.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "hello", "hello_world.jpg");
    m_TestImage = std::make_unique<StaticImage>();
    m_TestImage->Init(m_EntityManager.get(), m_TextureManager->GetTexture("hello"));

    m_PlayerController = std::make_unique<PlayerController>();
    m_PlayerController->Init(m_EntityManager.get(), m_TextureManager->GetTexture("blank"));

    m_Level = std::make_unique<Level>();
    m_Level->Init(m_EntityManager.get(), m_TextureManager->GetTexture("blank"));

    return true;
}

bool GameOver(Engine::EntityManager* entityManager_)
{
    LOG_INFO("GAME OVER. Game blocked until space is pressed");

    auto entityToMove = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent, Engine::InputComponent>();
    ASSERT(entityToMove.size() == 1, "Must be only one entity with PlayerComponent and InputComponent in GameOver()");

    auto entity = entityToMove.front();
    auto input = entity->GetComponent<Engine::InputComponent>();

    return Engine::InputManager::IsActionActive(input, fmt::format("Player{}Jump", 1));
}

bool Game::GameApp::GameSpecificUpdate(float dt)
{
    // If player hits something, shut down game
    bool playerCollided = m_PlayerController->Update(dt, m_EntityManager.get());
    if (!m_GodMode && playerCollided)
    {
        if (GameOver(m_EntityManager.get()))
        {
            return false;
        }
    }
        
    m_CameraController->Update(dt, m_EntityManager.get());
    m_Level->Update(dt, m_EntityManager.get(), m_TextureManager->GetTexture("blank"));

    return true;
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}