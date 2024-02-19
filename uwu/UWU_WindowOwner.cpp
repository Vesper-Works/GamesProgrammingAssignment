#include "UWU_WindowOwner.h"
#include "UWU_WindowManager.hpp"

using namespace ECS;
UWU_WindowOwner::UWU_WindowOwner(UWU_Window* window, float2 followStrength)
{
	_window = window;
	_followStrength = followStrength;
}

UWU_WindowOwner::~UWU_WindowOwner()
{
	UWU_WindowManager::RemoveWindow(_window);
}

void UWU_WindowOwner::Update(float deltaTime)
{
	int2 windowPos = _window->GetWindowPosition();
	int2 pos = (int2)_transform->position;
	pos.x -= _window->GetWindowSize().x / 2;
	pos.y -= _window->GetWindowSize().y / 2;
	pos.x += _transform->size.x / 2;
	pos.y += _transform->size.y / 2;
	int2 followPos;
	followPos.x = std::lerp(pos.x, windowPos.x, _followStrength.x);
	followPos.y = std::lerp(pos.y, windowPos.y, _followStrength.y);
	_window->SetWindowPosition(followPos);
}

void ECS::UWU_WindowOwner::LateUpdate(float)
{
}

void UWU_WindowOwner::Render(UWU_Window* window)
{
}

void UWU_WindowOwner::Start()
{
	_transform = entity->GetComponent<UWU_Transform>();
}

void ECS::UWU_WindowOwner::OnCollision(UWU_Entity*)
{
}

UWU_Window* ECS::UWU_WindowOwner::GetWindow()
{
	return _window;
}
