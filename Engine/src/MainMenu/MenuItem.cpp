#include "precomp.h"
#include "MenuItem.h"
#include "Core/Application.h"

#include <SDL_ttf.h>


namespace Engine
{
    int MenuItem::numOfItems = 0;

    SDL_Rect CalculateRect(int windowW_, int windowH_, int textW_, int textH_, int index_)
    {
        return {windowW_/2 - textW_/2, index_ == 0 ? windowH_/4 - textH_/2 : windowH_/2 + (index_ - 2)*textH_, textW_, textH_};
    }

    MenuItem::MenuItem(std::string label_, MenuItemsData* data_, SDL_Renderer* menuRenderer_)
        : m_ItemId(numOfItems++),
          m_Data(data_),
          m_Label(label_),
          m_ItemRenderer(menuRenderer_)
    {
        LOG_INFO(fmt::format("Initializing Item number {}", m_ItemId));

        SDL_Surface* itemSurface = TTF_RenderText_Solid(m_ItemId == 0 ? m_Data->m_TitleFont: m_Data->m_ItemsFont,
                                                        m_Label.c_str(), m_Data->m_DefaultColor);
        
        m_Texture = SDL_CreateTextureFromSurface(m_ItemRenderer, itemSurface);

        m_Rect = CalculateRect(Application::m_WindowData.m_Width, Application::m_WindowData.m_Height,
                               itemSurface->w, itemSurface->h, m_ItemId);

        SDL_FreeSurface(itemSurface);
    }

    void MenuItem::Update()
    {
        SDL_RenderCopy(m_ItemRenderer, m_Texture, NULL, &m_Rect);
    }

    MenuItem::~MenuItem()
    {
        SDL_DestroyTexture(m_Texture);
    }

    void MenuItem::SelectItem()
    {
        ChangeMenuItem(m_Data->m_SelectedColor);
    }

    void MenuItem::DeSelectItem()
    {
        ChangeMenuItem(m_Data->m_DefaultColor);
    }

    void MenuItem::ChangeMenuItem(SDL_Color color_)
    {
        SDL_DestroyTexture(m_Texture);

        SDL_Surface* itemSurface = TTF_RenderText_Solid(m_Data->m_ItemsFont, m_Label.c_str(), color_);
        m_Texture = SDL_CreateTextureFromSurface(m_ItemRenderer, itemSurface);

        m_Rect = CalculateRect(Application::m_WindowData.m_Width, Application::m_WindowData.m_Height,
                               itemSurface->w, itemSurface->h, m_ItemId);

        SDL_FreeSurface(itemSurface);
    }
}