#include <fstream>
#include <SDL.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <time.h>
#include "UWU_Debug.h"
#include "UWU_ECS.h"
#include "UWU_RectUtil.h"
#include "UWU_Timer.hpp"
#include "UWU_InputManager.h"
#include <nlohmann/json.hpp>
#include "UWU_SpriteAnimation.h"
#include "UWU_TextureManager.h"
#include "UWU_WindowManager.hpp"
#include "UWU_Audio.hpp"
#include "UWU_Tweening.hpp"
#include <SDL_image.h>

using json = nlohmann::json;
using namespace ECS;

bool sizeMoveTimerRunning = false;

UWU_InputManager* inputManager;

#include <LDtkLoader/Project.hpp>


int main(int argc, char* args[]) {


	UWU_Debug::SetDebug(true);
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		UWU_Debug::Error("SDL_Init failed: %s", DebugMask::High, SDL_GetError());
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		UWU_Debug::Error("IMG_Init failed: %s", DebugMask::High, IMG_GetError());
	}

	if (TTF_Init() == -1)
	{
		UWU_Debug::Message("TTF init failed: %s", DebugMask::High, TTF_GetError());
	}

	SDL_DisplayMode dm;

	if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
	{
		UWU_Debug::Error("SDL_GetCurrentDisplayMode failed: %s", DebugMask::High, SDL_GetError());
	}

	RenderSizeMultiplier = dm.w / (1920.f / 3.f);
	UWU_Debug::Message("RenderSizeMultiplier: %f", DebugMask::High, RenderSizeMultiplier);
	UWU_Debug::Message("Screen width: %i", DebugMask::High, dm.w);

	UWU_InputManager::getInstance().Initialise();
	UWU_TextureManager::getInstance().Initialise();
	UWU_Audio::Initialise();

	UWU_WindowManager::InitStarterWindows(12);

	UWU_Window* toggleFullscreenWindow = nullptr;

	UWU_Timer fpsTimer;
	UWU_Timer performanceLogTimer;
	UWU_Timer performanceDeltaLogTimer;
	bool runMainGameLoop = true;
	const float target_frame_time = 1.0f / 60.0f; // 60 fps
	const long long target_frame_time_ticks = (long long)(target_frame_time * 1000000000.0f);
	long long deltaTicks = 0;
	double deltaTime = target_frame_time;
	bool lag = false;
	long long inputDeltaTicks = 0;
	long long updateDeltaTicks = 0;
	long long renderDeltaTicks = 0;
	int framesLogged = 0;
	ldtk::Project* project = new ldtk::Project();
	UWU_World* world = nullptr;
	try {
		project->loadFromFile("Assets/Level/Example.ldtk");
		world = new UWU_World(&project->getWorld());
	}
	catch (std::exception& ex) {
		UWU_Debug::Error("Failed to load LDtk project: %s", DebugMask::High, ex.what());
	}

#pragma region SplashScreens

	runMainGameLoop = false;
	UWU_Audio::PlaySoundEffect("Startup");
	std::vector<UWU_Window*> splashScreenWindows;
	for (size_t i = 0; i < 6; i++)
	{
		UWU_Window* window = UWU_WindowManager::NewWindow(int2(0, 0), int2(100, 100), 0, 0);
		splashScreenWindows.push_back(window);
	}

	UWU_Timer* spiralTimer = new UWU_Timer();
	UWU_Tweening::DoFor(
		[&splashScreenWindows, spiralTimer, dm, deltaTime]() {
			for (int i = 0; i < splashScreenWindows.size(); i++)
			{
				float theta = (1.f - (spiralTimer->GetDeltaTicks() / 3000.f)) * 20.f;
				float r = 10 * RenderSizeMultiplier * theta;

				int2 size = (int2(150, 150) * (1.f - (spiralTimer->GetDeltaTicks() / 3000.f)) * RenderSizeMultiplier);
				splashScreenWindows[i]->SetWindowPosition(int2(
					r * cos(theta + (i / 1.f)) + (dm.w / 2) - (size.x / 2),
					r * sin(theta + (i / 1.f)) + (dm.h / 2) - (size.y / 2)));
				splashScreenWindows[i]->SetWindowSize(size);
				splashScreenWindows[i]->PrepareForRender();
				splashScreenWindows[i]->PresentRender();
			}
		},
		[&splashScreenWindows, spiralTimer]() {
			for (auto& window : splashScreenWindows) {
				UWU_WindowManager::RemoveWindow(window);
			}
			splashScreenWindows.clear();
			delete spiralTimer;
		}, 3000);

	UWU_Tweening::DoAfter([&runMainGameLoop, dm, world]() {
		UWU_Window* window = UWU_WindowManager::NewWindow(int2(0, 0), int2(100, 100), SDL_WINDOW_FULLSCREEN_DESKTOP, 0);
		UWU_Entity* entity = new UWU_Entity(world);
		entity->AddComponent<UWU_Transform>(float2((dm.w / RenderSizeMultiplier) / 2 - 150, (dm.h / RenderSizeMultiplier) / 2 - 150), float2(300, 300));
		entity->AddComponent<UWU_SpriteRenderer>();
		entity->AddComponent<UWU_SpriteAnimator>("startup");
		UWU_Tweening::DoFor(
			[window, entity, dm]() {
				entity->Update(0);
				entity->LateUpdate(0);
				window->PrepareForRender(0, 0, 0);
				entity->Render(window);
				UWU_TextManager::Draw(window->GetRenderer(),
R"V0G0N(
Keyboard Controls:

Arrow keys movement
Z: Jump
X: Attack
C: Dash
M: Toggle music

Debug:
1,2,3: Toggle logging debug mask
Q: Spawn block
W, E, R: Spawn item
F: Toggle fullscreen
)V0G0N", 0, 0);
				UWU_TextManager::Draw(window->GetRenderer(),
R"V0G0N(
Controller Controls:

Left stick movement
A: Jump
X: Attack
Right Trigger: Dash
)V0G0N", dm.w - (140 * RenderSizeMultiplier), 0);
				window->PresentRender();
			},
			[window, entity, &runMainGameLoop]() {
				UWU_WindowManager::RemoveWindow(window);
				delete entity;
				runMainGameLoop = true;
				GameTimer.Reset();
				UWU_Audio::PlayMusic("Song");
			}, 6000);
		}, 3000);
