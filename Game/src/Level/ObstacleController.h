#pragma once

namespace Game
{
	class ObstacleController
	{
	public:
		void GenerateObstacles(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary);
		void RemovePastObstacles(Engine::EntityManager* entityManager_, float boundary);

	private:
		float m_LastObstaclePos{ 0 };
		float m_MinObstacleDist{ 300 };

		void CreateFloorObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h);
		void CreateCeilingObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h);

		
	};
}