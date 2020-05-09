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

        static WindowData m_WindowData;

    protected:
        bool m_Running{ false };
        bool m_ShowMenu{ true };
        bool m_GameOver{ false };

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
        virtual void GameSpecificShutdown() = 0;
        virtual void GameSpecificUpdate(float dt) = 0;
        virtual int GetPlayerScore() = 0;

        void Update(float dt);
        void ProcessInput(SDL_Keycode key_);

        Application(const Application& other) = delete;
        Application& operator=(Application& other) = delete;
    };

    // To be defined in the game
    Application* CreateApplication();
}
