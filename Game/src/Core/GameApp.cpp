#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"
#include "Entities/PlayerController.h"
#include "Entities/LevelController.h"
#include "Entities/StaticImage.h"

#include <nlohmann/json.hpp>

void Game::GameApp::GameSpecificWindowData()
{
    Engine::WindowData gameSpecificWindowData;
    gameSpecificWindowData.m_Title = "Vampelvis";
    gameSpecificWindowData.m_Width = GameApp::WindowWidth;
    gameSpecificWindowData.m_Height = GameApp::WindowHeight;
    gameSpecificWindowData.m_Vsync = true;
    SetWindowData(gameSpecificWindowData);
    
    std::ifstream input("score.json");
    if (input.good()) return;

    std::ofstream output("score.json");
    nlohmann::json j;
    j["score"] = 0;
    output << j;
}

bool Game::GameApp::GameSpecificInit()
{
    this->LoadGameTextures();

    m_CameraController = std::make_unique<CameraController>();
    m_CameraController->Init(m_EntityManager.get());

    m_Level = std::make_unique<LevelController>();
    m_Level->Init(m_EntityManager.get(), m_TextureManager.get(), GameApp::WindowHeight);

    m_PlayerController = std::make_unique<PlayerController>();
    bool playerStatus = m_PlayerController->Init(m_EntityManager.get(), m_TextureManager->GetTexture("vampire"));
    ASSERT(playerStatus, "Player initialization failed in GameApp::GameSpecificInit()");

    return true;
}

void Game::GameApp::GameSpecificUpdate(float dt_)
{
    this->ChangetGameSpeed();

    bool playerHit = !m_PlayerController->Update(m_EntityManager.get());
    if (playerHit && !m_GodMode)
    {
        m_ShowMenu = m_GameOver = true;
        UpdateHighScore();

        return;
    }
    
    m_CameraController->Update(dt_, m_EntityManager.get());
    m_Level->Update(dt_, m_EntityManager.get(), m_TextureManager.get());
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}

void Game::GameApp::LoadGameTextures()
{
    //Common
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 0, "vampire", "./Textures/vampireSprites.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 0, "immortality", "./Textures/testTube.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 0, "batMode", "./Textures/testTube2.png");

    // Level 1 Backgrounds
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "blank", "./Textures/blank.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "woodenTexture", "./Textures/woodenTexture.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "woodenTexture1", "./Textures/woodenTexture1.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "woodenTextureLong", "./Textures/woodenTextureLong.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "background", "./Textures/background.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "floor", "./Textures/floorLong.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "ceiling", "./Textures/ceilingLong.png");

    // Level 1 Obstacles
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "chair", "./Textures/chair.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "table", "./Textures/table.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "chandelier", "./Textures/chandelier.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "chandelier2", "./Textures/chandelier2.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "chandelier3", "./Textures/chandelier3.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 1, "chanSil", "./Textures/chanSil.png");

    // Level 2 Backgrounds
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 2, "backCave", "./Textures/backCave.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 2, "floorCave", "./Textures/ceilingCave.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 2, "ceilingCave", "./Textures/floorCave.png");

    // Level 2 Obstacles
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 2, "stalactite", "./Textures/stalactite.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 2, "stalactite2", "./Textures/stalactite2.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 2, "stalactite3", "./Textures/stalactite3.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 2, "stalagmite", "./Textures/stalagmite.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), 2, "stalagmite2", "./Textures/stalagmite2.png");

    //m_TextureManager->WriteTextures();
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
    int score = static_cast<int>(ceil(m_PlayerController->GetPlayerPositionX() - m_PlayerController->GetPlayerStartingPositionX())) / 80;
    return score;
}

void Game::GameApp::UpdateHighScore()
{
    std::ifstream input("score.json");
    if (!input.is_open())
    {
        LOG_ERROR("Opening file with high score failed");
        return;
    }

    nlohmann::json scoreObject;
    input >> scoreObject;

    int newScore = GetScore();
    if (newScore > scoreObject["score"])
    {
        scoreObject["score"] = newScore;

        std::ofstream output("score.json");
        output << scoreObject;
    }
}
