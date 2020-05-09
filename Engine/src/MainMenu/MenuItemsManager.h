#pragma once
#include "MenuItem.h"
#include "MenuItemsData.h"


namespace Engine
{
	class MenuItemsManager
	{
	public:
		void Init(SDL_Renderer* menuRenderer_);
		void Update();
		void Shutdown();
	
		MenuItemsManager() = default;

		void GoUp();
		void GoDown();
		void EnterSubMenu();
		void LeaveSubmenu();
		void GameOver(int playerScore_);

		void ChangeSelectedItem(int newIndex_);

		std::vector<std::shared_ptr<MenuItem>> m_SelectableMenuItems;
		int m_SelectedItem{ 0 };

	private:
		int m_HighScore{ 0 };

		unsigned m_NumOfItems{ 0 };
		std::vector<std::shared_ptr<MenuItem>> m_MenuItems;
		std::vector<std::string> m_BeginingLabels;

		MenuItemsData m_ItemsData{};
		SDL_Renderer* m_MenuRenderer{};

		void CreateMenuItems(std::vector<std::string>& labels);
		void SetSelectableItems(std::vector<std::shared_ptr<MenuItem>>::iterator begining_, std::vector<std::shared_ptr<MenuItem>>::iterator end_);

		MenuItemsManager(const MenuItemsManager& other) = delete;
		MenuItemsManager& operator=(MenuItemsManager& other) = delete;
	};
}