#pragma once

#include "MenuItem.h"
#include "MenuItemsData.h"


namespace Engine
{
	class MenuItemsManager
	{
	public:
		bool Init(SDL_Renderer* menuRenderer_);
		void Update();
		void Shutdown();
	
		void GoUp();
		void GoDown();
		void GameOver(int playerScore_);

		int FindSelectedItem() const;
		void ChangeSelectedItem(int oldIndex_, int newIndex_);

		MenuItemsManager() = default;

	private:
		int m_HighScore{ 0 };

		unsigned m_NumOfItems{ 0 };
		std::vector<MenuItem> m_MenuItems;

		MenuItemsData m_ItemsData{};
		SDL_Renderer* m_MenuRenderer{};

		void CreateMenuItems(std::vector<std::string>& labels);

		MenuItemsManager(const MenuItemsManager& other) = delete;
		MenuItemsManager& operator=(MenuItemsManager& other) = delete;
	};
}