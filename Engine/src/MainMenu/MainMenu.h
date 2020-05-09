#pragma once
#include "MenuItemsManager.h"


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

		bool IsMenuVisible() const { return m_Visible; }

		std::unique_ptr<MenuItemsManager> m_MenuItemsManager;

	private:
		bool m_Visible{ true };

		SDL_Window* m_MenuWindow{};
		SDL_Renderer* m_MenuRenderer{};

		void setBackground() const;
		void setIcon() const;
	};
}