#pragma endregion

	while (1)
	{
		framesLogged++;
		performanceDeltaLogTimer.Reset();
		fpsTimer.Reset();
		if (UWU_InputManager::getInstance().ProcessEvents()) {
			break;
		}

		if (UWU_InputManager::getInstance().GetActionStateOnce(B_SoundToggle)) {
			 UWU_Audio::ToggleMute();
		}
		if (UWU_InputManager::getInstance().GetActionStateOnce(B_DebugAction5)) {
			if (toggleFullscreenWindow == nullptr) {
				toggleFullscreenWindow = UWU_WindowManager::NewWindow(int2(0, 0), int2(100, 100), SDL_WINDOW_FULLSCREEN_DESKTOP, 0);
			}
			else {
				UWU_WindowManager::RemoveWindow(toggleFullscreenWindow);
				toggleFullscreenWindow = nullptr;
			}
		}

		inputDeltaTicks += performanceDeltaLogTimer.GetDeltaTicks<std::chrono::nanoseconds>();
		performanceDeltaLogTimer.Reset();

		UWU_Tweening::Update();

		if (runMainGameLoop) {
			world->Update(deltaTime);
			updateDeltaTicks += performanceDeltaLogTimer.GetDeltaTicks<std::chrono::nanoseconds>();
			performanceDeltaLogTimer.Reset();

			UWU_WindowManager::RenderWorld(world);
			renderDeltaTicks += performanceDeltaLogTimer.GetDeltaTicks<std::chrono::nanoseconds>();
			performanceDeltaLogTimer.Reset();
		}

		if (performanceLogTimer.GetDeltaTicks<std::chrono::seconds>() >= 1) {
			UWU_Debug::Message("Average of %i frames - Update: %fms, Render: %fms, Input: %fms, Total: %fms", DebugMask::Low,
				framesLogged,
				updateDeltaTicks / 1000000.0f / framesLogged,
				renderDeltaTicks / 1000000.0f / framesLogged,
				inputDeltaTicks / 1000000.0f / framesLogged,
				(updateDeltaTicks + renderDeltaTicks + inputDeltaTicks) / 1000000.0f / framesLogged);
			updateDeltaTicks = 0;
			renderDeltaTicks = 0;
			inputDeltaTicks = 0;
			framesLogged = 0;
			performanceLogTimer.Reset();
		}

		//if (UWU_InputManager::getInstance().GetActionStateOnce(B_DebugLag)) {
		//	lag = !lag;
		//}
		//if (lag) {
		//	SDL_Delay((1.0f / 15.0f) * 1000.0f);
		//}

		deltaTicks = min(fpsTimer.GetDeltaTicks<std::chrono::nanoseconds>(), (long long)500000000);

		if (deltaTicks < target_frame_time_ticks)
		{
			long long delay_ticks = target_frame_time_ticks - deltaTicks;
			std::this_thread::sleep_for(std::chrono::nanoseconds(delay_ticks));
			deltaTicks += (delay_ticks);
		}
		deltaTime = deltaTicks / 1000000000.0f;

		std::string message = std::to_string(1.f / (deltaTime)) + " | Debug Mask: ";
		int currentMask = UWU_Debug::GetDebugMask();
		for (size_t i = 1; i <= DebugMask::END; i++)
		{
			if ((currentMask >> i - 1) & 1U) {
				message += DebugMaskNames[i - 1] + ", ";
			}
		}

		//Set console window title using SetConsoleTitleA to framerate
		SetConsoleTitleA(message.c_str());
	}
	//Clean up all memory usage
	delete project;
	delete world;
	UWU_TextureManager::DeleteInstance(); //Errors but I don't know why D:
	UWU_WindowManager::Release();
	TTF_Quit();
	SDL_Quit();
}