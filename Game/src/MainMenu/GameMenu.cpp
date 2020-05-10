#include "precomp.h"
#include "GameMenu.h"
#include "MenuItemsManager.h"
#include "Core\GameConstants.h"
#include "Render/Renderer.h"

#include <SDL.h>
#include <SDL_image.h>


namespace Game
{
    bool GameMenu::Init()
    {
        LOG_INFO("Initializing Main Menu");
        
        int createWindow = SDL_CreateWindowAndRenderer(GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT,
                                                       0, &m_MenuWindow, &m_MenuRenderer);
        
        SDL_SetWindowTitle(m_MenuWindow, std::string("Vampelvis").c_str());
        setIcon();

        m_MenuItemsManager = std::make_unique<MenuItemsManager>();
        m_MenuItemsManager->Init(m_MenuRenderer);

        return !createWindow;
    }

    void GameMenu::setIcon() const
    {
        SDL_Surface* iconSurface = IMG_Load(std::string("./Textures/testTubeRed.png").c_str());

        // If icon file doesn't exist just continue
        if (!iconSurface)
        {
            LOG_INFO("Error with Menu Window Icon");
            return;
        }

        SDL_SetWindowIcon(m_MenuWindow, iconSurface);
        SDL_FreeSurface(iconSurface);
    }

    void GameMenu::Shutdown()
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

    void GameMenu::Update()
    {
        SDL_RenderClear(m_MenuRenderer);

        setBackground();
        m_MenuItemsManager->Update();

        SDL_RenderPresent(m_MenuRenderer);
    }

    void GameMenu::setBackground() const
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

    void GameMenu::ShowMenu()
    {
        if (!m_Visible)
        {
            SDL_ShowWindow(m_MenuWindow);
            SDL_RaiseWindow(m_MenuWindow);

            m_Visible = true;
        }
    }

    void GameMenu::HideMenu()
    {
        if (m_Visible)
        {
            SDL_HideWindow(m_MenuWindow);

            // Switch back to first selectable Item
            m_MenuItemsManager->ChangeSelectedItem(0);

            m_Visible = false;
        }
    }

    bool GameMenu::ProcessInput(SDL_Keycode key_, bool showMenu_)
    {
        switch (key_)
        {
        case SDLK_SPACE: if (m_MenuItemsManager->m_SubmenuOpened) return showMenu_;
                         if (showMenu_) return false;
                         break;

        case SDLK_ESCAPE: if (!showMenu_) return true;
                          break;

        case SDLK_UP: m_MenuItemsManager->GoUp();
                      break;

        case SDLK_DOWN: m_MenuItemsManager->GoDown();
                        break;

        case SDLK_RETURN: m_MenuItemsManager->EnterSubMenu();
                          break;

        case SDLK_BACKSPACE: m_MenuItemsManager->LeaveSubmenu();
                             break;
        }

        return showMenu_;
    }

	void GameMenu::GameOver(int playerScore_)
	{
        m_MenuItemsManager->GameOver(playerScore_);
	}
}