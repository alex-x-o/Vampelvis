#pragma once

#include <Core\Application.h>

namespace Game
{
    class CameraController;
    class PlayerController;
    class Level;
    class StaticImage;
    class TextController;

    class GameApp final : public Engine::Application
    {
    private:
        bool m_GodMode{ false };
        bool gameOver{ false };

        void GameSpecificWindowData() override;
        bool GameSpecificInit() override;
        bool GameSpecificUpdate(float dt_) override;
        bool GameSpecificShutdown() override;

        std::unique_ptr<PlayerController> m_PlayerController{};
        std::unique_ptr<CameraController> m_CameraController{};
        std::unique_ptr<Level> m_Level{};
        std::unique_ptr<StaticImage> m_TestImage{};
        std::unique_ptr<TextController> m_TextController{};
    };
}

Engine::Application* Engine::CreateApplication()
{
    return new Game::GameApp();
}
