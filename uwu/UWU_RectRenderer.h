#ifndef UWU_RECTRENDERER
#define UWU_RECTRENDERER
#include "UWU_Transform.h"
namespace ECS {
	class UWU_RectRenderer :
		public UWU_Component
	{
	public:
		UWU_RectRenderer();
		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		virtual void OnCollision(UWU_Entity*) override = 0;
	private:
		UWU_Transform* _transform;
	};
}
#endif // !UWU_RECTRENDERER


