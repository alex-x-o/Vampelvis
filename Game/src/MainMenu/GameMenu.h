#pragma once
#include "MenuItemsManager.h"
#include "Audio/AudioManager.h"


namespace Game
{

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
		bool ProcessInput(SDL_Keycode key_, Engine::AudioManager* audioManager_);

		GameMenu() = default;

	private:
		bool m_Visible{ true };

		SDL_Window* m_MenuWindow{};
		SDL_Renderer* m_MenuRenderer{};

		void setBackground() const;
		void setIcon(std::string icon_) const;

		GameMenu(const GameMenu& other) = delete;
		GameMenu& operator=(GameMenu& other) = delete;
	};
}