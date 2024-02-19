#ifndef UWU_SPRITERENDERER
#define UWU_SPRITERENDERER

#include "UWU_Transform.h"
namespace ECS {
	class UWU_SpriteRenderer :
		public UWU_Component
	{
	public:
		UWU_SpriteRenderer(std::string = "default", bool useSpriteSize = false, SDL_Colour tint = SDL_Colour{255,255,255,255});
		~UWU_SpriteRenderer() override;
		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		virtual void OnCollision(UWU_Entity*) override;
		void SetSprite(std::string path, SDL_Rect rect);
		void SetFlipped(SDL_RendererFlip flip);
		void SetTint(SDL_Colour tint);
	private:
		UWU_Transform* _transform;
		std::string _spriteName;
		SDL_Rect _spriteRect;
		bool _useSpriteSize;
		SDL_RendererFlip _flip;
		SDL_Colour _tint;
	};
}
#endif // !UWU_SPRITERENDERER


