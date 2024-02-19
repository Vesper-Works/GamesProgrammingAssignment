#include "UWU_EnemyController.h"
#include "UWU_InputManager.h"
#include "UWU_SpriteRenderer.h"
#include "UWU_SpriteAnimator.h"
#include "UWU_HurtBody.h"
#include "UWU_Audio.hpp"
#include "UWU_Tweening.hpp"
#include "UWU_Health.h"

#define APPROX(a,b) std::abs(a - b) < 1.f * RenderSizeMultiplier

ECS::UWU_EnemyController::UWU_EnemyController(std::vector<int2> patrolPath, float movementSpeed)
{
	_movementSpeed = movementSpeed;
	_patrolPath = patrolPath;
	_currentNodeIndex = 0;
	_currentItem = ItemType::NONE;
}

ECS::UWU_EnemyController::~UWU_EnemyController()
{
	entity->world->RemoveEntity(_glow);
}

void ECS::UWU_EnemyController::Update(float deltaTime)
{
	if (APPROX(_transform->position.x, _patrolPath[_currentNodeIndex].x)) {
		if (++_currentNodeIndex >= _patrolPath.size()) {
			_currentNodeIndex = 0;
		}
	}

	float direction = _transform->position.x > _patrolPath[_currentNodeIndex].x ? -1 : 1;

	_rigidBody->velocity.x = direction * _movementSpeed * deltaTime;

	if (_attackTimer.GetDeltaTicks() > 1200) {
		UWU_Tweening::DoAfter([this]() {
			switch (_currentItem)
			{
			case Normal:
				LongSwing();
				UWU_Audio::PlaySoundEffect("EnemyLongSwing", 0.5f);
				break;
			case Long:
				LongLongSwing();
				UWU_Audio::PlaySoundEffect("EnemyLongSwing", 0.5f);
				break;
			case NONE:
				BaseSwing();
				UWU_Audio::PlaySoundEffect("EnemyShortSwing", 0.5f);
				break;
			}
			}, rand() % 200);

		_attackTimer.Reset();
	}
	_glow->GetComponent<UWU_Transform>()->position = _transform->position;
	if (_currentItem == ItemType::Heal) {
		entity->GetComponent<UWU_Health>()->Heal(1);
		_currentItem = NONE;
	}
}

void ECS::UWU_EnemyController::LateUpdate(float)
{
}

void ECS::UWU_EnemyController::Render(UWU_Window*)
{
}

void ECS::UWU_EnemyController::Start()
{
	_rigidBody = entity->GetComponent<UWU_Rigidbody>();
	_transform = entity->GetComponent<UWU_Transform>();
	_glow = new UWU_Entity(entity->world);
	_glow->AddComponent<UWU_Transform>(_transform->position / RenderSizeMultiplier, _transform->size / RenderSizeMultiplier);
	_glow->AddComponent<UWU_SpriteRenderer>()->enabled = false;
	_glow->AddComponent<UWU_SpriteAnimator>("player_glow", true);
	entity->world->AddEntity(_glow);
}

void ECS::UWU_EnemyController::OnCollision(UWU_Entity* other)
{
	if (_currentItem != ItemType::NONE) {
		return;
	}
	if (other->GetTag() == "Normal") {
		_currentItem = ItemType::Normal;
		entity->world->RemoveEntity(other);

		UWU_SpriteRenderer* glowRenderer = _glow->GetComponent<UWU_SpriteRenderer>();
		glowRenderer->enabled = true;
		glowRenderer->SetTint(SDL_Colour{ 255, 255, 255, 255 });

		UWU_Audio::PlaySoundEffect("PlayerPickup");
	}
	else if (other->GetTag() == "Long") {
		_currentItem = ItemType::Long;
		entity->world->RemoveEntity(other);

		UWU_SpriteRenderer* glowRenderer = _glow->GetComponent<UWU_SpriteRenderer>();
		glowRenderer->enabled = true;
		glowRenderer->SetTint(SDL_Colour{ 0, 0, 255, 255 });

		UWU_Audio::PlaySoundEffect("PlayerPickup");
	}
	else if (other->GetTag() == "Heal") {
		_currentItem = ItemType::Heal;

		other->GetComponent<UWU_Rigidbody>()->enabled = false;
		entity->world->RemoveEntity(other);

		UWU_Audio::PlaySoundEffect("PlayerPickup");
	}
}

