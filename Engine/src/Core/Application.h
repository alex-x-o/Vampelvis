#pragma once

#include "Render/WindowData.h"
#include "MainMenu/MainMenu.h"
#include <SDL.h>

namespace Engine {

    class RenderSystem;
    class PhysicsSystem;
    class InputManager;
    class EntityManager;
    class TextureManager;

    class Application
    {
    public:
        bool Init();
        bool Run();
        bool Shutdown();

        virtual ~Application();
        Application() = default;

        static WindowData GetWindowData() { return m_WindowData; }

    protected:
        void SetWindowData(const WindowData& windowData_) { m_WindowData = windowData_; }

        bool m_Running{ false };
        bool m_GameOver{ false };
        bool m_ShowMenu{ true };

        static WindowData m_WindowData;
        std::unique_ptr<RenderSystem> m_RenderSystem{};
        std::unique_ptr<MainMenu> m_MainMenu{};
        std::unique_ptr<PhysicsSystem> m_PhysicsSystem{};
        std::unique_ptr<InputManager> m_InputManager{};
        std::unique_ptr<EntityManager> m_EntityManager{};
        std::unique_ptr<TextureManager> m_TextureManager{};

    private:
        // To be overridden by the game
        virtual void GameSpecificWindowData() = 0;
        virtual bool GameSpecificInit() = 0;
        virtual bool GameSpecificShutdown() = 0;
        virtual void GameSpecificUpdate(float dt) = 0;

        void Update(float dt);

        Application(const Application& other) = delete;
        Application& operator=(Application& other) = delete;
    };

    // To be defined in the game
    Application* CreateApplication();
}
