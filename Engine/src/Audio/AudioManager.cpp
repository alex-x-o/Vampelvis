#include "precomp.h"
#include "AudioManager.h"
#include <SDL_mixer.h>
#include "Music.h"

bool Engine::AudioManager::LoadMusic(std::string path_)
{
	m_MusicTrack = std::make_unique<Music>(path_);
	return m_MusicTrack->m_Music != nullptr;
}

void Engine::AudioManager::PlayMusic()
{
	if (Mix_FadeInMusic(m_MusicTrack->m_Music, -1, 2000) == -1) {
		printf("Mix_FadeInMusic: %s\n", Mix_GetError());
	}
}

void Engine::AudioManager::StopMusic()
{
	Mix_HaltMusic();
}
