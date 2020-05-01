#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"
#include "Entities/PlayerController.h"
#include "Entities/TextController.h"
#include "Entities/LevelController.h"
#include "Entities/StaticImage.h"

#include <Engine.h>
#include <Core/EntryPoint.h>

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
    bool playerStatus = m_PlayerController->Init(m_EntityManager.get(), m_TextureManager->GetTexture("blank"));
    ASSERT(playerStatus, "Player initialization failed in GameApp::GameSpecificInit()");

    m_Level = std::make_unique<Level>();
    m_Level->Init(m_EntityManager.get(), m_TextureManager->GetTexture("blank"));

    m_TextController = std::make_unique<TextController>();
    m_TextController->Init(m_EntityManager.get(), m_TextureManager->GetTexture("blank"));

    return true;
}

bool Game::GameApp::GameSpecificUpdate(float dt_)
{
    if (gameOver && !m_GodMode)
    {
        // If space is pressed start new game
        bool spacePressed = m_TextController->Update(m_EntityManager.get(), m_PlayerController->GetPlayerPositionX());
        if (spacePressed) return false;
    }
    else
    {
        // If player hits something, shut down game
        gameOver = !m_PlayerController->Update(m_EntityManager.get());
    }

    m_CameraController->Update(dt_, m_EntityManager.get());
    m_Level->Update(dt_, m_EntityManager.get(), m_TextureManager->GetTexture("blank"));

    return true;
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}