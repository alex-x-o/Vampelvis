#include "precomp.h"
#include "MainMenu.h"
#include "Render/Renderer.h"

#include <SDL.h>


bool Engine::MainMenu::Init()
{
    LOG_INFO("Initializing Main Menu");

    int retVal = SDL_CreateWindowAndRenderer(m_Data.m_Width, m_Data.m_Height, 0, &m_MenuWindow, &m_MenuRenderer);
    SDL_SetWindowTitle(m_MenuWindow, m_Data.m_Title.c_str());

    CreateMenuItems();

	return !retVal;
}

void Engine::MainMenu::CreateMenuItems()
{
    m_MenuLabels = { m_Data.m_Title, "High score: 0", "Scoreboard", "About", "Press SPACE to start game" };

    int i = 0;
    for (auto& label : m_MenuLabels)
    {
        SDL_Surface* itemSurface = TTF_RenderText_Solid(i == 0 ? m_Data.m_TitleFont : m_Data.m_ItemsFont, label.c_str(), m_Data.m_DefaultColor);
        m_MenuTextures.push_back(SDL_CreateTextureFromSurface(m_MenuRenderer, itemSurface));

        m_MenuRects.push_back({ m_Data.m_Width / 2 - itemSurface->w / 2,
                                i == 0 ? m_Data.m_Height / 4 - itemSurface->h / 2 : m_Data.m_Height / 2 + (i - 1) * itemSurface->h,
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

int Engine::MainMenu::FindSelectedItem() const
{
    int index = 1;
    auto selected = std::find(std::begin(m_Selected), std::end(m_Selected), true);
    if (selected != std::end(m_Selected))
    {
        index = static_cast<int>(std::distance(m_Selected.begin(), selected));
    }

    return index;
}

void Engine::MainMenu::Update(Renderer* windowRenderer_)
{
    if (!IsVisible()) ShowMenu(windowRenderer_);

    SDL_RenderClear(m_MenuRenderer);
    
    ChangeColorOfItem(FindSelectedItem(), m_Data.m_SelectedColor);

    // Render all Menu Items
    for (unsigned i = 0; i < m_MenuLabels.size(); ++i)
    {
        SDL_RenderCopy(m_MenuRenderer, m_MenuTextures[i], NULL, &m_MenuRects[i]);
    }

    SDL_RenderPresent(m_MenuRenderer);
}

void Engine::MainMenu::ChangeColorOfItem(int index_, SDL_Color color_)
{
    SDL_DestroyTexture(m_MenuTextures[index_]);

    SDL_Surface* itemSurface = TTF_RenderText_Solid(m_Data.m_ItemsFont, m_MenuLabels[index_].c_str(), color_);
    m_MenuTextures[index_] = SDL_CreateTextureFromSurface(m_MenuRenderer, itemSurface);

    SDL_FreeSurface(itemSurface);
}

void Engine::MainMenu::ShowMenu(Renderer* windowRenderer_)
{
    windowRenderer_->HideWindow();

    SDL_ShowWindow(m_MenuWindow);
    SDL_RaiseWindow(m_MenuWindow);

    m_Data.m_Visible = true;
}

void Engine::MainMenu::HideMenu(Renderer* windowRenderer_)
{
    SDL_HideWindow(m_MenuWindow);

    windowRenderer_->ShowWindow();

    // Switch back to first Item being selected
    int index = FindSelectedItem();
    m_Selected[index] = false;
    ChangeColorOfItem(index, m_Data.m_DefaultColor);

    m_Selected[1] = true;
    ChangeColorOfItem(1, m_Data.m_SelectedColor);

    m_Data.m_Visible = false;
}

void Engine::MainMenu::GoUp()
{
    int index = FindSelectedItem();
    m_Selected[index] = false;
    ChangeColorOfItem(index, m_Data.m_DefaultColor);

    int newIndex = --index % m_Selected.size();
    if (newIndex == 0)
        newIndex = static_cast<int>(m_Selected.size()) - 2;

    m_Selected[newIndex] = true;
}

void Engine::MainMenu::GoDown()
{
    int index = FindSelectedItem();
    m_Selected[index] = false;
    ChangeColorOfItem(index, m_Data.m_DefaultColor);

    int newIndex = ++index % m_Selected.size();
    if (newIndex == m_Selected.size() - 1)
        newIndex = 1;

    m_Selected[newIndex] = true;
}