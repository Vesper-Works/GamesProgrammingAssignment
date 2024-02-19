#ifndef UWU_RIGIDBODY
#define UWU_RIGIDBODY
#include "UWU_Transform.h"
#include "UWU_Timer.hpp"
namespace ECS {
	class UWU_Rigidbody :
		public UWU_Component
	{
	public:
		UWU_Rigidbody(float2 gravity, float mass = 2, bool isStatic = false, bool isTrigger = false);
		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		void NotifyGrounded();
		virtual void OnCollision(UWU_Entity*) override;

		float2 acceleration;
		float2 velocity;
		float mass;
		bool isStatic;
		bool isTrigger;
		bool isGrounded; 
		UWU_Transform* transform;
	private:
		float2 _gravity;
		UWU_Timer  _groundedTimer;
	};
}
#endif // !UWU_RIGIDBODY