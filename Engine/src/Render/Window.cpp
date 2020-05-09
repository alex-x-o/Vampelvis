#include "precomp.h"
#include "Window.h"

#include <SDL.h>
#include <SDL_image.h>

namespace Engine
{
    bool Window::Init(const WindowData& windowData_)
    {
        LOG_INFO("Initializing Window");

        m_WindowData = windowData_;
        ASSERT(m_WindowData.m_Width > 0 && m_WindowData.m_Height > 0, "Window size must be greater than zero");

        m_NativeWindowHandle = SDL_CreateWindow(
            m_WindowData.m_Title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_WindowData.m_Width,
            m_WindowData.m_Height,
            0
        );

        SetWindowIcon();

        if (m_NativeWindowHandle == nullptr)
        {
            LOG_CRITICAL("Unable to create a window. SDL Error: {}", SDL_GetError());
            return false;
        }

        LOG_INFO("Window successfully initialized");

        SDL_HideWindow(m_NativeWindowHandle);
        return true;
    }

    void Window::SetWindowIcon()
    {
        SDL_Surface* iconSurface = IMG_Load(m_WindowData.m_Icon.c_str());

        if (iconSurface)
        {
            SDL_SetWindowIcon(m_NativeWindowHandle, iconSurface);
            SDL_FreeSurface(iconSurface);
        }
    }

    bool Window::Shutdown()
    {
        LOG_INFO("Shutting down Window");

        if (m_NativeWindowHandle != nullptr)
        {
            SDL_DestroyWindow(m_NativeWindowHandle);
        }

        m_NativeWindowHandle = nullptr;

        return true;
    }

    Window::~Window()
    {
        Shutdown();
    }
}
