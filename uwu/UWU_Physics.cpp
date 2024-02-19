#include "UWU_Physics.h"
#include "UWU_RectUtil.h"
#include <algorithm>
#define aprox(a,b) std::abs(a-b)<0.01

using namespace ECS;

void UWU_Physics::DoSteps(int numOfSteps) {
	//const float subDeltaTime = deltaTime / float(numOfSteps);
	for (size_t i = 0; i < numOfSteps; i++)
	{
		Step(1.f / numOfSteps);
		ConstrainToScreen();
	}
}

void UWU_Physics::ConstrainToScreen() {
	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		UWU_Debug::Error("SDL_GetDesktopDisplayMode failed: %s", DebugMask::High, SDL_GetError());
	}

	const int w{ dm.w },
		h{ dm.h };
	for (auto& body : _simulatedBodies) {
		if (body->isStatic || !body->enabled) continue;
		UWU_Transform* transform = body->transform;
		transform->position.x = std::fmax(0, std::fmin(w, transform->position.x + transform->size.x) - transform->size.x);
		transform->position.y = std::fmax(0, std::fmin(h - 40, transform->position.y + transform->size.y) - transform->size.y);
		body->isGrounded = body->isGrounded || transform->position.y + transform->size.y == h - 40;
	}
}

//SAT my beloved
bool UWU_Physics::ProcessCollisionSAT(const SDL_Rect* rectA, const SDL_Rect* rectB, float2* collisionA, float2* collisionB) {

	float2 rectACorners[4] = {
		float2(rectA->x, rectA->y),
		float2(rectA->x + rectA->w, rectA->y),
		float2(rectA->x + rectA->w, rectA->y + rectA->h),
		float2(rectA->x, rectA->y + rectA->h)
	};
	float2 rectBCorners[4] = {
		float2(rectB->x, rectB->y),
		float2(rectB->x + rectB->w, rectB->y),
		float2(rectB->x + rectB->w, rectB->y + rectB->h),
		float2(rectB->x, rectB->y + rectB->h)
	};

	//Typically get from rects, but with no rotation the axis is always the same
	float2 axis[2] = {
		float2(1, 0),
		float2(0, 1)
	};
	const int axisCount = 2; //replace with 4 when rotation is added
	float smallestMag = FLT_MAX;
	for (size_t i = 0; i < axisCount; i++)
	{
		//get the max dot product of the corners of rectA and rectB with the axis

		float maxA = -FLT_MAX;
		float minA = FLT_MAX;
		float maxB = -FLT_MAX;
		float minB = FLT_MAX;

		for (size_t j = 0; j < 4; j++)
		{
			float dotA = axis[i].Dot(rectACorners[j]);
			if (dotA < minA) {
				minA = dotA;
			}
			else if (dotA > maxA) {
				maxA = dotA;
			}
			float dotB = axis[i].Dot(rectBCorners[j]);
			if (dotB < minB) {
				minB = dotB;
			}
			else if (dotB > maxB) {
				maxB = dotB;
			}
		}
		if (maxA < minB || maxB < minA) {
			return false;
		}

		float distance = 0;
		if (maxA < maxB) {
			distance = minB - maxA;
		}
		else {
			distance = maxB - minA;
		}
		//Check if smallest vector so far
		if (abs(distance) < abs(smallestMag)) {
			smallestMag = distance;
			*collisionA = axis[i];
			*collisionB = -axis[i];
		}
	}
	*collisionA *= smallestMag;
	*collisionB *= smallestMag;
	return true;
}
void UWU_Physics::Step(float stepSize)
{
	//Broad stage of sweep and prune collision detection
	//Sort the bodies by their x position
	std::sort(_simulatedBodies.begin(), _simulatedBodies.end(), [](UWU_Rigidbody* a, UWU_Rigidbody* b) {
		return a->transform->position.x < b->transform->position.x;
		});

	//Find all likely collisions based on x positions
	std::vector<std::pair<size_t, size_t>> collisionPairs;
	collisionPairs.reserve(_simulatedBodies.size() * (_simulatedBodies.size() - 1) / 2);

	std::vector<size_t> activeInterval;
	activeInterval.reserve(_simulatedBodies.size());

	for (size_t i = 0; i < _simulatedBodies.size(); i++)
	{
		UWU_Transform* transformA = _simulatedBodies[i]->transform;
		for (size_t j = 0; j < activeInterval.size(); j++)
		{
			UWU_Transform* transformB = _simulatedBodies[activeInterval[j]]->transform;

			//skip if disabled or both static
			if (_simulatedBodies[i]->isStatic && _simulatedBodies[activeInterval[j]]->isStatic) {
				continue;
			}

			if (transformA->position.x > transformB->position.x + transformB->size.x) {
				std::swap(activeInterval[j], activeInterval.back());
				activeInterval.pop_back();
				j--;
			}
			else {
				collisionPairs.emplace_back(i, activeInterval[j]);
			}

		}
		activeInterval.push_back(i);
	}

	//Narrow stage of sweep and prune collision detection
	//Find the collision normal and penetration depth
	for (size_t i = 0; i < collisionPairs.size(); i++)
	{
		UWU_Rigidbody* bodyA = _simulatedBodies[collisionPairs[i].first];
		UWU_Rigidbody* bodyB = _simulatedBodies[collisionPairs[i].second];
		UWU_Transform* transformA = bodyA->transform;
		UWU_Transform* transformB = bodyB->transform;
		float2 directionA;
		float2 directionB;
		if (ProcessCollisionSAT(transformA->GetRect(), transformB->GetRect(), &directionA, &directionB)) {
			const float aMinMass = min(1, bodyA->mass);
			const float bMinMass = min(1, bodyB->mass);
			const float aMassRatio = aMinMass / (aMinMass + bMinMass);
			const float bMassRatio = bMinMass / (aMinMass + bMinMass);

			bodyA->entity->OnCollision(bodyB->entity);
			bodyB->entity->OnCollision(bodyA->entity);

			const float directionDot = directionA.Dot(directionB);

			if (bodyA->isTrigger || bodyB->isTrigger) {
				continue;
			}

			if (bodyA->isStatic) {
				bodyB->velocity += directionB * 0.2f;
			}
			else {
				if (directionA.y < 0) {
					bodyA->NotifyGrounded();
				}
				if (directionDot < 0) {
					bodyA->velocity += directionA * stepSize * bMassRatio;
				}
				transformA->position += directionA;
			}
			if (bodyB->isStatic) {
				bodyA->velocity += directionA * 0.2f;
			}
			else {
				if (directionB.y < 0) {
					bodyB->NotifyGrounded();
				}
				if (directionDot < 0) {
					bodyB->velocity += directionB * stepSize * aMassRatio;
				}
				transformB->position += directionB;
			}
		}
	}
}


void UWU_Physics::AddBody(UWU_Rigidbody* body)
{
	_simulatedBodies.push_back(body);
}

void UWU_Physics::RemoveBody(UWU_Rigidbody* body)
{
	_simulatedBodies.erase(std::remove(_simulatedBodies.begin(), _simulatedBodies.end(), body), _simulatedBodies.end());
}