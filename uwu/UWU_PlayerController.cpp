#include "UWU_PlayerController.h"
#include "UWU_InputManager.h"
#include "UWU_SpriteRenderer.h"
#include "UWU_SpriteAnimator.h"
#include "UWU_HurtBody.h"
#include "UWU_TextManager.hpp"
#include "UWU_Audio.hpp"
#include "UWU_WindowOwner.h"
#include "UWU_WindowManager.hpp"
#include "UWU_Tweening.hpp"
#include "UWU_Health.h"

ECS::UWU_PlayerController::UWU_PlayerController(float movementSpeed)
{
	_movementSpeed = movementSpeed;
	_directionFacing = 1;
	_currentItem = NONE;
	_state = Idle;

}

void ECS::UWU_PlayerController::Update(float deltaTime)
{
	State lastState = _state;
	float direction = UWU_InputManager::getInstance().GetAxisActionState(A_XMovement);

	if (direction != 0 && _dashTimer.GetDeltaTicks() > 200) {
		_directionFacing = _directionFacing = (direction < 0) ? -1 : 1;;
	}

	_rigidBody->velocity.x = direction * _movementSpeed * deltaTime;

#pragma region Dash

	if (UWU_InputManager::getInstance().GetActionStateOnce(B_Dash) && _dashTimer.GetDeltaTicks() > 800) {
		UWU_Audio::PlaySoundEffect("Dash");
		_state = Dashing;
		_dashTimer.Reset();
	}
	if (_dashTimer.GetDeltaTicks() < 200) {
		_rigidBody->velocity.x = 1000 * deltaTime * _directionFacing;
		_rigidBody->velocity.y = 0;
	}
	else if (_state == Dashing) {
		_state = Idle;
	}

#pragma endregion

#pragma region Jump
	if (_rigidBody->isGrounded) {
		if (UWU_InputManager::getInstance().GetActionStateOnce(B_Jump)) {
			_jumpTimer.Reset();
			_rigidBody->isGrounded = false;
			UWU_Audio::PlaySoundEffect("Jump");
		}
	}
	if (UWU_InputManager::getInstance().GetActionState(B_Jump) && _jumpTimer.GetDeltaTicks() < 300) {
		_rigidBody->velocity.y = -600 * deltaTime;
	}
#pragma endregion

	if (UWU_InputManager::getInstance().GetActionStateOnce(B_Attack) && _attackTimer.GetDeltaTicks() > 1000) {
		UWU_WindowOwner* windowOwner;
		switch (_currentItem)
		{
		case Normal:
			LongSwing();
			windowOwner = entity->GetComponent<UWU_WindowOwner>();
			windowOwner->GetWindow()->SetWindowSize(windowOwner->GetWindow()->GetWindowSize() - int2(20 * RenderSizeMultiplier, 20 * RenderSizeMultiplier));
			UWU_Audio::PlaySoundEffect("LongSwing");
			break;
		case Long:
			LongLongSwing();
			windowOwner = entity->GetComponent<UWU_WindowOwner>();
			windowOwner->GetWindow()->SetWindowSize(windowOwner->GetWindow()->GetWindowSize() - int2(20 * RenderSizeMultiplier, 20 * RenderSizeMultiplier));
			UWU_Audio::PlaySoundEffect("LongSwing");
			break;
		case NONE:
			BaseSwing();
			UWU_Audio::PlaySoundEffect("ShortSwing");
			break;
		}
		_currentItem = NONE;
		_glow->GetComponent<UWU_SpriteRenderer>()->enabled = false;
		_attackTimer.Reset();
	}
	if (_currentItem == ItemType::Heal) {
		entity->GetComponent<UWU_Health>()->Heal(1);
		_currentItem = ItemType::NONE;
	}
	_glow->GetComponent<UWU_Transform>()->position = _transform->position;

	if (_state != Dashing) {
		if (_rigidBody->isGrounded) {
			if (direction == 0) {
				_state = Idle;
			}
			else {
				_state = Running;
			}
		}
		else {
			_state = Jumping;
		}
	}
	if (lastState != _state)
		entity->GetComponent<UWU_SpriteAnimator>()->SetAnimation(stateNames[_state]);
}
void ECS::UWU_PlayerController::LateUpdate(float) {

}
void ECS::UWU_PlayerController::Render(UWU_Window* window)
{
	float2 pos = window->GetInversePosition(_transform->position);
	UWU_TextManager::Draw(window->GetRenderer(), "Time left: " + std::to_string(MaxTimeLeft - GameTimer.GetDeltaTicks<std::chrono::seconds>()) + "\nScore: " + std::to_string(Score), pos.x, pos.y + _transform->size.y + (2 * RenderSizeMultiplier));

	if (MaxTimeLeft - GameTimer.GetDeltaTicks<std::chrono::seconds>() <= 0) {
		entity->GetComponent<UWU_Health>()->TakeDamage(3);
	}

}

