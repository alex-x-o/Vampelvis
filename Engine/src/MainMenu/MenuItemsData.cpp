#include "precomp.h"
#include "MenuItemsData.h"

namespace Engine
{
	MenuItemsData::MenuItemsData()
	{
		LOG_INFO("Initializing Menu Items Data");

		if (TTF_Init() == -1)
		{
			LOG_CRITICAL("Unable to initialize SDL_ttf in MenuItemsManager::Init()");
		}

		m_TitleFont = TTF_OpenFont("./menuFont.ttf", 160);
		m_ItemsFont = TTF_OpenFont("./menuFont.ttf", 50);

		if (!m_TitleFont || !m_ItemsFont)
		{
			LOG_ERROR("Failed to initialize Menu Fonts: ");
			LOG_ERROR(TTF_GetError());
		}
	}
	MenuItemsData::~MenuItemsData()
	{
		TTF_CloseFont(m_TitleFont);
		TTF_CloseFont(m_ItemsFont);
	}
}