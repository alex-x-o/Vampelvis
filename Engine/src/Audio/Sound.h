#pragma once

struct Mix_Chunk;

namespace Engine
{
	struct Sound
	{
		Mix_Chunk* m_Sound{};
		std::string m_SoundName;

		bool LoadSound(std::string soundName_, std::string path_);

		Sound() = default;
		Sound(std::string soundName_, std::string path_);
		~Sound();
	};
}