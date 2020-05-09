#pragma once

namespace GameConstants
{
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;

	const int WALL_HEIGHT = 30;

	const int PLAYER_WIDTH = 80;
	const int PLAYER_HEIGHT = 100;

	const int BAT_WIDTH = 40;
	const int BAT_HEIGHT = 40;
	
	const float IMMORTALITY_DURATION = 240.f;
	const float BATMODE_DURATION = 1200.f;

	const int IMMORTAITY_PICKUP_CHANCE = 1;
	const int BATMODE_PICKUP_CHANCE = 4;
}

namespace Game
{
	enum Powerup {
		Immortality,
		BatMode
	};

}