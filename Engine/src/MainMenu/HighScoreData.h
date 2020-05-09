#pragma once
#include "precomp.h"

#include <nlohmann/json.hpp>

namespace
{
	class HighScoreData
	{
	public:
		static HighScoreData& GetData()
		{
			static HighScoreData data;
			return data;
		}

		int GetHighScore() const { return m_HighScore; }
		void SetHighScore(int score_) 
		{ 
			m_HighScore = score_; 
			StoreHighScore("score", score_);
		}

		void LoadHighScore() 
		{
			std::ifstream inputFile("score.json");
			nlohmann::json scoreObject = nlohmann::json::parse(inputFile);
			m_HighScore = scoreObject.at("score");
		}

		void StoreHighScore(std::string key_, int value_)
		{
			std::ofstream output("score.json");
			nlohmann::json scoreObject;
			scoreObject.emplace(key_, value_);
			output << scoreObject;
		}

		HighScoreData& operator=(HighScoreData& other) = delete;
	private:
		HighScoreData() 
		{
			// Get current High score from file
			if (std::filesystem::exists("score.json"))
			{
				LoadHighScore();
			}
			else
			{
				StoreHighScore("score", 0);
			}
		}

		int m_HighScore{ 0 };
	};
		
	extern HighScoreData scoreData = HighScoreData::GetData();
}