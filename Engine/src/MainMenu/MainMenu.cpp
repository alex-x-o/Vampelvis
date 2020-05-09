#include "precomp.h"
#include "MainMenu.h"
#include "MenuItemsManager.h"
#include "Core/Application.h"
#include "Render/Renderer.h"

#include <SDL.h>
#include <SDL_image.h>


namespace Engine
{
    bool MainMenu::Init()
    {
        LOG_INFO("Initializing Main Menu");

        int createWindow = SDL_CreateWindowAndRenderer(Application::m_WindowData.m_Width, Application::m_WindowData.m_Height, 0, &m_MenuWindow, &m_MenuRenderer);
        
        SDL_SetWindowTitle(m_MenuWindow, Application::m_WindowData.m_Title.c_str());
        setIcon();

        m_MenuItemsManager = std::make_unique<MenuItemsManager>();
        m_MenuItemsManager->Init(m_MenuRenderer);

        return !createWindow;
    }

    void MainMenu::setIcon() const
    {
        SDL_Surface* iconSurface = IMG_Load(Application::m_WindowData.m_Icon.c_str());

        // If icon file doesn't exist just continue
        if (!iconSurface)
        {
            LOG_INFO("Error with Menu Window Icon");
            return;
        }

        SDL_SetWindowIcon(m_MenuWindow, iconSurface);
        SDL_FreeSurface(iconSurface);
    }

    void MainMenu::Shutdown()
    {
        m_MenuItemsManager->Shutdown();

        LOG_INFO("Shutting down Menu Renderer");
        if (m_MenuRenderer != nullptr)
        {
            SDL_DestroyRenderer(m_MenuRenderer);
            m_MenuRenderer = nullptr;
        }

        LOG_INFO("Shutting down Menu Window");
        if (m_MenuWindow != nullptr)
        {
            SDL_DestroyWindow(m_MenuWindow);
            m_MenuWindow = nullptr;
        }
    }

    void MainMenu::Update(Renderer* gameRenderer_)
    {
        if (!m_Visible) ShowMenu(gameRenderer_);

        SDL_RenderClear(m_MenuRenderer);

        setBackground();
        m_MenuItemsManager->Update();

        SDL_RenderPresent(m_MenuRenderer);
    }

    void MainMenu::setBackground() const
    {
        SDL_Texture* background = IMG_LoadTexture(m_MenuRenderer, "./Textures/backCave.png");

        if (!background)
        {
            LOG_INFO("Error with setting Main Menu background");
            return;
        }

        SDL_RenderCopy(m_MenuRenderer, background, NULL, NULL);

        SDL_DestroyTexture(background);
    }

    void MainMenu::ShowMenu(Renderer* gameRenderer_)
    {
        gameRenderer_->HideWindow();
        SDL_ShowWindow(m_MenuWindow);
        SDL_RaiseWindow(m_MenuWindow);

        m_Visible = true;
    }

    void MainMenu::HideMenu(Renderer* gameRenderer_)
    {
        SDL_HideWindow(m_MenuWindow);
        gameRenderer_->ShowWindow();

        // Switch back to first selectable Item
        m_MenuItemsManager->ChangeSelectedItem(0);

        m_Visible = false;
    }
}