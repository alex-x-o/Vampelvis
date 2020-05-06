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
		bool Init();
		void Update(Renderer* windowRenderer_);
		void Shutdown();

		void ShowMenu(Renderer* windowRenderer_);
		void HideMenu(Renderer* windowRenderer_);
		bool isVisible() const { return m_Visible; }

	private:
		SDL_Window* m_MenuWindow;
		WindowData m_WindowData;
		SDL_Renderer* m_MenuRenderer;
		TTF_Font* m_TitleFont;
		TTF_Font* m_ItemsFont;

		bool m_Visible{ true };

		std::vector<std::string> m_MenuLabels;

		void ShowMenuItems(SDL_Color& textColor) const;
	};
}