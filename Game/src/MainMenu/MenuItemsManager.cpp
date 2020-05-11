#include "precomp.h"
#include "MenuItemsManager.h"
#include "MenuItem.h"
#include "HighScoreData.h"
#include "MenuLabelsData.h"
#include "Core\GameConstants.h"


namespace Game
{
    void MenuItemsManager::Init(SDL_Renderer* menuRenderer_)
    {
        LOG_INFO("Initializing Menu Items Manager");

        m_MenuRenderer = menuRenderer_;

        // Update High Score from file
        scoreData->LoadHighScore();
        MenuLabelsData::BEGINING_LABELS[1] = "Best score: " + std::to_string(scoreData->GetHighScore());

        ChangeMenu(MenuLabelsData::BEGINING_LABELS, 2, 1);
    }

    void MenuItemsManager::ChangeMenu(std::vector<std::string>& labels_, int selectableBegining_, int selectableEnd_)
    {
        m_MenuItems.clear();
        m_SelectableMenuItems.clear();
        MenuItem::numOfItems = 0;

        CreateMenuItems(labels_);
        SetSelectableItems(std::begin(m_MenuItems) + selectableBegining_, std::end(m_MenuItems) - selectableEnd_);

    }

    void MenuItemsManager::CreateMenuItems(std::vector<std::string>& labels_)
    {
        m_NumOfItems = static_cast<unsigned>(labels_.size());
        m_MenuItems.reserve(m_NumOfItems);

        unsigned i = 0;
        while (i < m_NumOfItems)
        {
            m_MenuItems.push_back(std::make_shared<MenuItem>(labels_[i], &m_ItemsData, m_MenuRenderer));

            ++i;
        }
    }

    void MenuItemsManager::SetSelectableItems(std::vector<std::shared_ptr<MenuItem>>::iterator begining_, std::vector<std::shared_ptr<MenuItem>>::iterator end_)
    {
        m_SelectableMenuItems = std::vector<std::shared_ptr<MenuItem>>(begining_, end_);

        m_SelectedItem = 0;
        ChangeSelectedItem(m_SelectedItem);
    }

    void MenuItemsManager::Update()
    {
        // Render all Menu Items
        for (auto& item : m_MenuItems)
        {
            item->Update();
        }

        if (m_ItemsData.m_RepoQRTexture)
        {
            SDL_Rect qrRect = { GameConstants::SCREEN_WIDTH/2-100, GameConstants::SCREEN_HEIGHT/2-80, 200, 200 };
            SDL_RenderCopy(m_MenuRenderer, m_ItemsData.m_RepoQRTexture, NULL, &qrRect);
        }
    }

    void MenuItemsManager::Shutdown() {}

    void MenuItemsManager::GoUp()
    {
        if (m_SubmenuOpened) return;

        int newIndex = m_SelectedItem - 1 < 0 ? static_cast<int>(m_SelectableMenuItems.size()) - 1 : m_SelectedItem - 1;
 
        ChangeSelectedItem(newIndex);
    }

    void MenuItemsManager::GoDown()
    {
        if (m_SubmenuOpened) return;

        int newIndex = (m_SelectedItem + 1) % static_cast<int>(m_SelectableMenuItems.size());

        ChangeSelectedItem(newIndex);
    }

    void MenuItemsManager::EnterSubMenu()
    {
        if (m_SelectableMenuItems[m_SelectedItem]->m_Label == MenuLabelsData::BEGINING_LABELS[2])
        {
            auto hallOfFame = scoreData->GetSortedHallOfFame();

            std::vector<std::string> bestVampires;
            std::transform(std::begin(hallOfFame), std::end(hallOfFame), std::back_inserter(bestVampires),
                [](std::pair<const int, std::string>& entry) {return entry.second + " : " +std::to_string(entry.first); });

            bestVampires.insert(std::begin(bestVampires), MenuLabelsData::BEGINING_LABELS[2]);
            bestVampires.push_back("Press ESCAPE to return");

            ChangeMenu(bestVampires, static_cast<int>(bestVampires.size()) - 1, 0);
            SubmenuDeselectItems();
        }
        else if (m_SelectableMenuItems[m_SelectedItem]->m_Label == MenuLabelsData::BEGINING_LABELS[3])
        {
            // TOGGLE SOUND
        }
        else if (m_SelectableMenuItems[m_SelectedItem]->m_Label == MenuLabelsData::BEGINING_LABELS[4])
        {
            ChangeMenu(MenuLabelsData::INSTRUCTION_LABELS, static_cast<int>(MenuLabelsData::INSTRUCTION_LABELS.size()) - 1, 0);
            SubmenuDeselectItems();
        }
        else if (m_SelectableMenuItems[m_SelectedItem]->m_Label == MenuLabelsData::BEGINING_LABELS[5])
        {
            ChangeMenu(MenuLabelsData::ABOUTUS_LABELS, static_cast<int>(MenuLabelsData::ABOUTUS_LABELS.size()) - 1, 0);
            SubmenuDeselectItems();

            m_ItemsData.CreateQRTexture(m_MenuRenderer);
        }
    }

    void MenuItemsManager::SubmenuDeselectItems()
    {
        m_SubmenuOpened = true;
        m_SelectedItem = 0;
        m_SelectableMenuItems[0]->DeSelectItem();
    }

    void MenuItemsManager::LeaveSubmenu()
    {
        if (m_SubmenuOpened)
        {
            ChangeMenu(MenuLabelsData::BEGINING_LABELS, 2, 1);

            m_SubmenuOpened = false;
            if (m_ItemsData.m_RepoQRTexture) m_ItemsData.DestroyQRTexture();
        }
    }

    void MenuItemsManager::ChangeSelectedItem(int newIndex_)
    {
        m_SelectableMenuItems[m_SelectedItem]->DeSelectItem();

        m_SelectableMenuItems[newIndex_]->SelectItem();
        m_SelectedItem = newIndex_;
    }

    void MenuItemsManager::GameOver(int playerScore_)
    {
        MenuLabelsData::GAMEOVER_LABELS[3] = playerScore_ > scoreData->GetHighScore() ? 
                                             "New High Score!!! Score: " + std::to_string(playerScore_) :
                                             "Your score: " + std::to_string(playerScore_);

        ChangeMenu(MenuLabelsData::GAMEOVER_LABELS, 3, 1);
    }
}