void ECS::UWU_PlayerController::Start()
{
	_rigidBody = entity->GetComponent<UWU_Rigidbody>();
	_transform = entity->GetComponent<UWU_Transform>();
	_glow = new UWU_Entity(entity->world);
	_glow->AddComponent<UWU_Transform>(_transform->position / RenderSizeMultiplier, _transform->size / RenderSizeMultiplier);
	_glow->AddComponent<UWU_SpriteRenderer>()->enabled = false;
	_glow->AddComponent<UWU_SpriteAnimator>("player_glow", true);
	entity->world->AddEntity(_glow);
}

void ECS::UWU_PlayerController::OnCollision(UWU_Entity* other)
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
		UWU_WindowOwner* windowOwner = entity->GetComponent<UWU_WindowOwner>();
		windowOwner->GetWindow()->SetWindowSize(windowOwner->GetWindow()->GetWindowSize() + int2(20 * RenderSizeMultiplier, 20 * RenderSizeMultiplier));
	}
	else if (other->GetTag() == "Long") {
		_currentItem = ItemType::Long;
		entity->world->RemoveEntity(other);

		UWU_SpriteRenderer* glowRenderer = _glow->GetComponent<UWU_SpriteRenderer>();
		glowRenderer->enabled = true;
		glowRenderer->SetTint(SDL_Colour{ 0, 0, 255, 255 });

		UWU_Audio::PlaySoundEffect("PlayerPickup");
		UWU_WindowOwner* windowOwner = entity->GetComponent<UWU_WindowOwner>();
		windowOwner->GetWindow()->SetWindowSize(windowOwner->GetWindow()->GetWindowSize() + int2(20 * RenderSizeMultiplier, 20 * RenderSizeMultiplier));
	}
	else if (other->GetTag() == "Heal") {
		_currentItem = ItemType::Heal;

		other->GetComponent<UWU_Rigidbody>()->enabled = false;
		entity->world->RemoveEntity(other);

		UWU_Audio::PlaySoundEffect("PlayerPickup");
	}
}

void ECS::UWU_PlayerController::BaseSwing()
{
	float xMod;
	if (_directionFacing > 0) {
		xMod = _transform->size.x;
	}
	else {
		xMod = -30 * RenderSizeMultiplier;
	}
	UWU_Entity* newEntity = new UWU_Entity(entity->world);
	newEntity->AddComponent<UWU_Transform>((_transform->position + float2(xMod, 0)) / RenderSizeMultiplier, float2(30, 20));
	newEntity->AddComponent<UWU_SpriteRenderer>()->SetFlipped((SDL_RendererFlip)(_directionFacing > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE | (rand() % 2 == 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL)));
	newEntity->AddComponent<UWU_SpriteAnimator>("base_swing", false);
	newEntity->AddComponent<UWU_Rigidbody>(float2(0, 0), 1, true, true);
	newEntity->AddComponent<UWU_HurtBody>(1, std::vector<UWU_Entity*>{ entity });
	entity->world->AddEntity(newEntity);
}

void ECS::UWU_PlayerController::LongSwing()
{
	float xMod;
	if (_directionFacing > 0) {
		xMod = _transform->size.x;
	}
	else {
		xMod = -50 * RenderSizeMultiplier;
	}
	UWU_Entity* newEntity = new UWU_Entity(entity->world);
	newEntity->AddComponent<UWU_Transform>((_transform->position + float2(xMod, 0)) / RenderSizeMultiplier, float2(50, 20));
	newEntity->AddComponent<UWU_SpriteRenderer>()->SetFlipped((SDL_RendererFlip)(_directionFacing > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE | (rand() % 2 == 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL)));
	newEntity->AddComponent<UWU_SpriteAnimator>("long_swing", false);
	newEntity->AddComponent<UWU_Rigidbody>(float2(0, 0), 1, true, true);
	newEntity->AddComponent<UWU_HurtBody>(2, std::vector<UWU_Entity*>{ entity });
	entity->world->AddEntity(newEntity);
}

void ECS::UWU_PlayerController::LongLongSwing()
{
	float xMod;
	if (_directionFacing > 0) {
		xMod = _transform->size.x;
	}
	else {
		xMod = -80 * RenderSizeMultiplier;
	}
	UWU_Entity* newEntity = new UWU_Entity(entity->world);
	newEntity->AddComponent<UWU_Transform>((_transform->position + float2(xMod, 0)) / RenderSizeMultiplier, float2(80, 20));
	newEntity->AddComponent<UWU_SpriteRenderer>("default", false, SDL_Colour{ 0,0,255,255 })->SetFlipped((SDL_RendererFlip)(_directionFacing > 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE | (rand() % 2 == 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL)));
	newEntity->AddComponent<UWU_SpriteAnimator>("long_swing", false);
	newEntity->AddComponent<UWU_Rigidbody>(float2(0, 0), 1, true, true);
	newEntity->AddComponent<UWU_HurtBody>(1, std::vector<UWU_Entity*>{ entity });
	entity->world->AddEntity(newEntity);
}
