#ifndef UWU_EXIT
#define UWU_EXIT

#include "UWU_Component.h"
namespace ECS {
	class UWU_Exit:
		public UWU_Component
	{
	public:
		UWU_Exit();
		~UWU_Exit() override;
		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		virtual void OnCollision(UWU_Entity*) override;
	private:
		bool _exiting;
	};
}
#endif // !UWU_EXIT