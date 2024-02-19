#include "UWU_SpriteAnimator.h"
#include "UWU_TextureManager.h"
#include "UWU_RigidBody.h"

ECS::UWU_SpriteAnimator::UWU_SpriteAnimator(std::string animationName, bool loop)
{
	_animation = UWU_TextureManager::getInstance().GetAnimation(animationName);
	_currentFrameNumber = -1;
	_currentFrame = _animation->GetNextFrame(_currentFrameNumber);
	_loop = loop;
}

ECS::UWU_SpriteAnimator::~UWU_SpriteAnimator()
{
}

void ECS::UWU_SpriteAnimator::SetAnimation(std::string animationName)
{
	_animation = UWU_TextureManager::getInstance().GetAnimation(animationName);
	_currentFrameNumber = 0;
	_currentFrame = _animation->GetNextFrame(_currentFrameNumber);
}

void ECS::UWU_SpriteAnimator::Update(float)
{
	if (_timer.GetDeltaTicks() >= _currentFrame->Duration) {
		_timer.Reset();
		_currentFrame = _animation->GetNextFrame(_currentFrameNumber);
		_spriteRenderer->SetSprite(_animation->SpriteFileName, _currentFrame->Rect);
		if (_currentFrameNumber == 0 && !_loop) {
			entity->world->RemoveEntity(entity);
		}
	}
}

void ECS::UWU_SpriteAnimator::LateUpdate(float)
{
}

void ECS::UWU_SpriteAnimator::Render(UWU_Window*)
{

}

void ECS::UWU_SpriteAnimator::Start()
{
	_spriteRenderer = entity->GetComponent<UWU_SpriteRenderer>();
	_spriteRenderer->SetSprite(_animation->SpriteFileName, _currentFrame->Rect);
}

void ECS::UWU_SpriteAnimator::OnCollision(UWU_Entity*)
{
}
