#pragma once
#include "precomp.h"
#include "Core\GameConstants.h"


namespace Game
{
	namespace MenuLabelsData
	{
		static std::vector<std::string> BEGINING_LABELS = { GameConstants::GAME_TITLE, 
															"Best score: " + std::to_string(0),
															"Hall of Fame", "Sound : on", "How to play", 
															"About us", "Press ENTER to open or SPACE to Play"
														  };

		static std::vector<std::string> GAMEOVER_LABELS = { GameConstants::GAME_TITLE, " " ,"GAME OVER",
															"Your score: " + std::to_string(0),
															"Press SPACE to return to Main Menu" };

		static std::vector<std::string> INSTRUCTION_LABELS = {  BEGINING_LABELS[4], 
																"You have awakened from your slumber!",
																"Fly through the SPACE, SHIFT into a terrifying",
																"creature of the night by drinking Red Blood Vials or put",
																"your death under CONTROL by drinking Blue Blood",
																"Vials and becoming immune to damage for a short period.",
																"Press ESCAPE to return" };

		static std::vector<std::string> ABOUTUS_LABELS = {  BEGINING_LABELS[5], " ", " ", " ",
															"Scan it to find out more!",
															"Press ESCAPE to return" };
	}
}