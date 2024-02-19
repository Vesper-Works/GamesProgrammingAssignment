#include "UWU_Transform.h"
namespace ECS {

	void UWU_Transform::Update(float deltaTime)
	{
	}
	void UWU_Transform::LateUpdate(float)
	{
	}
	void UWU_Transform::Render(UWU_Window*)
	{
	}
	void UWU_Transform::OnCollision(UWU_Entity*)
	{
	}
	const SDL_Rect* UWU_Transform::GetRect()
	{
		_rect.x = position.x;
		_rect.y = position.y;
		_rect.w = size.x;
		_rect.h = size.y;
		return &_rect;
	}
	UWU_Transform::UWU_Transform(float2 pos, float2 size)
	{
		this->position = pos * RenderSizeMultiplier;
		this->size = size * RenderSizeMultiplier;
	}
	void UWU_Transform::Start()
	{
	}
}