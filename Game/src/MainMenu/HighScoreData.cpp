#include "HighScoreData.h"


namespace Game
{
	HighScoreData* scoreData = HighScoreData::GetData();
	HighScoreData* HighScoreData::instance = nullptr;

	HighScoreData::HighScoreData()
	{
		// Get current High score from file
		if (std::filesystem::exists(m_FileName))
		{
			LoadHighScore();
		}
		else
		{
			CreateHighScoreData();
		}
	}

	std::map<std::string, int> HighScoreData::GetHallOfFame()
	{
		std::ifstream inputFile(m_FileName);
		json scoreObject = json::parse(inputFile);

		return scoreObject["hallOfFame"];
	}

	std::map<int, std::string, std::greater<int>> HighScoreData::GetSortedHallOfFame()
	{
		auto hallOfFame = scoreData->GetHallOfFame();

		std::map<int, std::string, std::greater<int>> sortedMap;

		for (auto& it : hallOfFame)
			sortedMap[it.second] = it.first;

		return sortedMap;
	}
	
	void HighScoreData::SetHighScore(int score_)
	{
		m_HighScore = score_;
		StoreHighScore(score_);
	}

	void HighScoreData::LoadHighScore()
	{
		std::ifstream inputFile(m_FileName);
		json scoreObject = json::parse(inputFile);
		m_HighScore = scoreObject.at("score");
	}

	void HighScoreData::StoreHighScore(int value_)
	{
		std::ifstream inputFile(m_FileName);
		nlohmann::json scoreObject;
		inputFile >> scoreObject;

		scoreObject["score"] = value_;

		// Check if player is better than famous vampires
		std::map<int, std::string> sortedMap;

		for (auto& it : GetHallOfFame())
			sortedMap[it.second] = it.first;

		if (value_ > sortedMap.begin()->first)
		{
			int id = scoreObject["numOfFamousPlayers"];
			scoreObject["hallOfFame"]["Vampelvis" + std::to_string(++id)] = value_;
			
			scoreObject["hallOfFame"].erase(sortedMap.begin()->second);

			scoreObject["numOfFamousPlayers"] = id;
		}

		std::ofstream outputFile(m_FileName);
		outputFile << scoreObject;
	}

	void HighScoreData::CreateHighScoreData()
	{
		json scoreObject;
		scoreObject["score"] = 0;
		scoreObject["numOfFamousPlayers"] = 0;
		scoreObject["hallOfFame"] = { {"Dracula", 1000},
									  {"Sava Savanovic", 700},
									  {"Damon Salvatore", 3},
									  {"Edward Cullen", 1} };

		std::ofstream output(m_FileName);
		output << scoreObject.dump();
	}
}