#ifndef UWU_FOLLOWMOUSE
#define UWU_FOLLOWMOUSE
#include "UWU_Transform.h"
namespace ECS {
	class UWU_FollowMouse :
		public UWU_Component
	{
	public:
		UWU_FollowMouse();
		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		virtual void OnCollision(UWU_Entity*) override;

	private:
		UWU_Transform* _transform;
	};
}
#endif // !UWU_FOLLOWMOUSE


