#pragma once
#include "MenuItem.h"
#include "MenuItemsData.h"


namespace Game
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
		void EnterSubMenu(Engine::AudioManager* audioManager_);
		void LeaveSubmenu();
		void GameOver(int playerScore_);

		void ChangeSelectedItem(int newIndex_);

		std::vector<std::shared_ptr<MenuItem>> m_SelectableMenuItems;
		int m_SelectedItem{ 0 };
		bool m_SubmenuOpened{ false };

	private:
		unsigned m_NumOfItems{ 0 };
		std::vector<std::shared_ptr<MenuItem>> m_MenuItems;

		MenuItemsData m_ItemsData{};
		SDL_Renderer* m_MenuRenderer{};

		void ChangeMenu(std::vector<std::string>& labels_, int selectableBegining_, int selectableEnd_);
		void CreateMenuItems(std::vector<std::string>& labels);
		void SetSelectableItems(std::vector<std::shared_ptr<MenuItem>>::iterator begining_, std::vector<std::shared_ptr<MenuItem>>::iterator end_);
		void SubmenuDeselectItems();

		MenuItemsManager(const MenuItemsManager& other) = delete;
		MenuItemsManager& operator=(MenuItemsManager& other) = delete;
	};
}