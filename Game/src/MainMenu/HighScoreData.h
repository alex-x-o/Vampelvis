#pragma once
#include "precomp.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;


namespace Game
{
	class HighScoreData
	{
	public:
		static HighScoreData* GetData()
		{
			if (!instance)
			{
				instance = new HighScoreData();
			}

			return instance;
		}

		int GetHighScore() const { return m_HighScore; }
		std::map<std::string, int> GetHallOfFame();
		std::map<int, std::string, std::greater<int>> GetSortedHallOfFame();

		void SetHighScore(int score_);

		void LoadHighScore();
		void StoreHighScore(int value_);

		HighScoreData(const HighScoreData& other) = delete;
		HighScoreData& operator=(HighScoreData& other) = delete;
	private:
		HighScoreData();
		static HighScoreData* instance;

		void CreateHighScoreData();

		int m_HighScore{ 0 };
		std::string fileName{ "score.json" };
	};

	extern HighScoreData* scoreData;
}