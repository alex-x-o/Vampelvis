#pragma once
#include "precomp.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

		std::map<std::string, int> GetHallOfFame()
		{
			std::ifstream inputFile(fileName);
			json scoreObject = json::parse(inputFile);

			return scoreObject["hallOfFame"];
		}

		int GetHighScore() const { return m_HighScore; }

		void SetHighScore(int score_) 
		{ 
			m_HighScore = score_; 
			StoreHighScore(score_);
		}

		void LoadHighScore() 
		{
			std::ifstream inputFile(fileName);
			json scoreObject = json::parse(inputFile);
			m_HighScore = scoreObject.at("score");
		}

		void StoreHighScore(int value_)
		{
			std::ifstream inputFile(fileName);
			nlohmann::json scoreObject;
			inputFile >> scoreObject;

			scoreObject["score"] = value_;

			std::ofstream outputFile(fileName);
			outputFile << scoreObject;
		}

		HighScoreData& operator=(HighScoreData& other) = delete;
	private:
		HighScoreData() 
		{
			// Get current High score from file
			if (std::filesystem::exists(fileName))
			{
				LoadHighScore();
			}
			else
			{
				CreateHighScoreData();
			}
		}

		void CreateHighScoreData()
		{
			json scoreObject;
			scoreObject["score"] = 0;
			scoreObject["hallOfFame"] = {
											{"Dracula", 1000},
											{"Sava Savanovic", 700},
											{"Damon Salvatore", 500},
											{"Edward Cullen", 300}
										};

			std::ofstream output(fileName);
			output << scoreObject.dump(); 
		}

		int m_HighScore{ 0 };
		std::string fileName{ "score.json" };
	};
		
	extern HighScoreData scoreData = HighScoreData::GetData();
}