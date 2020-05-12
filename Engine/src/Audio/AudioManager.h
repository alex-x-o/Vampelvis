#pragma once
#include "Music.h"

namespace Engine
{

	class AudioManager
	{
	public:
		bool Init();
		void Shutdown();

		bool LoadMusic(std::string path_);
		void PlayMusic();
		void StopMusic();

		void ToggleMusicPlaying();
		bool m_IsPlaying{ false };

		AudioManager() = default;
	private:
		std::unique_ptr<Music> m_MusicTrack;

		AudioManager(const AudioManager& other) = delete;
		AudioManager& operator=(AudioManager& other) = delete;
	};

}


