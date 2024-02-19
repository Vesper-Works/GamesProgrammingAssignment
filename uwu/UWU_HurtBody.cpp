#include "UWU_HurtBody.h"
#include "UWU_Health.h"
#include "UWU_Rigidbody.h"

using namespace ECS;
UWU_HurtBody::UWU_HurtBody(int damage, std::vector<UWU_Entity*> avoid)
{
	_avoid = avoid;
	_damage = damage;
}

UWU_HurtBody::~UWU_HurtBody()
{
}

void UWU_HurtBody::Update(float)
{
}

void UWU_HurtBody::Render(UWU_Window* window)
{
}

void UWU_HurtBody::Start()
{
}

void UWU_HurtBody::LateUpdate(float)
{
}

void UWU_HurtBody::OnCollision(UWU_Entity* other)
{
	if (std::count(_avoid.begin(), _avoid.end(), other) > 0) { return; }
	UWU_Debug::Message("Collided with <%p>", DebugMask::High, other);
	_avoid.push_back(other);
	if (other->HasComponent<UWU_Health>()) {
		other->GetComponent<UWU_Health>()->TakeDamage(_damage);
	}
}
