#pragma once
#include "MainMenuData.h"


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

		void GoUp();
		void GoDown();

		bool IsVisible() const { return m_Visible; }

	private:
		MainMenuData m_Data{};
		bool m_Visible{ true };

		SDL_Window* m_MenuWindow;
		SDL_Renderer* m_MenuRenderer;

		std::vector<std::string> m_MenuLabels;
		std::vector<SDL_Texture*> m_MenuTextures;
		std::vector<SDL_Rect> m_MenuRects;
		std::vector<bool> m_Selected;

		void CreateMenuItems();
		int FindSelectedItem() const;
		void ChangeSelectedItem(int oldIndex_, int newIndex_);
		void ChangeColorOfItem(int index_, SDL_Color color_);
	};
}