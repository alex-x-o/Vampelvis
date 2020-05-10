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
                             "Hall of Fame", "Sound : on", "How to play", "About us", "Press ENTER to open or SPACE to Play" };

        ChangeMenu(m_BeginingLabels, 2, 1);
    }

    void MenuItemsManager::ChangeMenu(std::vector<std::string>& labels_, int selectableBegining_, int selectableEnd_)
    {
        m_MenuItems.clear();
        m_SelectableMenuItems.clear();
        MenuItem::numOfItems = 0;

        CreateMenuItems(labels_);
        SetSelectableItems(std::begin(m_MenuItems) + selectableBegining_, std::end(m_MenuItems) - selectableEnd_);

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

        if (m_ItemsData.m_RepoQRTexture)
        {
            SDL_Rect qrRect = { Application::m_WindowData.m_Width/2-100, Application::m_WindowData.m_Height/2-80, 200, 200 };
            SDL_RenderCopy(m_MenuRenderer, m_ItemsData.m_RepoQRTexture, NULL, &qrRect);
        }
    }

    void Engine::MenuItemsManager::Shutdown() {}

    void Engine::MenuItemsManager::GoUp()
    {
        if (m_SubmenuOpened) return;

        int newIndex = m_SelectedItem - 1 < 0 ? static_cast<int>(m_SelectableMenuItems.size()) - 1 : m_SelectedItem - 1;
 
        ChangeSelectedItem(newIndex);
    }

    void Engine::MenuItemsManager::GoDown()
    {
        if (m_SubmenuOpened) return;

        int newIndex = (m_SelectedItem + 1) % static_cast<int>(m_SelectableMenuItems.size());

        ChangeSelectedItem(newIndex);
    }

    void MenuItemsManager::EnterSubMenu()
    {
        if (m_SelectableMenuItems[m_SelectedItem]->m_Label == m_BeginingLabels[2])
        {
            auto hallOfFame = scoreData.GetHallOfFame();

            // Sort High Scores 
            std::map<int, std::string, std::greater<int>> sortedMap;
            
            for (auto& it : hallOfFame)
                sortedMap[it.second] = it.first;

            std::vector<std::string> bestVampires;

            std::transform(std::begin(sortedMap), std::end(sortedMap), std::back_inserter(bestVampires),
                [](std::pair<const int, std::string>& entry) {return entry.second + " : " +std::to_string(entry.first); });

            bestVampires.insert(std::begin(bestVampires), m_BeginingLabels[2]);
            bestVampires.push_back("Press BACKSPACE to return");
            ChangeMenu(bestVampires, static_cast<int>(bestVampires.size()) - 1, 0);

            m_SubmenuOpened = true;
            m_SelectableMenuItems[0]->DeSelectItem();
        }
        else if (m_SelectableMenuItems[m_SelectedItem]->m_Label == m_BeginingLabels[4])
        {
            std::vector<std::string> instructions = { m_BeginingLabels[4], "You have awakened from your slumber!",
                                                      "Fly through the SPACE, SHIFT into a terrifying",
                                                      "creature of the night by drinking Red Blood Vials or put",
                                                      "your death under CONTROL by drinking Blue Blood", 
                                                      "Vials and becoming immune to damage for a short period.",
                                                      "Press BACKSPACE to return"};

            ChangeMenu(instructions, static_cast<int>(instructions.size()) - 1, 0);
        
            m_SubmenuOpened = true;
            m_SelectableMenuItems[0]->DeSelectItem();
        }
        else if (m_SelectableMenuItems[m_SelectedItem]->m_Label == m_BeginingLabels[5])
        {
            std::vector<std::string> aboutUs(3, " ");
            aboutUs.insert(std::begin(aboutUs), m_BeginingLabels[5]);
            aboutUs.push_back("Scan it to find out more!");
            aboutUs.push_back("Press BACKSPACE to return");

            ChangeMenu(aboutUs, static_cast<int>(aboutUs.size()) - 1, 0);

            m_SubmenuOpened = true;
            m_SelectableMenuItems[0]->DeSelectItem();

            m_ItemsData.CreateQRTexture(m_MenuRenderer);
        }
    }

    void MenuItemsManager::LeaveSubmenu()
    {
        if (m_SubmenuOpened)
        {
            ChangeMenu(m_BeginingLabels, 2, 1);

            m_SubmenuOpened = false;
            if (m_ItemsData.m_RepoQRTexture) m_ItemsData.DestroyQRTexture();
        }
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

        ChangeMenu(newLabels, 3, 1);
    }
}