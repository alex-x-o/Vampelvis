#pragma once
#include "MenuItemsManager.h"


namespace Engine
{
	class Renderer;

	class MainMenu
	{
	public:
		bool Init();
		void Update(Renderer* gameRenderer_);
		void Shutdown();

		void ShowMenu(Renderer* gameRenderer_);
		void HideMenu(Renderer* gameRenderer_);

		std::unique_ptr<MenuItemsManager> m_MenuItemsManager;

		MainMenu() = default;

	private:
		bool m_Visible{ true };

		SDL_Window* m_MenuWindow{};
		SDL_Renderer* m_MenuRenderer{};

		void setBackground() const;
		void setIcon() const;

		MainMenu(const MainMenu& other) = delete;
		MainMenu& operator=(MainMenu& other) = delete;
	};
}