#ifndef UWU_HURTBODY
#define UWU_HURTBODY

#include "UWU_Component.h"
namespace ECS {
	class UWU_HurtBody:
		public UWU_Component
	{
	public:
		UWU_HurtBody(int, std::vector<UWU_Entity*> avoid = {});
		~UWU_HurtBody() override;
		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		virtual void OnCollision(UWU_Entity*) override;
	private: 
		std::vector<UWU_Entity*> _avoid;
		int _damage;
	};
}
#endif //!UWU_HURTBODY