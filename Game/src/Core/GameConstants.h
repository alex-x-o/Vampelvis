#pragma once

#include "precomp.h"

namespace GameConstants
{
	const std::string GAME_TITLE = "Vampelvis";
	const std::string GAME_ICON = "Textures/testTubeRed.png";

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;

	const int WALL_HEIGHT = 30;
	const int LEVEL_BOUNDARY_WIDTH = 100;

	const int PLAYER_WIDTH = 80;
	const int PLAYER_HEIGHT = 100;

	const int BAT_WIDTH = 40;
	const int BAT_HEIGHT = 40;
	
	const float IMMORTALITY_DURATION = 240.f;
	const float BATMODE_DURATION = 1200.f;

	const int IMMORTAITY_PICKUP_CHANCE = 1;
	const int BATMODE_PICKUP_CHANCE = 4;

	const int MAX_VIALS_IN_INVENTORY = 3;

	const float MAX_SPEED_COEF = 4.5f;
	const float SMOKE_DURATION = 14.f;
}

namespace Game
{
	enum Powerup {
		Immortality,
		BatMode
	};

	enum Levels {
		LVL_CASTLE = 1,
		LVL_CAVE
	};

	enum TextureCategories {
		TEX_PLAYER,
		TEX_PICKUP,
		TEX_CEILING,
		TEX_FLOOR,
		TEX_BACKGROUND,
		TEX_WALL,
		TEX_ENEMY
	};

}
