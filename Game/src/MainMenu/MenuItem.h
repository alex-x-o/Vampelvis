#pragma once
#include "MenuItemsData.h"

#include <SDL.h>


namespace Game
{
	class MenuItem
	{
	public:
		MenuItem(std::string label_, MenuItemsData* data_, SDL_Renderer* menuRenderer_);
		~MenuItem();

		void Update();

		void SelectItem();
		void DeSelectItem();

		std::string m_Label;
		void ChangeMenuItem(SDL_Color color_);

		static int numOfItems;

	private:
		int m_ItemId{ 0 };

		SDL_Renderer* m_ItemRenderer;
		MenuItemsData* m_Data;
		
		SDL_Texture* m_Texture;
		SDL_Rect m_Rect;

		MenuItem(const MenuItem& other) = delete;
		MenuItem& operator=(MenuItem& other) = delete;
	};
}