#pragma once
#include "Render/WindowData.h"

#include <SDL_ttf.h>

struct SDL_Renderer;

namespace Engine
{
    struct WindowData;
    class Window;
    class Entity;
    struct Texture;

    class Renderer
    {
    public:
        bool Init(const WindowData& windowData_ = WindowData());
        bool Shutdown();

        void DrawEntities(const std::vector<Entity*> renderables_, const Entity* camera);
        void DrawEntity(const Entity* r, const Entity* camera);
        void DrawPlayerScore(int score_);
        void DrawPlayerInventory(const std::unordered_map<int, int>& playerInventory_);

        void BeginScene() const;
        void EndScene() const;

        SDL_Renderer* GetNativeRenderer() const { return m_NativeRenderer; }

        void ShowWindow();
        void HideWindow();

        void SetBackgroundColor(unsigned char bgR_, unsigned char bgG_, unsigned char bgB_, unsigned char bgA_);
        void SetBackgroundColor(const Color& col_);
        void SetBackgroundTexture(Engine::Texture* texture_);
        ~Renderer();

    private:
        std::unique_ptr<Window> m_Window;
        SDL_Renderer* m_NativeRenderer{ };
        Color m_BackgroundColor{ };

        TTF_Font* m_ScoreFont{};

        bool m_Visible{ false };
    };
}
