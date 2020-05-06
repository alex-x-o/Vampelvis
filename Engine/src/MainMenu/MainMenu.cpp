#include "precomp.h"
#include "MainMenu.h"
#include "Render/Renderer.h"

#include <SDL.h>

bool Engine::MainMenu::Init()
{
    LOG_INFO("Initializing Main Menu");

    int retVal = SDL_CreateWindowAndRenderer(800, 600, 0, &m_MenuWindow, &m_MenuRenderer);
    SDL_SetWindowTitle(m_MenuWindow, "Vampelvis");

	return !retVal;
}

void Engine::MainMenu::Update(Renderer* windowRenderer_)
{
    if (!isVisible()) ShowMenu(windowRenderer_);
  
    SDL_SetRenderDrawColor(m_MenuRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_MenuRenderer);
    SDL_RenderPresent(m_MenuRenderer);
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

    visible = true;
}

void Engine::MainMenu::HideMenu(Renderer* windowRenderer_)
{
    SDL_HideWindow(m_MenuWindow);

    windowRenderer_->ShowWindow();

    visible = false;
}
