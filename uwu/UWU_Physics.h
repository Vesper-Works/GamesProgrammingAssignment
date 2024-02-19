#ifndef UWU_PHYSICS
#define UWU_PHYSICS
#include "UWU_Rigidbody.h"
#include "UWU_Entity.h"

class UWU_Physics
{
public:
	void DoSteps(int);
	void AddBody(ECS::UWU_Rigidbody*);
	void RemoveBody(ECS::UWU_Rigidbody*);
private:
	void Step(float);
	void ConstrainToScreen();
	bool ProcessCollisionSAT(const SDL_Rect* rectA, const SDL_Rect* rectB, float2* collisionA, float2* collisionB);
	std::vector<ECS::UWU_Rigidbody*> _simulatedBodies;
};

#endif // !UWU_PHYSICS


