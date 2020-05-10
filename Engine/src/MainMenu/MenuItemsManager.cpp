#include "precomp.h"
#include "MenuItemsManager.h"
#include "MenuItem.h"
#include "Core/Application.h"
#include "HighScoreData.h"


namespace Engine
{
    void MenuItemsManager::Init(SDL_Renderer* menuRenderer_)
    {
        LOG_INFO("Initializing Menu Items Manager");

        m_MenuRenderer = menuRenderer_;

        scoreData.LoadHighScore();
        m_BeginingLabels = { Application::m_WindowData.m_Title, "Best score: " + std::to_string(scoreData.GetHighScore()),
                             "Hall of Fame", "Game Instructions", "About us", "Press SPACE to Play" };

        MenuItem::numOfItems = 0;

        CreateMenuItems(m_BeginingLabels);
        SetSelectableItems(std::begin(m_MenuItems) + 2, std::end(m_MenuItems) - 1);
    }

    void Engine::MenuItemsManager::CreateMenuItems(std::vector<std::string>& labels_)
    {
        m_NumOfItems = static_cast<unsigned>(labels_.size());
        m_MenuItems.reserve(m_NumOfItems);

        unsigned i = 0;
        while (i < m_NumOfItems)
        {
            m_MenuItems.push_back(std::make_shared<MenuItem>(labels_[i], &m_ItemsData, m_MenuRenderer));

            ++i;
        }

        m_SelectedItem = 0;
    }

    void MenuItemsManager::SetSelectableItems(std::vector<std::shared_ptr<MenuItem>>::iterator begining_, std::vector<std::shared_ptr<MenuItem>>::iterator end_)
    {
        m_SelectableMenuItems = std::vector<std::shared_ptr<MenuItem>>(begining_, end_);

        ChangeSelectedItem(0);
    }

    void Engine::MenuItemsManager::Update()
    {
        // Render all Menu Items
        for (auto& item : m_MenuItems)
        {
            item->Update();
        }
    }

    void Engine::MenuItemsManager::Shutdown() {}

    void Engine::MenuItemsManager::GoUp()
    {
        int newIndex = m_SelectedItem - 1 < 0 ? static_cast<int>(m_SelectableMenuItems.size()) - 1 : m_SelectedItem - 1;
 
        ChangeSelectedItem(newIndex);
    }

    void Engine::MenuItemsManager::GoDown()
    {
        int newIndex = (m_SelectedItem + 1) % static_cast<int>(m_SelectableMenuItems.size());

        ChangeSelectedItem(newIndex);
    }

    void MenuItemsManager::EnterSubMenu()
    {
        if (m_SelectableMenuItems[m_SelectedItem]->m_Label == "Hall of Fame")
        {
            
        }
    }

    void MenuItemsManager::LeaveSubmenu()
    {
    }

    void Engine::MenuItemsManager::ChangeSelectedItem(int newIndex_)
    {
        m_SelectableMenuItems[m_SelectedItem]->DeSelectItem();

        m_SelectableMenuItems[newIndex_]->SelectItem();
        m_SelectedItem = newIndex_;
    }

    void Engine::MenuItemsManager::GameOver(int playerScore_)
    {
        std::vector<std::string> newLabels = { Application::m_WindowData.m_Title, " " ,"GAME OVER", 
                                               "Your score: " + std::to_string(playerScore_),
                                               "Press SPACE to return to Main Menu" };

        if (playerScore_ > scoreData.GetHighScore())
            newLabels[3] = "New High Score!!! Score: " + std::to_string(playerScore_);

        m_MenuItems.clear();
        m_SelectableMenuItems.clear();
        MenuItem::numOfItems = 0;

        CreateMenuItems(newLabels);
        SetSelectableItems(std::begin(m_MenuItems) + 3, std::end(m_MenuItems) - 1);
    }
}