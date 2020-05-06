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

		SDL_Color m_DefaultColor{ 255, 0, 0 };
		SDL_Color m_SelectedColor{ 255, 255, 255 };

		TTF_Font* m_TitleFont;
		TTF_Font* m_ItemsFont;

		bool m_Visible{ true };

		MainMenuData();
		~MainMenuData();
	};
}