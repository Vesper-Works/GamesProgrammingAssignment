#ifndef UWU_HEALTH
#define UWU_HEALTH

#include "UWU_Transform.h"
namespace ECS {
	class UWU_Health :
		public UWU_Component
	{
	public:
		UWU_Health(int maxHealth);
		~UWU_Health() override;
		void TakeDamage(int);
		void Heal (int);
		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		virtual void OnCollision(UWU_Entity*) override;
	private:
		int _health;
		UWU_Transform* _transform;
	};
}
#endif // !UWU_HEALTH