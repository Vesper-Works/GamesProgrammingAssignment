#ifndef UWU_PLAYERCONTROLLER
#define UWU_PLAYERCONTROLLER
#include "UWU_Rigidbody.h"
#include "UWU_Timer.hpp"
namespace ECS {
	class UWU_PlayerController :
		public UWU_Component
	{
	public:
		enum State {
			Idle,
			Running,
			Jumping,
			Dashing,
		};
		inline static const char* stateNames[] = {
			"player_idle",
			"player_run",
			"player_jump",
			"player_dash",
		};
		UWU_PlayerController(float movementSpeed = 5);
		// Inherited via UWU_Component
		virtual void Update(float) override;
		virtual void LateUpdate(float) override;
		virtual void Render(UWU_Window*) override;
		virtual void Start() override;
		virtual void OnCollision(UWU_Entity*) override;
	private:
		void BaseSwing();
		void LongSwing();
		void LongLongSwing();
		UWU_Rigidbody* _rigidBody;
		UWU_Transform* _transform;
		float _movementSpeed;
		UWU_Timer _dashTimer;
		UWU_Timer _jumpTimer;
		UWU_Timer _attackTimer;
		float _directionFacing;
		State _state;
		UWU_Entity* _glow;
		ItemType _currentItem;
	};
}
#endif // !UWU_PLAYERCONTROLLER


