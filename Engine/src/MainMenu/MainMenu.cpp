#include "precomp.h"
#include "MainMenu.h"
#include "Render/Renderer.h"
#include "Core/Application.h"

#include <SDL.h>


bool Engine::MainMenu::Init()
{
    LOG_INFO("Initializing Main Menu");

    if (TTF_Init() == -1)
    {
        LOG_CRITICAL("Unable to initialize SDL_ttf");
        return false;
    }

    m_WindowData = Engine::Application::GetWindowData();

    int retVal = SDL_CreateWindowAndRenderer(m_WindowData.m_Width, m_WindowData.m_Height, 0, &m_MenuWindow, &m_MenuRenderer);
    SDL_SetWindowTitle(m_MenuWindow, m_WindowData.m_Title.c_str());

    m_TitleFont = TTF_OpenFont("./menuFont.ttf", 150);
    m_ItemsFont = TTF_OpenFont("./menuFont.ttf", 50);
    if (!m_TitleFont || !m_ItemsFont)
    {
        LOG_ERROR("Failed to initialize Menu Fonts: ");
        LOG_ERROR(TTF_GetError());

        return false;
    }

    m_MenuLabels = { m_WindowData.m_Title, "High score", "About", "Press SPACE to start game"};

	return !retVal;
}

void Engine::MainMenu::Update(Renderer* windowRenderer_)
{
    if (!isVisible()) ShowMenu(windowRenderer_);
  
    SDL_Color redColor = { 255,0,0 };

    SDL_SetRenderDrawColor(m_MenuRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_MenuRenderer);
    
    ShowMenuItems(redColor);

    SDL_RenderPresent(m_MenuRenderer);
}

void Engine::MainMenu::ShowMenuItems(SDL_Color& textColor) const
{
    int i = 0;
    for (auto& label : m_MenuLabels)
    {
        SDL_Surface* itemSurface = TTF_RenderText_Solid(i == 0 ? m_TitleFont : m_ItemsFont, label.c_str(), textColor);
        SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(m_MenuRenderer, itemSurface);

        SDL_Rect itemRect = { m_WindowData.m_Width/2 - itemSurface->w/2,
                              i == 0 ? m_WindowData.m_Height/4 - itemSurface->h/2 : m_WindowData.m_Height/2 + i*itemSurface->h,
                              itemSurface->w, itemSurface->h };
        
        SDL_RenderCopy(m_MenuRenderer, itemTexture, NULL, &itemRect);

        ++i;
    }
}

void Engine::MainMenu::Shutdown()
{
    LOG_INFO("Shutting down Menu Renderer");

    if (m_MenuRenderer != nullptr)
    {
        SDL_DestroyRenderer(m_MenuRenderer);
    }

    m_MenuRenderer = nullptr;

    LOG_INFO("Shutting down Menu Window");

    if (m_MenuWindow != nullptr)
    {
        SDL_DestroyWindow(m_MenuWindow);
    }

    m_MenuWindow = nullptr;
}

void Engine::MainMenu::ShowMenu(Renderer* windowRenderer_)
{
    windowRenderer_->HideWindow();

    SDL_ShowWindow(m_MenuWindow);
    SDL_RaiseWindow(m_MenuWindow);

    m_Visible = true;
}

void Engine::MainMenu::HideMenu(Renderer* windowRenderer_)
{
    SDL_HideWindow(m_MenuWindow);

    windowRenderer_->ShowWindow();

    m_Visible = false;
}
