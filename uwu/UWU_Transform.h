#ifndef UWU_TRANSFORM
#define UWU_TRANSFORM
#include "UWU_Component.h"
namespace ECS {

	class UWU_Transform :
		public UWU_Component
	{
	public:
		UWU_Transform(float2, float2);
		// Inherited via UWU_Component
		virtual void Start() override;
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void OnCollision(UWU_Entity*) override;
		const SDL_Rect* GetRect();
		float2 position;
		float2 size;
	private:
		SDL_Rect _rect;
	};

}
#endif // !UWU_TRANSFORM


