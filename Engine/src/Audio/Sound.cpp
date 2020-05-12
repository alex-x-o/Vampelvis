#include "precomp.h"
#include "Sound.h"

#include <SDL_mixer.h>

bool Engine::Sound::LoadSound(std::string soundName_, std::string path_)
{
    if (m_Sound != nullptr)
    {
        LOG_WARNING("Overwriting already loaded sound with: {}", path_);
        Mix_FreeChunk(m_Sound);
    }
    
    m_Sound = Mix_LoadWAV(path_.c_str());

    if (m_Sound == nullptr)
    {
        LOG_ERROR("Unable to load music: {}, Mix_LoadMUS returned error {}", path_, Mix_GetError());
        return false;
    }

    m_SoundName = soundName_;

    return true;
}

Engine::Sound::Sound(std::string soundName_, std::string path_)
{
    LoadSound(soundName_, path_);
}

Engine::Sound::~Sound()
{
    if (m_Sound != nullptr)
    {
        Mix_FreeChunk(m_Sound);
    }
}
