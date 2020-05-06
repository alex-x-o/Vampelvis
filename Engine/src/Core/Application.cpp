#include "precomp.h"
#include "Application.h"
#include "ECS/EntityManager.h"
#include "Input/InputManager.h"
#include "Render/RenderSystem.h"
#include "Render/Renderer.h"
#include "Render/Texture.h"
#include "Render/Window.h"
#include "Render/WindowData.h"
#include "Render/TextureManager.h"
#include "Physics/PhysicsSystem.h"

#include <SDL.h>

namespace Engine {

    Application::~Application() = default;
    WindowData Application::m_WindowData{};

    bool Application::Init()
    {
        LOG_INFO("Initializing application");

        GameSpecificWindowData();

        // Render system initialize
        m_RenderSystem = std::make_unique<RenderSystem>();
        if (!m_RenderSystem->Init(m_WindowData))
        {
            LOG_CRITICAL("Failed to initialize RenderSystem");
            return false;
        }

        // Main Menu initialize
        m_MainMenu = std::make_unique<MainMenu>();
        if (!m_MainMenu->Init())
        {
            LOG_CRITICAL("Failed to initialize MainMenu");
            return false;
        }

        // Texture Manager create
        m_TextureManager = std::make_unique<TextureManager>();

        // Input Manager initialize
        m_InputManager = std::make_unique<InputManager>();
        if (!m_InputManager->Init())
        {
            LOG_CRITICAL("Failed to initialize InputManager");
            return false;
        }

        // Entity Manager initialize
        m_EntityManager = std::make_unique<EntityManager>();
        if (!m_EntityManager->Init())
        {
            LOG_CRITICAL("Failed to initialize EntityManager");
            return false;
        }

        // Physics system initialize
        m_PhysicsSystem = std::make_unique<PhysicsSystem>();
        if (!m_PhysicsSystem->Init())
        {
            LOG_CRITICAL("Failed to initialize PhysicsSystem");
            return false;
        }

        if (GameSpecificInit() != true)
        {
            LOG_CRITICAL("Error initializing game specific systems!");
            return false;
        }

        return true;
    }

    bool Application::Shutdown()
    {
        LOG_INFO("Shutting down application");

        GameSpecificShutdown();

        m_RenderSystem->Shutdown();
        m_RenderSystem.reset();

        m_MainMenu->Shutdown();
        m_MainMenu.reset();

        return true;
    }

    bool Application::Run()
    {
        m_Running = true;
        auto previousFrameTime = SDL_GetPerformanceCounter();

        // Main loop
        SDL_Event event{ };
        while (m_Running)
        {
            while (SDL_PollEvent(&event) != 0)
            {
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE || event.type == SDL_QUIT)
                {
                    m_Running = false;
                }
                else if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE: 
                    {
                        if (!m_ShowMenu) m_ShowMenu = !m_ShowMenu;
                        break;
                    }
                    case SDLK_SPACE:
                    {
                        if (m_ShowMenu) m_ShowMenu = !m_ShowMenu;
                        break;
                    }
                    case SDLK_UP:
                    {
                        m_MainMenu->GoUp();
                        break;
                    }
                    case SDLK_DOWN:
                    {
                        m_MainMenu->GoDown();
                        break;
                    }
                    }
                }
            }

            auto frameTime = SDL_GetPerformanceCounter();

            float deltaTime = (frameTime - previousFrameTime) / static_cast<float>(SDL_GetPerformanceFrequency());

            //LOG_INFO("Current FPS: {}", 1.f / deltaTime);

            if (!m_ShowMenu && m_GameOver) return true;

            m_ShowMenu ? m_MainMenu->Update(m_RenderSystem->GetRenderer()) : Update(deltaTime);

            previousFrameTime = frameTime;
        }

        return false;
    }

    void Application::Update(float dt)
    {
        if (m_MainMenu->IsVisible()) m_MainMenu->HideMenu(m_RenderSystem->GetRenderer());

        // Update all systems
        m_InputManager->Update(dt, m_EntityManager.get());
        m_PhysicsSystem->Update(dt, m_EntityManager.get());
        m_EntityManager->Update(dt);
        m_RenderSystem->Update(dt, m_EntityManager.get());

        GameSpecificUpdate(dt);
    }

}
