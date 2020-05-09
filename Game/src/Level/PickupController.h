#pragma once
#include "Core/GameConstants.h"

namespace Game 
{
	class PickupController
	{
	public:
		void GeneratePickups(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary_, float minObstDist_, float lastObstPos_);
		void RemovePastPickups(Engine::EntityManager* entityManager_, float boundary);


	private:
		float m_LastPickupPos{ 0 };
		float m_MinPickupDist{ 300 };

		Powerup ChooseRandomPowerup();
		void CreatePowerupPickup(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x_, float y_, Game::Powerup type_);

	};
}


