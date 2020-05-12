#pragma once
#include "precomp.h"
#include "Audio/AudioManager.h"
#include <SDL.h>

using KeyboardButton = int;

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

		virtual bool ProcessInput(SDL_Keycode key_, AudioManager* audioManager_) = 0;

		virtual ~MainMenu() = default;

		std::unordered_map<std::string, KeyboardButton> m_MenuInput{ { "PauseGame", SDLK_ESCAPE } };
	};
}