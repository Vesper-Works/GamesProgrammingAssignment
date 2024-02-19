#ifndef UWU_WINDOWOWNER
#define UWU_WINDOWOWNER

#include "UWU_Component.h"
#include "UWU_Transform.h"
namespace ECS {
	class UWU_WindowOwner:
		public UWU_Component
	{
	public:
		UWU_WindowOwner(UWU_Window*, float2 lerpScale = float2(0.5f, 0.5f));
		~UWU_WindowOwner() override;
		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		virtual void OnCollision(UWU_Entity*) override;
		UWU_Window* GetWindow();
	private:
		UWU_Window* _window;
		UWU_Transform* _transform;
		float2 _followStrength;
	};
}
#endif //UWU_WINDOWOWNER