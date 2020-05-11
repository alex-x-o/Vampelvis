#pragma once

namespace Game
{
    struct Level
    {
        int levelId;
        float levelStart{ 0 };
        float levelLength{ 12000  };

        float obstacleMinDistance{ 300 };
        float obstacleLastPosition{ 0 };

        float enemyMinDistance{ 300 };
        float enemyLastPosition{ 0 };
        int enemySpawnChance{ 5 }; // 1 in x

        bool spawnBats{ true };
        bool spawnGhosts{ true };

        float pickupMinDistance{ 300 };
        float pickupLastPosition{ 0 };
        int pickupSpawnChance{ 5 }; // 1 in x

        Level(int levelId_) :levelId(levelId_) {}
    };

}