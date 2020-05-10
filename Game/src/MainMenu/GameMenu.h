#pragma once
#include "Core\MainMenu.h"
#include "MenuItemsManager.h"


namespace Game
{
	class Renderer;

	class GameMenu : public Engine::MainMenu
	{
	public:
		bool Init();
		void Update();
		void Shutdown();

		void ShowMenu();
		void HideMenu();
		void GameOver(int playerScore_);

		std::unique_ptr<MenuItemsManager> m_MenuItemsManager;
		bool ProcessInput(SDL_Keycode key_, bool showMenu_);

		GameMenu() = default;

	private:
		bool m_Visible{ true };

		SDL_Window* m_MenuWindow{};
		SDL_Renderer* m_MenuRenderer{};

		void setBackground() const;
		void setIcon() const;

		GameMenu(const GameMenu& other) = delete;
		GameMenu& operator=(GameMenu& other) = delete;
	};
}