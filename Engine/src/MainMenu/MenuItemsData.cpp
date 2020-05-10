#include "precomp.h"
#include "MenuItemsData.h"

#include <SDL_image.h>

namespace Engine
{
	MenuItemsData::MenuItemsData()
	{
		LOG_INFO("Initializing Menu Items Data");

		if (TTF_Init() == -1)
		{
			LOG_CRITICAL("Unable to initialize SDL_ttf in MenuItemsManager::Init()");
		}

		m_TitleFont = TTF_OpenFont("./Data/menuFont.ttf", 150);
		m_ItemsFont = TTF_OpenFont("./Data/menuFont.ttf", 45);

		if (!m_TitleFont || !m_ItemsFont)
		{
			LOG_ERROR("Failed to initialize Menu Fonts: ");
			LOG_ERROR(TTF_GetError());
		}

		m_RepoQRTexture = nullptr;
	}
	MenuItemsData::~MenuItemsData()
	{
		TTF_CloseFont(m_TitleFont);
		TTF_CloseFont(m_ItemsFont);

		if (!m_RepoQRTexture)
			DestroyQRTexture();
	}

	void MenuItemsData::CreateQRTexture(SDL_Renderer* menuRenderer_)
	{
		m_RepoQRTexture = IMG_LoadTexture(menuRenderer_, "./Data/repoQR.png");
	}

	void MenuItemsData::DestroyQRTexture()
	{
		SDL_DestroyTexture(m_RepoQRTexture);

		m_RepoQRTexture = nullptr;
	}
}