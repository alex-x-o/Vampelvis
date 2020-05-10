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

		int GetHighScore() const { return m_HighScore; }

		void SetHighScore(int score_) 
		{ 
			m_HighScore = score_; 
			StoreHighScore("score", score_);
		}

		void LoadHighScore() 
		{
			std::ifstream inputFile(fileName);
			json scoreObject = json::parse(inputFile);
			m_HighScore = scoreObject.at("score");
		}

		void StoreHighScore(std::string key_, int value_)
		{
			std::ifstream inputFile(fileName);
			nlohmann::json scoreObject;
			inputFile >> scoreObject;

			scoreObject[key_] = value_;

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
			std::unordered_map<std::string, int> famousVampires;
			famousVampires.insert(std::pair<std::string, int>("Dracula", 1000));
			famousVampires.insert(std::pair<std::string, int>("Sava Savanovic", 700));
			famousVampires.insert(std::pair<std::string, int>("Damon Salvatore", 500));
			famousVampires.insert(std::pair<std::string, int>("Edward Cullen", 300));
			famousVampires.insert(std::pair<std::string, int>("Barnabas Collins", 100));
			famousVampires.insert(std::pair<std::string, int>("score", 0));

			json scoreObject(famousVampires);

			std::ofstream output(fileName);
			output << scoreObject; 
		}

		int m_HighScore{ 0 };
		std::string fileName{ "score.json" };
	};
		
	extern HighScoreData scoreData = HighScoreData::GetData();
}