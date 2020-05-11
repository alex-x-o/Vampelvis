#pragma once
#include "Core/GameConstants.h"

namespace Game
{
	struct Level;

	class PickupController
	{
	public:
		void GeneratePickups(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Level* level_, float boundary_);
		void RemovePastPickups(Engine::EntityManager* entityManager_, float boundary);

	private:
		Powerup ChooseRandomPowerup();
		void CreatePowerupPickup(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x_, float y_, Game::Powerup type_);

	};
}


