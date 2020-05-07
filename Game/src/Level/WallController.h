#pragma once

namespace Game
{
	class WallController
	{

	public:
		void Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void MoveWalls(Engine::EntityManager* entityManager_, float speed_);

	private:
		void CreateWalls(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
	};
}

