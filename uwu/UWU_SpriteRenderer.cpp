#include "UWU_SpriteRenderer.h"
#include "UWU_TextureManager.h"

using namespace ECS;
UWU_SpriteRenderer::UWU_SpriteRenderer(std::string spriteName, bool useSpriteSize, SDL_Colour tint)
{
	_spriteName = spriteName;
	_useSpriteSize = useSpriteSize;
	_spriteRect = { -1,-1,-1,-1 };
	_tint = tint;
}

UWU_SpriteRenderer::~UWU_SpriteRenderer()
{
}

void UWU_SpriteRenderer::Update(float deltaTime)
{
}

void ECS::UWU_SpriteRenderer::LateUpdate(float)
{
}

void UWU_SpriteRenderer::Render(UWU_Window* window)
{
	SDL_Rect rect;
	float2 inversePos = window->GetInversePosition(_transform->position);
	rect.x = std::floor(inversePos.x);
	rect.y = std::floor(inversePos.y);
	if (_useSpriteSize) {
		_transform->size = float2(_spriteRect.w, _spriteRect.h)*RenderSizeMultiplier;
	}	
	rect.w = std::floor(_transform->size.x);
	rect.h = std::floor(_transform->size.y);
	UWU_TextureManager::getInstance().Draw(window->GetRenderer(), _spriteName, _spriteRect .x == -1 ? 0 : &_spriteRect, &rect, 0, 0, _flip, _tint);
}

void UWU_SpriteRenderer::Start()
{
	_transform = entity->GetComponent<UWU_Transform>();
}

void ECS::UWU_SpriteRenderer::OnCollision(UWU_Entity*)
{
}

void ECS::UWU_SpriteRenderer::SetSprite(std::string path, SDL_Rect rect)
{
	_spriteName = path;
	_spriteRect = rect;
}

void ECS::UWU_SpriteRenderer::SetFlipped(SDL_RendererFlip flip)
{
	_flip = flip;
}

void ECS::UWU_SpriteRenderer::SetTint(SDL_Colour tint)
{
	_tint = tint;
}
