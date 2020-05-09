#include "precomp.h"
#include "MenuItemsManager.h"
#include "MenuItem.h"
#include "Core/Application.h"

#include <nlohmann/json.hpp>


namespace Engine
{
    bool MenuItemsManager::Init(SDL_Renderer* menuRenderer_)
    {
        LOG_INFO("Initializing Menu Items Manager");

        m_MenuRenderer = menuRenderer_;

        // Get current High score from file
        if (!std::filesystem::exists("score.json"))
        {
            LOG_INFO("File with High score does not exist!");
            return false;
        }

        std::ifstream inputFile("score.json");
        nlohmann::json scoreObject = nlohmann::json::parse(inputFile);
        m_HighScore = scoreObject.at("score");

        std::vector<std::string> beginingLabels = { Application::m_WindowData.m_Title,
                                                   "High score: " + std::to_string(m_HighScore),
                                                   "Scoreboard", "About", "Press SPACE to Play" };

        MenuItem::numOfItems = 0;
        CreateMenuItems(beginingLabels);

        return true;
    }

    void Engine::MenuItemsManager::CreateMenuItems(std::vector<std::string>& labels_)
    {
        m_NumOfItems = static_cast<unsigned>(labels_.size());
        m_MenuItems.reserve(m_NumOfItems);

        unsigned i = 0;
        while (i < m_NumOfItems)
        {
            m_MenuItems.emplace_back(labels_[i], &m_ItemsData, m_MenuRenderer);
            
            ++i;
        }

        LOG_INFO("Done initializing Menu items");
    }

    void Engine::MenuItemsManager::Update()
    {
        // Render all Menu Items
        for (auto& item : m_MenuItems)
        {
            item.Update();
        }
    }

    void Engine::MenuItemsManager::Shutdown() {}

    void Engine::MenuItemsManager::GoUp()
    {
        int index = FindSelectedItem();
        if (index < 0) return;
        int newIndex = index - 1 == 1 ? static_cast<int>(m_MenuItems.size() - 2) : index - 1;

        ChangeSelectedItem(index, newIndex);
    }

    void Engine::MenuItemsManager::GoDown()
    {
        int index = FindSelectedItem();
        if (index < 0) return;
        int newIndex = index + 1 == static_cast<int>(m_MenuItems.size() - 1) ? 2 : index + 1;

        ChangeSelectedItem(index, newIndex);
    }

    void Engine::MenuItemsManager::ChangeSelectedItem(int oldIndex_, int newIndex_)
    {
        m_MenuItems[oldIndex_].m_Selected = false;
        m_MenuItems[oldIndex_].DeSelectItem();

        m_MenuItems[newIndex_].m_Selected = true;
        m_MenuItems[newIndex_].SelectItem();
    }

    void Engine::MenuItemsManager::GameOver(int playerScore_)
    {
        std::vector<std::string> newLabels = { Application::m_WindowData.m_Title, "GAME OVER", 
                                               "Your score: " + std::to_string(playerScore_) ,
                                               "Press SPACE to return to Main Menu" };

        m_MenuItems.clear();
        MenuItem::numOfItems = 0;

        CreateMenuItems(newLabels);

        m_MenuItems[2].m_Selected = false;
    }

    int Engine::MenuItemsManager::FindSelectedItem() const
    {
        int index{ -1 };
        auto selected = std::find_if(std::cbegin(m_MenuItems), std::cend(m_MenuItems), [](const MenuItem& i) {return i.m_Selected; });
        if (selected != std::cend(m_MenuItems))
        {
            index = static_cast<int>(std::distance(std::cbegin(m_MenuItems), selected));
        }

        return index;
    }
}