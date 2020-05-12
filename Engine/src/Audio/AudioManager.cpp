#include "precomp.h"
#include "AudioManager.h"
#include <SDL_mixer.h>
#include "Music.h"

bool Engine::AudioManager::Init()
{
	LOG_INFO("Initializing Audio Manager");

	if (-1 == Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096))
	{
		LOG_WARNING(fmt::format("SDL_mixer initialization failed! {}", Mix_GetError()));
		return false;
	}

	m_Sounds.reserve(5);

	return true;
}

void Engine::AudioManager::Shutdown()
{
	Mix_CloseAudio();

	m_Sounds.clear();
}

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

void Engine::AudioManager::PauseMusic()
{
	Mix_PauseMusic();
}

void Engine::AudioManager::ResumePlayingMusic()
{
	Mix_ResumeMusic();
}

void Engine::AudioManager::ToggleMusicPlaying()
{
	if (m_IsPlaying) PauseMusic();
	else ResumePlayingMusic();

	m_IsPlaying = !m_IsPlaying;
}

void Engine::AudioManager::LoadSound(std::string soundName_, std::string path_)
{
	m_Sounds.emplace_back(soundName_, path_);
}

void Engine::AudioManager::PlaySound(std::string soundName_)
{
	auto foundPlace = std::find_if(std::begin(m_Sounds), std::end(m_Sounds), 
					  [soundName_](Sound& sound) {return sound.m_SoundName == soundName_; });

	if (foundPlace == std::end(m_Sounds))
	{
		LOG_INFO(fmt::format("There is not such sound loaded: {}", soundName_));
		return;
	}

	int channelNo = Mix_PlayChannel(-1, foundPlace->m_Sound, 0);
	if (-1 == channelNo)
	{
		LOG_WARNING(fmt::format("Play {} failed! ", Mix_GetError()));
	}

	Mix_Volume(-1, 128);
}