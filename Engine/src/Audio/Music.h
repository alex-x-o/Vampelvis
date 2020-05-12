#pragma once

typedef struct _Mix_Music Mix_Music;

namespace Engine
{
	

	struct Music
	{
		Mix_Music* m_Music{};

		bool LoadMusic(std::string path_);
		Music() = default;
		Music(std::string path_);
		~Music();

	};

}

