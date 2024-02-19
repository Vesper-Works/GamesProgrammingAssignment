#include "UWU_Rigidbody.h"

ECS::UWU_Rigidbody::UWU_Rigidbody(float2 gravity, float mass, bool isStatic, bool isTrigger)
{
	_gravity = gravity;
	this->mass = mass;
	transform = 0;
	velocity = float2(0, 0);
	this->isStatic = isStatic;
	this->isTrigger = isTrigger;
	isGrounded = false;
}

void ECS::UWU_Rigidbody::Update(float deltaTime)
{
}

void ECS::UWU_Rigidbody::LateUpdate(float deltaTime)
{
	if (isStatic) { return; }
	float2 dragForce = velocity * (isGrounded ? -4 : -1);// *(acceleration.x == 0 && acceleration.y == 0 ? 2 : 0); //Fails with controller
	velocity += acceleration * deltaTime;
	velocity += dragForce * deltaTime;
	velocity += _gravity * deltaTime;
	transform->position += velocity * RenderSizeMultiplier / 3; //Divide by 3 as I tuned all values before realising 
																//I needed to multiply by RenderSizeMultiplier to keep 
																//movement speed independent from desktop resolution
	if (_groundedTimer.GetDeltaTicks() > 200) {
		isGrounded = false;
	}
}

void ECS::UWU_Rigidbody::Render(UWU_Window*)
{
}

void ECS::UWU_Rigidbody::Start()
{
	//_physicsManager.AddBody(this);
	transform = entity->GetComponent<UWU_Transform>();
}

void ECS::UWU_Rigidbody::NotifyGrounded()
{
	isGrounded = true;
	_groundedTimer.Reset();
}

void ECS::UWU_Rigidbody::OnCollision(UWU_Entity*)
{
}
