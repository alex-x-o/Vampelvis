#pragma once

namespace Game
{
	struct Level;

	class EnemyController
	{
	public:
		void GenerateEnemies(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_, float boundary_);
		void MoveEnemies(Engine::EntityManager* entityManager_, float speed_);
		
		void RemovePastEnemies(Engine::EntityManager* entityManager_, float boundary_);
		
	private:
		void AnimateEnemy(Engine::SpriteComponent* sprite_);
	};
}