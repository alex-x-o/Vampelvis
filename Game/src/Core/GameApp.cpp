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
    this->LoadGameTextures();

    m_RenderSystem->SetBackgroundTexture(m_TextureManager->GetTexture("woodenTexture"));

    m_CameraController = std::make_unique<CameraController>();
    m_CameraController->Init(m_EntityManager.get());

    m_PlayerController = std::make_unique<PlayerController>();
    bool playerStatus = m_PlayerController->Init(m_EntityManager.get(), m_TextureManager->GetTexture("vampire"));
    ASSERT(playerStatus, "Player initialization failed in GameApp::GameSpecificInit()");

    m_Level = std::make_unique<Level>();
    m_Level->Init(m_EntityManager.get(), m_TextureManager.get());

    m_TextController = std::make_unique<TextController>();
    m_TextController->Init(m_EntityManager.get(), m_TextureManager->GetTexture("woodenTexture1"));

    return true;
}

bool Game::GameApp::GameSpecificUpdate(float dt_)
{
    this->ChangetGameSpeed();
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
    m_Level->Update(dt_, m_EntityManager.get(), m_TextureManager.get());

    return true;
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}

void Game::GameApp::LoadGameTextures()
{
    //Player
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "vampire", "./Textures/vampireSprites.png");

    // Backgrounds
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "blank", "./Textures/blank.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "woodenTexture", "./Textures/woodenTexture.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "woodenTexture1", "./Textures/woodenTexture1.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "woodenTextureLong", "./Textures/woodenTextureLong.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "background", "./Textures/background.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "floor", "./Textures/floorLong.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "ceiling", "./Textures/ceilingLong.png");

    // Level 1 Obstacles
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "chair", "./Textures/chair.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "table", "./Textures/table.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "chandelier", "./Textures/chandelier.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "chandelier2", "./Textures/chandelier2.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "chandelier3", "./Textures/chandelier3.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "chanSil", "./Textures/chanSil.png");

}

void Game::GameApp::ChangetGameSpeed()
{
    float coef = 1.0f + static_cast<float>(GetScore()) / 100.0f;
    m_CameraController->UpdateSpeed(coef);
    m_PlayerController->UpdateSpeed(coef);
    m_Level->UpdateSpeed(coef);
}

int Game::GameApp::GetScore()
{
    int score = ceil(m_PlayerController->GetPlayerPositionX() - m_PlayerController->GetPlayerStartingPositionX()) / 80;
    return score;
}
