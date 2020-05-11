#include "HighScoreData.h"


namespace Game
{
	HighScoreData* scoreData = HighScoreData::GetData();
	HighScoreData* HighScoreData::instance = nullptr;

	HighScoreData::HighScoreData()
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

	std::map<std::string, int> HighScoreData::GetHallOfFame()
	{
		std::ifstream inputFile(fileName);
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
		std::ifstream inputFile(fileName);
		json scoreObject = json::parse(inputFile);
		m_HighScore = scoreObject.at("score");
	}

	void HighScoreData::StoreHighScore(int value_)
	{
		std::ifstream inputFile(fileName);
		nlohmann::json scoreObject;
		inputFile >> scoreObject;

		scoreObject["score"] = value_;

		std::ofstream outputFile(fileName);
		outputFile << scoreObject;
	}

	void HighScoreData::CreateHighScoreData()
	{
		json scoreObject;
		scoreObject["score"] = 0;
		scoreObject["hallOfFame"] = { {"Dracula", 1000},
									  {"Sava Savanovic", 700},
									  {"Damon Salvatore", 500},
									  {"Edward Cullen", 300} };

		std::ofstream output(fileName);
		output << scoreObject.dump();
	}
}