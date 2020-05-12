#include "precomp.h"
#include "Music.h"
#include <SDL_mixer.h>


namespace Engine
{
    bool Music::LoadMusic(std::string path_)
    {
        if (m_Music != nullptr)
        {
            LOG_WARNING("Overwriting already loaded music with: {}", path_);
            Mix_FreeMusic(m_Music);
        }

        m_Music = Mix_LoadMUS(path_.c_str());

        if (m_Music == nullptr)
        {
            LOG_ERROR("Unable to load music: {}, Mix_LoadMUS returned error {}", path_, Mix_GetError());
            return false;
        }

        return true;
    }


    Music::Music(std::string path_)
    {
        LoadMusic(path_);
    }

    Music::~Music()
    {
        if (m_Music != nullptr)
        {
            Mix_FreeMusic(m_Music);
        }
    }
}