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

    CreateMenuItems();

	return !retVal;
}

void Engine::MainMenu::CreateMenuItems()
{
    m_MenuLabels = { m_WindowData.m_Title, "High score", "About", "Press SPACE to start game" };

    int i = 0;
    for (auto& label : m_MenuLabels)
    {
        SDL_Surface* itemSurface = TTF_RenderText_Solid(i == 0 ? m_TitleFont : m_ItemsFont, label.c_str(), m_DarkColor);
        m_MenuTextures.push_back(SDL_CreateTextureFromSurface(m_MenuRenderer, itemSurface));

        m_MenuRects.push_back({ m_WindowData.m_Width / 2 - itemSurface->w / 2,
                                i == 0 ? m_WindowData.m_Height / 4 - itemSurface->h / 2 : m_WindowData.m_Height / 2 + (i - 1) * itemSurface->h,
                                itemSurface->w, itemSurface->h });

        m_Selected.push_back(i == 1 ? true : false);

        SDL_FreeSurface(itemSurface);

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

    for (auto& texture : m_MenuTextures)
    {
        SDL_DestroyTexture(texture);
    }
}

int FindSelectedItem(std::vector<bool>& vec_)
{
    int index = 1;
    auto selected = std::find(std::begin(vec_), std::end(vec_), true);
    if (selected != std::end(vec_))
    {
        index = static_cast<int>(std::distance(vec_.begin(), selected));
    }

    return index;
}

void Engine::MainMenu::Update(Renderer* windowRenderer_)
{
    if (!IsVisible()) ShowMenu(windowRenderer_);

    SDL_SetRenderDrawColor(m_MenuRenderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(m_MenuRenderer);
    
    ChangeColorOfItem(FindSelectedItem(m_Selected), m_LightColor);
    for (unsigned i = 0; i < m_MenuLabels.size(); ++i)
    {
        SDL_RenderCopy(m_MenuRenderer, m_MenuTextures[i], NULL, &m_MenuRects[i]);
    }

    SDL_RenderPresent(m_MenuRenderer);
}

void Engine::MainMenu::ChangeColorOfItem(int index_, SDL_Color color_)
{
    SDL_DestroyTexture(m_MenuTextures[index_]);

    SDL_Color lightColor = { 0, 0, 255 };

    SDL_Surface* itemSurface = TTF_RenderText_Solid(m_ItemsFont, m_MenuLabels[index_].c_str(), color_);
    m_MenuTextures[index_] = SDL_CreateTextureFromSurface(m_MenuRenderer, itemSurface);

    SDL_FreeSurface(itemSurface);
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

    int index = FindSelectedItem(m_Selected);
    m_Selected[index] = false;
    ChangeColorOfItem(index, m_DarkColor);

    m_Selected[1] = true;
    ChangeColorOfItem(1, m_LightColor);

    m_Visible = false;
}

void Engine::MainMenu::GoUp()
{
    int index = FindSelectedItem(m_Selected);
    m_Selected[index] = false;
    ChangeColorOfItem(index, m_DarkColor);

    int newIndex = (index - 1) % m_Selected.size();
    if (newIndex == 0)
        newIndex = static_cast<int>(m_Selected.size()) - 2;

    m_Selected[newIndex] = true;
}

void Engine::MainMenu::GoDown()
{
    int index = FindSelectedItem(m_Selected);
    m_Selected[index] = false;
    ChangeColorOfItem(index, m_DarkColor);

    int newIndex = (index + 1) % m_Selected.size();
    if (newIndex == m_Selected.size() - 1)
        newIndex = 1;

    m_Selected[newIndex] = true;
}