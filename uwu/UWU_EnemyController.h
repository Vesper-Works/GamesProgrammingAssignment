#ifndef UWU_ENEMYCONTROLLER
#define UWU_ENEMYCONTROLLER
#include "UWU_Component.h"
#include "UWU_Rigidbody.h"
namespace ECS{
class UWU_EnemyController :
    public UWU_Component
{
public:
	UWU_EnemyController(std::vector<int2> patrolPath, float movementSpeed = 5);
	~UWU_EnemyController();
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
	std::vector<int2> _patrolPath;
	int _currentNodeIndex;
	UWU_Rigidbody* _rigidBody;
	UWU_Transform* _transform;
	float _movementSpeed;
	UWU_Timer _attackTimer;
	UWU_Entity* _glow;
	ItemType _currentItem;
};
}
#endif //!UWU_ENEMYCONTROLLER

