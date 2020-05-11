#pragma once

namespace Game
{
	struct Level;

	class WallController
	{

	public:
		void Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_);
		void MoveWalls(Engine::EntityManager* entityManager_, float speed_);

	private:
		void CreateWalls(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_);
	};
}

