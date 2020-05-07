#pragma once

#include <SDL_ttf.h>

namespace Engine 
{
	class MainMenuData
	{
	public:
		int m_Width;
		int m_Height;
		std::string m_Title;
		SDL_Surface* m_Icon;

		SDL_Color m_DefaultColor{ 138, 3, 3 };
		SDL_Color m_SelectedColor{ 0, 0, 0 };

		TTF_Font* m_TitleFont;
		TTF_Font* m_ItemsFont;

		int m_HighScore{ 0 };

		MainMenuData();
		~MainMenuData();
	};
}