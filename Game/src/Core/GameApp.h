#pragma once

#include <Core\Application.h>

namespace Game
{
    class CameraController;
    class PlayerController;
    class LevelController;
    class StaticImage;

    class GameApp final : public Engine::Application
    {
    public:
        const static int WindowWidth{ 800 };
        const static int WindowHeight{ 600 };
    private:
        bool m_GodMode{ false };

        void GameSpecificWindowData() override;
        bool GameSpecificInit() override;
        void GameSpecificUpdate(float dt_) override;
        bool GameSpecificShutdown() override;
        void LoadGameTextures();
        void ChangetGameSpeed();

        int GetScore();
        void UpdateHighScore();

        std::unique_ptr<PlayerController> m_PlayerController{};
        std::unique_ptr<CameraController> m_CameraController{};
        std::unique_ptr<LevelController> m_Level{};
        std::unique_ptr<StaticImage> m_TestImage{};
    };
}

Engine::Application* Engine::CreateApplication()
{
    return new Game::GameApp();
}
