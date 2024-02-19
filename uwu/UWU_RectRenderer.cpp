#include "UWU_RectRenderer.h"


ECS::UWU_RectRenderer::UWU_RectRenderer()
{
}

void ECS::UWU_RectRenderer::Update(float deltaTime)
{

}
void ECS::UWU_RectRenderer::LateUpdate(float) {

}
void ECS::UWU_RectRenderer::Start() {
	_transform = entity->GetComponent<UWU_Transform>();
}

void ECS::UWU_RectRenderer::Render(UWU_Window* window)
{
	SDL_Rect rect;
	float2 inversePos = window->GetInversePosition(_transform->position);
	rect.x = std::floor(inversePos.x);
	rect.y = std::floor(inversePos.y);
	rect.w = std::floor(_transform->size.x);
	rect.h = std::floor(_transform->size.y);
	SDL_Renderer* renderer = window->GetRenderer();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(renderer, &rect);
}