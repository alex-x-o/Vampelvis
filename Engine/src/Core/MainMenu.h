#pragma once

#include <SDL.h>

namespace Engine
{
	class MainMenu
	{
	public:
		virtual bool Init() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;

		virtual void ShowMenu() = 0;
		virtual void HideMenu() = 0;
		virtual void GameOver(int score_) = 0;

		virtual bool ProcessInput(SDL_Keycode key_, bool showMenu_) = 0;

		virtual ~MainMenu() = default;
	};
}