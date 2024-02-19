#include "UWU_Health.h"
#include "UWU_TextManager.hpp"
#include "UWU_Audio.hpp"
#include "UWU_Tweening.hpp"
#include "UWU_WindowManager.hpp"

using namespace ECS;
UWU_Health::UWU_Health(int maxHealth)
{
	_health = maxHealth;
}

UWU_Health::~UWU_Health()
{
}

void UWU_Health::Update(float)
{
}

void UWU_Health::Render(UWU_Window* window)
{
	float2 pos = window->GetInversePosition(_transform->position);
	UWU_TextManager::Draw(window->GetRenderer(), std::to_string(_health), pos.x, pos.y - (10 * RenderSizeMultiplier), 255, 100, 100);
}

void UWU_Health::Start()
{
	_transform = entity->GetComponent<UWU_Transform>();
}

void UWU_Health::LateUpdate(float)
{
}

void UWU_Health::OnCollision(UWU_Entity* other)
{
}

void UWU_Health::TakeDamage(int damage)
{
	_health -= damage;
	if (_health <= 0) {
		if (entity->GetTag() == "Player") {
			UWU_Audio::StopMusic();
			UWU_Audio::PlaySoundEffect("GameOver");
			UWU_Window* window = UWU_WindowManager::NewCustomWindow(int2(1920.f / RenderSizeMultiplier / 2, 1080.f / RenderSizeMultiplier / 2) - int2(100, 100), int2(200, 200), 0, 0);
			UWU_Tweening::DoFor([window]() {
				window->PrepareForRender(0, 0, 0);
				UWU_TextManager::Draw(window->GetRenderer(), "Game Over\nScore: " + std::to_string(Score), 40 * RenderSizeMultiplier, 40 * RenderSizeMultiplier, 255, 205, 205);
				window->PresentRender();
				},
				[]() {
					exit(1);
				}, 5500);
		}
		else {
			Score += 3000;
			MaxTimeLeft += 3;
			if (entity->GetTag() == "Player") {
				Score -= 750;
			}
		}
		entity->world->RemoveEntity(entity);
		UWU_Audio::PlaySoundEffect("Death");
	}
	else {
		UWU_Audio::PlaySoundEffect("Hurt");

	}
}

void ECS::UWU_Health::Heal(int amount)
{
	_health += amount < 0 ? 0 : amount;
	if (entity->GetTag() == "Player") {
		Score += 750;
	}
}
