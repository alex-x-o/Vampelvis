#include "precomp.h"
#include "Texture.h"
#include "Render/Renderer.h"

#include <SDL.h>
#include <SDL_image.h>

namespace Engine
{
    bool Texture::LoadTexture(Renderer* renderer_, std::string path_)
    {
        if (m_Texture != nullptr)
        {
            LOG_WARNING("Overwriting already loaded texture with: {}", path_);
            SDL_DestroyTexture(m_Texture);
        }

        m_Texture = IMG_LoadTexture(renderer_->GetNativeRenderer(), path_.c_str());

        if (m_Texture == nullptr)
        {
            LOG_ERROR("Unable to load texture: {}, SDL_Image returned error {}", path_, IMG_GetError());
            return false;
        }

        return true;
    }

    vec2 Texture::GetTextureSize() {
        int width, height;
        int query = SDL_QueryTexture(m_Texture, NULL, NULL, &width, &height);

        if (query)
        {
            LOG_WARNING("Failed to get texture size");
        }

        return vec2(width, height);
    }

    Texture::Texture(Renderer* renderer_, std::string path_)
    {
        LoadTexture(renderer_, path_);
    }

    Texture::~Texture()
    {
        if (m_Texture != nullptr)
        {
            SDL_DestroyTexture(m_Texture);
        }
    }
}
