#include "UWU_FollowMouse.h"

ECS::UWU_FollowMouse::UWU_FollowMouse()
{
}

void ECS::UWU_FollowMouse::Update(float deltaTime)
{
	int x, y;
	SDL_GetGlobalMouseState(&x, &y);
	_transform->position.x = x - _transform->size.x / 2;
	_transform->position.y = y - _transform->size.y / 2;
}

void ECS::UWU_FollowMouse::LateUpdate(float) {

}

void ECS::UWU_FollowMouse::Render(UWU_Window*)
{
}

void ECS::UWU_FollowMouse::Start()
{
	_transform = entity->GetComponent<UWU_Transform>();
}

void ECS::UWU_FollowMouse::OnCollision(UWU_Entity*)
{
}
