#pragma once

namespace Game
{
	class EnemyController
	{
	public:
		void GenerateEnemies(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary_);
		void MoveEnemies(Engine::EntityManager* entityManager_, float speed_);
		
		void RemovePastEnemies(Engine::EntityManager* entityManager_, float boundary_);
	};
}