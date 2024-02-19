#ifndef UWU_COMPONENT
#define UWU_COMPONENT
#include "UWU_Entity.h"
#include "UWU_Window.h"
namespace ECS {

	class UWU_Entity;
	class UWU_Component
	{
	public:
		virtual ~UWU_Component() = default;
		virtual void Update(float) = 0;
		virtual void LateUpdate(float) = 0;
		virtual	void Render(UWU_Window*) = 0;
		virtual	void Start() = 0;
		virtual void OnCollision(UWU_Entity*) = 0;
		UWU_Entity* entity;
		bool enabled = true;
	};
}
#endif //! UWU_COMPONENT