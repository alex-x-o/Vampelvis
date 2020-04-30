#pragma once

#include <Core\Application.h>

namespace Game
{
    class CameraController;
    class PlayerController;
    class Level;
    class StaticImage;

    class GameApp final : public Engine::Application
    {
    private:
        bool m_GodMode{ false };

        void GameSpecificWindowData() override;
        bool GameSpecificInit() override;
        bool GameSpecificUpdate(float dt) override;
        bool GameSpecificShutdown() override;

        std::unique_ptr<PlayerController> m_PlayerController{};
        std::unique_ptr<CameraController> m_CameraController{};
        std::unique_ptr<Level> m_Level{};
        std::unique_ptr<StaticImage> m_TestImage{};
    };

}

Engine::Application* Engine::CreateApplication()
{
    return new Game::GameApp();
}
