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
#include "Audio/AudioManager.h"

#include <SDL.h>

namespace Engine {

    Application::~Application() = default;

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

        // Initialize Audio manager
        m_AudioManager = std::make_unique<AudioManager>();
        if (!m_AudioManager->Init())
        {
            LOG_CRITICAL("Failed to initialize AudioManager");
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

        m_AudioManager->Shutdown();
        m_AudioManager.reset();

        m_RenderSystem->Shutdown();
        m_RenderSystem.reset();

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
                    if (event.key.keysym.sym == m_MainMenu->m_MenuInput["PauseGame"] && !m_ShowMenu)
                    {
                        m_ShowMenu = true;
                    }
                    else if (m_ShowMenu)
                    {
                        m_ShowMenu = m_MainMenu->ProcessInput(event.key.keysym.sym);
                    }
                }
            }

            auto frameTime = SDL_GetPerformanceCounter();

            float deltaTime = (frameTime - previousFrameTime) / static_cast<float>(SDL_GetPerformanceFrequency());

            //LOG_INFO("Current FPS: {}", 1.f / deltaTime);

            // Restart game
            if (m_GameOver && !m_ShowMenu) return true;

            if (m_ShowMenu)
            {
                m_RenderSystem->GetRenderer()->HideWindow();
                m_MainMenu->ShowMenu();
                m_MainMenu->Update();
            }
            else
            {
                m_RenderSystem->GetRenderer()->ShowWindow();
                m_MainMenu->HideMenu();
                Update(deltaTime);
            }

            previousFrameTime = frameTime;
        }

        return false;
    }

    void Application::Update(float dt)
    {
        // Update all systems
        m_InputManager->Update(dt, m_EntityManager.get());
        m_PhysicsSystem->Update(dt, m_EntityManager.get());
        m_EntityManager->Update(dt);
        m_RenderSystem->Update(dt, m_EntityManager.get());

        GameSpecificUpdate(dt);
    }
}