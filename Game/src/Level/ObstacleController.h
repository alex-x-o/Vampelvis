#pragma once

namespace Game
{	
	struct Level;

	class ObstacleController
	{
	public:
		void GenerateObstacles(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_, float boundary_);
		void GenerateLevelBoundary(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_);
		void RemovePastObstacles(Engine::EntityManager* entityManager_, float boundary);

	private:
		void CreateFloorObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h);
		void CreateCeilingObstacle(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x, float y, float w, float h);
		
	};
}