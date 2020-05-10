#include "precomp.h"
#include "PickupController.h"

#include "Core/GameConstants.h"
#include "Entities/GameComponents.h"
#include <random>

namespace Game 
{
	void PickupController::GeneratePickups(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, float boundary_,
										   float minObstDist_, float lastObstPos_)
	{
		if (boundary_ < m_LastPickupPos + m_MinPickupDist)
			return;

		float pickupPosX = lastObstPos_ + minObstDist_ / 2;
		m_LastPickupPos = pickupPosX;

		std::random_device rd;
		std::uniform_int_distribution<> rollForPickup(0, 1);

		if (rollForPickup(rd) == 0)
		{
			float space = static_cast<int>(GameConstants::SCREEN_HEIGHT / 2 - GameConstants::WALL_HEIGHT);
			std::uniform_int_distribution<> yPos(-space, space);

			float pickupPosY = yPos(rd);

			Game::Powerup powerup = ChooseRandomPowerup();
			Engine::Texture* pickupTexture;
			if (powerup == Game::Immortality)
				pickupTexture = textureManager_->GetCommonTexture(Game::TEX_PICKUP, "immortality");
			else 
				pickupTexture = textureManager_->GetCommonTexture(Game::TEX_PICKUP, "batMode");

			CreatePowerupPickup(entityManager_, pickupTexture, pickupPosX, pickupPosY, powerup);
		}	

	}
	void PickupController::RemovePastPickups(Engine::EntityManager* entityManager_, float boundary)
	{
		auto pickups = entityManager_->GetAllEntitiesWithComponent<Engine::PickupComponent>();

		for (auto& pickup : pickups)
		{
			auto pickupTransformer = pickup->GetComponent<Engine::TransformComponent>();
			auto picupComponent = pickup->GetComponent<Engine::PickupComponent>();
			float xPos = pickupTransformer->m_Position.x;
			float xWidth = pickupTransformer->m_Size.x;

			if (xWidth / 2 + xPos < boundary || picupComponent->m_PickedUp)
			{
				entityManager_->RemoveEntityById(pickup->GetId());
			}
		}
	}

	void PickupController::CreatePowerupPickup(Engine::EntityManager* entityManager_, Engine::Texture* texture_, float x_, float y_, Game::Powerup type_)
	{
		vec2 pickupSize = texture_->GetTextureSize();

		auto pickup = std::make_unique<Engine::Entity>();
		pickup->AddComponent<Engine::TransformComponent>(x_, y_, pickupSize.x, pickupSize.y);
		pickup->AddComponent<Engine::CollisionComponent>(pickupSize.x, pickupSize.y);
		pickup->AddComponent<Engine::SpriteComponent>().m_Image = texture_;
		pickup->AddComponent<Engine::PickupComponent>(type_);

		entityManager_->AddEntity(std::move(pickup));
	}

	Game::Powerup PickupController::ChooseRandomPowerup()
	{
		int v = GameConstants::IMMORTAITY_PICKUP_CHANCE + GameConstants::BATMODE_PICKUP_CHANCE;

		std::random_device rd;
		std::uniform_int_distribution<> rollForPickupType(0, v);


		Game::Powerup choice;
		if (rollForPickupType(rd) < GameConstants::IMMORTAITY_PICKUP_CHANCE)
		{
			choice = Game::Immortality;
		}
		else
		{
			choice = Game::BatMode;
		}

		return choice;
	}
}