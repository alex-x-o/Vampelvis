#pragma once
#include "MenuItemsData.h"
#include <SDL.h>

namespace Engine
{
	class MenuItem
	{
	public:
		MenuItem(std::string label_, MenuItemsData* data_, SDL_Renderer* menuRenderer_);
		~MenuItem();

		void Update();

		void SelectItem();
		void DeSelectItem();

		bool m_Selected{ false };
		static int numOfItems;

	private:
		int m_ItemId{ 0 };

		SDL_Renderer* m_ItemRenderer;
		MenuItemsData* m_Data;

		std::string m_Label;
		SDL_Texture* m_Texture;
		SDL_Rect m_Rect;

		void ChangeMenuItem(SDL_Color color_);

		MenuItem& operator=(MenuItem& other) = delete;
	};
}