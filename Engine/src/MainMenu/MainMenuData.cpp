#include "precomp.h"
#include "Render/WindowData.h"
#include "Core/Application.h"

#include <SDL_ttf.h>

Engine::MainMenuData::MainMenuData()
{
	WindowData windowData = Application::GetWindowData();
	m_Width = windowData.m_Width;
	m_Height = windowData.m_Height;
	m_Title = windowData.m_Title;
	m_Icon = windowData.m_Icon;

	if (TTF_Init() == -1)
	{
		LOG_CRITICAL("Unable to initialize SDL_ttf");
	}

	m_TitleFont = TTF_OpenFont("./menuFont.ttf", 150);
	m_ItemsFont = TTF_OpenFont("./menuFont.ttf", 50);

	if (!m_TitleFont || !m_ItemsFont)
	{
		LOG_ERROR("Failed to initialize Menu Fonts: ");
		LOG_ERROR(TTF_GetError());
	}
}

Engine::MainMenuData::~MainMenuData()
{
	TTF_CloseFont(m_TitleFont);
	TTF_CloseFont(m_ItemsFont);
}