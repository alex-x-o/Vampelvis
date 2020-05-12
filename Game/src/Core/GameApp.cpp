#include "precomp.h"

#include "GameApp.h"
#include "GameConstants.h"
#include "MainMenu/HighScoreData.h"
#include "MainMenu/GameMenu.h"
#include "MainMenu/MenuLabelsData.h"
#include "Entities/CameraController.h"
#include "Entities/PlayerController.h"
#include "Entities/LevelController.h"
#include "Entities/StaticImage.h"
#include "MainMenu/MenuLabelsData.h"

#include <nlohmann/json.hpp>
#include <ctime>


void Game::GameApp::GameSpecificWindowData()
{
    Engine::WindowData gameSpecificWindowData;
    gameSpecificWindowData.m_Title = GameConstants::GAME_TITLE;
    gameSpecificWindowData.m_Width = GameApp::WindowWidth;
    gameSpecificWindowData.m_Height = GameApp::WindowHeight;
    gameSpecificWindowData.m_Vsync = true;
    gameSpecificWindowData.m_Icon = GameConstants::GAME_ICON;
    SetWindowData(gameSpecificWindowData);
}

bool Game::GameApp::GameSpecificInit()
{
    // Main Menu initialize
    m_MainMenu = std::make_unique<Game::GameMenu>();
    if (!m_MainMenu->Init(m_AudioManager.get()))
    {
        LOG_CRITICAL("Failed to initialize MainMenu");
        return false;
    }

    this->LoadGameTextures();

    m_CameraController = std::make_unique<CameraController>();
    m_CameraController->Init(m_EntityManager.get());

    m_Level = std::make_unique<LevelController>();
    m_Level->Init(m_EntityManager.get(), m_TextureManager.get(), GameApp::WindowHeight);

    m_PlayerController = std::make_unique<PlayerController>();
    bool playerStatus = m_PlayerController->Init(m_EntityManager.get(), m_TextureManager->GetCommonTexture(TEX_PLAYER, "vampire"));
    ASSERT(playerStatus, "Player initialization failed in GameApp::GameSpecificInit()");

    if (TTF_Init() == -1)
    {
        LOG_CRITICAL("Unable to initialize SDL_ttf in MenuItemsManager::Init()");
    }

    m_ScoreFont = TTF_OpenFont("./Data/menuFont.ttf", 30);

    if (!m_ScoreFont)
    {
        LOG_ERROR("Failed to initialize Menu Fonts: ");
        LOG_ERROR(TTF_GetError());

        return false;
    }

    srand(time(NULL));

    return true;
}

void Game::GameApp::GameSpecificUpdate(float dt_)
{
    this->ChangetGameSpeed();
    DrawPlayerScore();
    DrawPlayerInventory();

    float r = rand() / static_cast<float>(RAND_MAX);

    if (m_Level->GetCurrentLevelIndex() == 0)
    {
        if (r < 0.002f) m_AudioManager->PlaySound("GhostSound");
    }
    else if (m_Level->GetCurrentLevelIndex() == 1)
    {
        if (r < 0.002f) m_AudioManager->PlaySound("BatWings");
    }

    bool playerHit = !m_PlayerController->Update(m_EntityManager.get(), m_TextureManager.get());
    if (playerHit && !m_GodMode)
    {
        m_ShowMenu = m_GameOver = true;

        m_AudioManager->PlaySound("GameOver");
        m_AudioManager->StopSound("GhostSound");
        m_AudioManager->StopSound("BatWings");

        m_MainMenu->GameOver(GetPlayerScore());
        UpdateHighScore();

        return;
    }
    
    m_CameraController->Update(dt_, m_EntityManager.get());
    m_Level->Update(dt_, m_EntityManager.get(), m_TextureManager.get());
}

void Game::GameApp::GameSpecificShutdown() 
{
    m_MainMenu->Shutdown();
    m_MainMenu.reset();
}

