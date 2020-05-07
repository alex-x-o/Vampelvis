#include "precomp.h"
#include "MainMenu.h"
#include "Render/Renderer.h"

#include <SDL.h>
#include <SDL_image.h>


bool Engine::MainMenu::Init()
{
    LOG_INFO("Initializing Main Menu");

    int retVal = SDL_CreateWindowAndRenderer(m_Data.m_Width, m_Data.m_Height, 0, &m_MenuWindow, &m_MenuRenderer);
    SDL_SetWindowTitle(m_MenuWindow, m_Data.m_Title.c_str());

    SDL_SetWindowIcon(m_MenuWindow, m_Data.m_Icon);

    CreateMenuItems();

	return !retVal;
}

void Engine::MainMenu::CreateMenuItems()
{
    m_MenuLabels = { m_Data.m_Title, "High score: " + std::to_string(m_Data.m_HighScore), "Scoreboard", "About", "Press SPACE to start game" };

    int i = 0;
    for (auto& label : m_MenuLabels)
    {
        SDL_Surface* itemSurface = TTF_RenderText_Solid(i == 0 ? m_Data.m_TitleFont : m_Data.m_ItemsFont, label.c_str(), m_Data.m_DefaultColor);
        m_MenuTextures.push_back(SDL_CreateTextureFromSurface(m_MenuRenderer, itemSurface));

        int windowWidth = m_Data.m_Width;
        int windowHeight = m_Data.m_Height;
        int textWidth = itemSurface->w;
        int textHeight = itemSurface->h;

        m_MenuRects.push_back({windowWidth/2-textWidth/2, i == 0 ? windowHeight/4-textHeight/2 : windowHeight/2+(i-1)*textHeight, textWidth, textHeight });

        m_Selected.push_back(i == 1 ? true : false);

        SDL_FreeSurface(itemSurface);

        ++i;
    }

    ChangeColorOfItem(1, m_Data.m_SelectedColor);
}

void Engine::MainMenu::Shutdown()
{
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

    for (auto& texture : m_MenuTextures)
    {
        SDL_DestroyTexture(texture);
    }
}

int Engine::MainMenu::FindSelectedItem() const
{
    int index{ 1 };
    auto selected = std::find(std::cbegin(m_Selected), std::cend(m_Selected), true);
    if (selected != std::cend(m_Selected))
    {
        index = static_cast<int>(std::distance(std::cbegin(m_Selected), selected));
    }

    return index;
}

void Engine::MainMenu::Update(Renderer* windowRenderer_)
{
    if (!m_Visible) ShowMenu(windowRenderer_);

    SDL_RenderClear(m_MenuRenderer);

    SDL_Texture* background = IMG_LoadTexture(m_MenuRenderer, "./Textures/backCave.png");
    SDL_RenderCopy(m_MenuRenderer, background, NULL, NULL);

    // Render all Menu Items
    for (unsigned i = 0; i < m_MenuLabels.size(); ++i)
    {
        SDL_RenderCopy(m_MenuRenderer, m_MenuTextures[i], NULL, &m_MenuRects[i]);
    }

    SDL_RenderPresent(m_MenuRenderer);
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

    // Switch back to first Item being selected
    ChangeSelectedItem(FindSelectedItem(), 1);

    m_Visible = false;
}

void Engine::MainMenu::GoUp()
{
    int index = FindSelectedItem();
    int newIndex = index - 1 == 0 ? static_cast<int>(m_Selected.size()) - 2 : index - 1;
    
    ChangeSelectedItem(index, newIndex);
}

void Engine::MainMenu::GoDown()
{
    int index = FindSelectedItem();
    int newIndex = index % (m_Selected.size() - 2) + 1;
    
    ChangeSelectedItem(index, newIndex);
}

void Engine::MainMenu::ChangeSelectedItem(int oldIndex_, int newIndex_)
{
    m_Selected[oldIndex_] = false;
    ChangeColorOfItem(oldIndex_, m_Data.m_DefaultColor);

    m_Selected[newIndex_] = true;
    ChangeColorOfItem(newIndex_, m_Data.m_SelectedColor);
}

void Engine::MainMenu::ChangeColorOfItem(int index_, SDL_Color color_)
{
    SDL_DestroyTexture(m_MenuTextures[index_]);

    SDL_Surface* itemSurface = TTF_RenderText_Solid(m_Data.m_ItemsFont, m_MenuLabels[index_].c_str(), color_);
    m_MenuTextures[index_] = SDL_CreateTextureFromSurface(m_MenuRenderer, itemSurface);

    SDL_FreeSurface(itemSurface);
}