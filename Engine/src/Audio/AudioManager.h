#pragma once
#include "Music.h"
#include "Sound.h"

namespace Engine
{

	class AudioManager
	{
	public:
		bool Init();
		void Shutdown();

		bool LoadMusic(std::string path_);
		void PlayMusic();
		void PauseMusic();
		void ResumePlayingMusic();

		void ToggleMusicPlaying();
		bool m_IsPlaying{ true };

		void LoadSound(std::string soundName_, std::string path_);
		void PlaySound(std::string soundName_);

		AudioManager() = default;
	private:
		std::unique_ptr<Music> m_MusicTrack;
		std::vector<Sound> m_Sounds;

		AudioManager(const AudioManager& other) = delete;
		AudioManager& operator=(AudioManager& other) = delete;
	};

}


