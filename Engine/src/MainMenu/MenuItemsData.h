#pragma once
#include <SDL_ttf.h>


namespace Engine
{
	class MenuItemsData
	{
	public:
		SDL_Color m_DefaultColor{ 138, 3, 3 };
		SDL_Color m_SelectedColor{ 0, 0, 0 };

		TTF_Font* m_TitleFont;
		TTF_Font* m_ItemsFont;

		MenuItemsData();
		~MenuItemsData();
	};
}