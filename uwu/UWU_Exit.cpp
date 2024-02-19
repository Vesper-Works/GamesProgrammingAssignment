#include "UWU_Exit.h"
#include "UWU_Transform.h"
#include "UWU_WindowManager.hpp"
#include "UWU_Tweening.hpp"
#include "UWU_Audio.hpp"

using namespace ECS;
UWU_Exit::UWU_Exit()
{
}

UWU_Exit::~UWU_Exit()
{	
}

void UWU_Exit::Update(float)
{
}

void UWU_Exit::Render(UWU_Window* window)
{
}

void UWU_Exit::Start()
{
}

void UWU_Exit::LateUpdate(float)
{
}

void UWU_Exit::OnCollision(UWU_Entity* other)
{
	if (!_exiting && other->GetTag() == "Player")
	{
		GameTimer.Reset();
		_exiting = true;
		UWU_Audio::StopMusic();
		UWU_Audio::PlaySoundEffect("Victory");
		UWU_Window* window = UWU_WindowManager::NewCustomWindow(int2(1920.f / RenderSizeMultiplier / 2, 1080.f / RenderSizeMultiplier / 2) - int2(100, 100), int2(200, 200), 0, 0);
		UWU_Tweening::DoFor([window]() {
			window->PrepareForRender(0, 0, 0);
			UWU_TextManager::Draw(window->GetRenderer(), "Congradulations!\nScore: " + std::to_string(Score), 40 * RenderSizeMultiplier, 40 * RenderSizeMultiplier, 205, 255, 205);
			window->PresentRender();
			},
			[]() {
				exit(1);
			}, 5500);
		entity->world->RemoveEntity(entity);
	}
}