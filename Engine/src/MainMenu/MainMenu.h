#pragma once

struct SDL_Window;
struct SDL_Renderer;

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
		bool isVisible() const { return visible; }

	private:
		SDL_Window* m_MenuWindow;
		SDL_Renderer* m_MenuRenderer;

		bool visible{ true };
	};
}