#pragma once

#include "Render/WindowData.h"

#include <SDL_ttf.h>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Color;

namespace Engine
{
	class Renderer;

	class MainMenu
	{
	public:
		SDL_Color m_DarkColor{ 255, 0, 0 };
		SDL_Color m_LightColor{ 255, 255, 255 };

		bool Init();
		void Update(Renderer* windowRenderer_);
		void Shutdown();

		void ShowMenu(Renderer* windowRenderer_);
		void HideMenu(Renderer* windowRenderer_);
		bool IsVisible() const { return m_Visible; }

		void GoUp();
		void GoDown();

	private:
		SDL_Window* m_MenuWindow;
		WindowData m_WindowData;
		SDL_Renderer* m_MenuRenderer;

		TTF_Font* m_TitleFont;
		TTF_Font* m_ItemsFont;

		bool m_Visible{ true };

		std::vector<std::string> m_MenuLabels;
		std::vector<SDL_Texture*> m_MenuTextures;
		std::vector<SDL_Rect> m_MenuRects;

		std::vector<bool> m_Selected;
		void ChangeColorOfItem(int index_, SDL_Color color_);

		void CreateMenuItems();
	};
}