void Game::GameApp::LoadGameTextures()
{
    //Common
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), TEX_PLAYER, "vampire", "./Textures/vampireSprites.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), TEX_PLAYER, "bat", "./Textures/batSprites.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), TEX_PLAYER, "smoke", "./Textures/smoke.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), TEX_PICKUP, "immortality", "./Textures/testTubeBlue.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), TEX_PICKUP, "batMode", "./Textures/testTubeRed.png");

    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), TEX_ENEMY, "ghost", "./Textures/ghostSprites.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), TEX_ENEMY, "bat", "./Textures/batSprites.png");

    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), TEX_FLOOR, "boundary", "./Textures/stone.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), TEX_CEILING, "boundary", "./Textures/stone.png");


    // Level 1 Backgrounds
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CASTLE, TEX_BACKGROUND, "background", "./Textures/background.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CASTLE, TEX_WALL, "floor", "./Textures/floorLong.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CASTLE, TEX_WALL, "ceiling", "./Textures/ceilingLong.png");

    // Level 1 Obstacles
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CASTLE, TEX_FLOOR, "chair", "./Textures/chair.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CASTLE, TEX_FLOOR, "table", "./Textures/table.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CASTLE, TEX_FLOOR, "chanSil", "./Textures/chanSil.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CASTLE, TEX_CEILING, "chandelier", "./Textures/chandelier.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CASTLE, TEX_CEILING, "chandelier2", "./Textures/chandelier2.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CASTLE, TEX_CEILING, "chandelier3", "./Textures/chandelier3.png");
    

    // Level 2 Backgrounds
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CAVE, TEX_BACKGROUND, "background", "./Textures/backCave.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CAVE, TEX_WALL, "ceiling", "./Textures/ceilingCave.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CAVE, TEX_WALL, "floor", "./Textures/floorCave.png");

    // Level 2 Obstacles
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CAVE, TEX_CEILING, "stalactite", "./Textures/stalactite.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CAVE, TEX_CEILING, "stalactite2", "./Textures/stalactite2.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CAVE, TEX_CEILING, "stalactite3", "./Textures/stalactite3.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CAVE, TEX_FLOOR, "stalagmite", "./Textures/stalagmite.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), LVL_CAVE, TEX_FLOOR, "stalagmite2", "./Textures/stalagmite2.png");

}

void Game::GameApp::ChangetGameSpeed()
{
    float coef = 1.0f + static_cast<float>(GetPlayerScore()) / 100.0f;
    if (coef <= GameConstants::MAX_SPEED_COEF)
    {
        m_CameraController->UpdateSpeed(coef);
        m_PlayerController->UpdateSpeed(coef);
        m_Level->UpdateSpeed(coef);
    }

}

void Game::GameApp::DrawPlayerScore()
{
    m_RenderSystem->GetRenderer()->DrawTextOnGameScreen(fmt::format("Score: {}", GetPlayerScore()), 30, -3, m_ScoreFont);
}

void Game::GameApp::DrawPlayerInventory()
{
    int i = 0;
    for (auto& inv : GetPlayerInventory())
    {
        m_RenderSystem->GetRenderer()->DrawTextOnGameScreen(std::to_string(inv.second), 750 - i*90, -3, m_ScoreFont);

        std::string fileName = inv.first == Game::Powerup::BatMode ? "./Textures/testTubeRed.png" : "./Textures/testTubeBlue.png";
        
        // Count how much of powerup left, and if it is less than 0.2 blink text
        bool active = false;
        auto& activePowers = GetActivePowers();
        if (activePowers.find(inv.first) != std::end(activePowers))
        {
            float left = (activePowers.at(inv.first) - m_PlayerController->GetPlayerPositionX());
            left /= inv.first == Game::Powerup::BatMode ? GameConstants::BATMODE_DURATION : GameConstants::IMMORTALITY_DURATION;

            active = left < 0.2? true : false;
        }
        
        m_RenderSystem->GetRenderer()->DrawBlinkingTextureOnGameScreen(active, fileName, 800 - (i + 1) * 90, -3, 30, 30);

        ++i;
    }
}

const std::unordered_map<int, int>& Game::GameApp::GetPlayerInventory()
{
    auto player = m_EntityManager.get()->GetAllEntitiesWithComponents<Engine::PlayerComponent, Engine::InventoryComponent>();
    ASSERT(player.size() == 1, "Must be only one Player");

    return player.front()->GetComponent<Engine::InventoryComponent>()->m_Inventory;
}

const std::unordered_map<int, float>& Game::GameApp::GetActivePowers()
{
    auto player = m_EntityManager.get()->GetAllEntitiesWithComponents<Engine::PlayerComponent, Engine::PowerupComponent>();
    ASSERT(player.size() == 1, "Must be only one Player");

    return player.front()->GetComponent<Engine::PowerupComponent>()->m_ActivePowers;
}

int Game::GameApp::GetPlayerScore()
{
    int score = static_cast<int>(ceil(m_PlayerController->GetPlayerPositionX() - m_PlayerController->GetPlayerStartingPositionX())) / 80;
    return score;
}

void Game::GameApp::UpdateHighScore()
{
    int newScore = GetPlayerScore();
    if (newScore > scoreData->GetHighScore())
    {
        scoreData->SetHighScore(newScore);
    }
}