void ECS::UWU_EnemyController::BaseSwing()
{
	float rightSwingX = _transform->size.x;
	float leftSwingX = -30 * RenderSizeMultiplier;

	UWU_Entity* rightSwing = new UWU_Entity(entity->world);
	rightSwing->AddComponent<UWU_Transform>((_transform->position + float2(rightSwingX, 0)) / RenderSizeMultiplier, float2(30, 20));
	rightSwing->AddComponent<UWU_SpriteRenderer>()->SetFlipped((SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | (rand() % 2 == 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL)));
	rightSwing->AddComponent<UWU_SpriteAnimator>("base_swing", false);
	rightSwing->AddComponent<UWU_Rigidbody>(float2(0, 0), 1, true, true);
	rightSwing->AddComponent<UWU_HurtBody>(1, std::vector<UWU_Entity*>{ entity });
	entity->world->AddEntity(rightSwing);

	UWU_Entity* leftSwing = new UWU_Entity(entity->world);;
	leftSwing->AddComponent<UWU_Transform>((_transform->position + float2(leftSwingX, 0)) / RenderSizeMultiplier, float2(30, 20));
	leftSwing->AddComponent<UWU_SpriteRenderer>()->SetFlipped((SDL_RendererFlip)(SDL_FLIP_NONE | (rand() % 2 == 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL)));
	leftSwing->AddComponent<UWU_SpriteAnimator>("base_swing", false);
	leftSwing->AddComponent<UWU_Rigidbody>(float2(0, 0), 1, true, true);
	leftSwing->AddComponent<UWU_HurtBody>(1, std::vector<UWU_Entity*>{ entity });
	entity->world->AddEntity(leftSwing);
}

void ECS::UWU_EnemyController::LongSwing()
{
	float rightSwingX = _transform->size.x;
	float leftSwingX = -50 * RenderSizeMultiplier;

	UWU_Entity* rightSwing = new UWU_Entity(entity->world);
	rightSwing->AddComponent<UWU_Transform>((_transform->position + float2(rightSwingX, 0)) / RenderSizeMultiplier, float2(50, 20));
	rightSwing->AddComponent<UWU_SpriteRenderer>()->SetFlipped((SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | (rand() % 2 == 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL)));
	rightSwing->AddComponent<UWU_SpriteAnimator>("long_swing", false);
	rightSwing->AddComponent<UWU_Rigidbody>(float2(0, 0), 1, true, true);
	rightSwing->AddComponent<UWU_HurtBody>(2, std::vector<UWU_Entity*>{ entity });
	entity->world->AddEntity(rightSwing);

	UWU_Entity* leftSwing = new UWU_Entity(entity->world);
	leftSwing->AddComponent<UWU_Transform>((_transform->position + float2(leftSwingX, 0)) / RenderSizeMultiplier, float2(50, 20));
	leftSwing->AddComponent<UWU_SpriteRenderer>()->SetFlipped((SDL_RendererFlip)(SDL_FLIP_NONE | (rand() % 2 == 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL)));
	leftSwing->AddComponent<UWU_SpriteAnimator>("long_swing", false);
	leftSwing->AddComponent<UWU_Rigidbody>(float2(0, 0), 1, true, true);
	leftSwing->AddComponent<UWU_HurtBody>(2, std::vector<UWU_Entity*>{ entity });
	entity->world->AddEntity(leftSwing);
}

void ECS::UWU_EnemyController::LongLongSwing()
{
	float rightSwingX = _transform->size.x;
	float leftSwingX = -80 * RenderSizeMultiplier;

	UWU_Entity* rightSwing = new UWU_Entity(entity->world);
	rightSwing->AddComponent<UWU_Transform>((_transform->position + float2(rightSwingX, 0)) / RenderSizeMultiplier, float2(80, 20));
	rightSwing->AddComponent<UWU_SpriteRenderer>("default", false, SDL_Colour{ 0,0,255,255 })->SetFlipped((SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | (rand() % 2 == 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL)));
	rightSwing->AddComponent<UWU_SpriteAnimator>("long_swing", false);
	rightSwing->AddComponent<UWU_Rigidbody>(float2(0, 0), 1, true, true);
	rightSwing->AddComponent<UWU_HurtBody>(1, std::vector<UWU_Entity*>{ entity });
	entity->world->AddEntity(rightSwing);

	UWU_Entity* leftSwing = new UWU_Entity(entity->world);
	leftSwing->AddComponent<UWU_Transform>((_transform->position + float2(leftSwingX, 0)) / RenderSizeMultiplier, float2(80, 20));
	leftSwing->AddComponent<UWU_SpriteRenderer>("default", false, SDL_Colour{ 0,0,255,255 })->SetFlipped((SDL_RendererFlip)(SDL_FLIP_NONE | (rand() % 2 == 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL)));
	leftSwing->AddComponent<UWU_SpriteAnimator>("long_swing", false);
	leftSwing->AddComponent<UWU_Rigidbody>(float2(0, 0), 1, true, true);
	leftSwing->AddComponent<UWU_HurtBody>(1, std::vector<UWU_Entity*>{ entity });
	entity->world->AddEntity(leftSwing);
}
