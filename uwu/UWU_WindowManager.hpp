#ifndef UWU_WINDOWMANAGER
#define UWU_WINDOWMANAGER
#pragma comment (lib, "Dwmapi")
#include "UWU_Window.h"
#include "UWU_Debug.h"
#include "UWU_World.h"
#include "UWU_TextureManager.h"
#include "UWU_TextManager.hpp"
#include <SDL.h>
#include <SDL_syswm.h>
#include <dwmapi.h>

class UWU_WindowManager {
private:
	static inline std::vector<UWU_Window*> _activeWindows;
	static inline std::vector<UWU_Window*> _inactiveWindows;
	static inline std::vector<UWU_Window*> _customWindows;

	static inline void RemoveWindowAnimation(SDL_Window* window) {
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);
		HWND hwnd = wmInfo.info.win.window;
		BOOL attrib = TRUE;
		DwmSetWindowAttribute(hwnd, DWMWA_TRANSITIONS_FORCEDISABLED, &attrib, sizeof(attrib));
	}
public:
	static inline void InitStarterWindows(int numberOfWindows) {
		for (size_t i = 0; i < numberOfWindows; i++)
		{
			SDL_Window* sdlWindow = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_HIDDEN);
			//RemoveWindowAnimation(sdlWindow);
			SDL_Renderer* sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
			UWU_TextureManager::getInstance().InitialiseRenderer(sdlRenderer);
			UWU_TextManager::InitialiseRenderer(sdlRenderer);
			UWU_Window* window = new UWU_Window(sdlWindow, sdlRenderer);
			_inactiveWindows.push_back(window);
		}
	}

	static inline UWU_Window* NewWindow(int2 position, int2 size, Uint32 fullscreenFlags, Uint32 rendererFlags) {

		if (_inactiveWindows.size() == 0) {
			SDL_Window* sdlWindow = SDL_CreateWindow("", position.x * RenderSizeMultiplier,
				position.y * RenderSizeMultiplier,
				size.x * RenderSizeMultiplier,
				size.y * RenderSizeMultiplier,
				fullscreenFlags);
			SDL_Renderer* sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, rendererFlags);
			UWU_TextureManager::getInstance().InitialiseRenderer(sdlRenderer);
			UWU_TextManager::InitialiseRenderer(sdlRenderer);
			UWU_Window* window = new UWU_Window(sdlWindow, sdlRenderer);
			//RemoveWindowAnimation(sdlWindow);
			SDL_RaiseWindow(sdlWindow);
			_activeWindows.push_back(window);
			return window;
		}
		UWU_Window* window = _inactiveWindows[0];
		_inactiveWindows.erase(_inactiveWindows.begin());
		window->SetWindowSize(size * RenderSizeMultiplier);
		window->SetWindowPosition(position * RenderSizeMultiplier);
		SDL_SetWindowFullscreen(window->GetWindow(), fullscreenFlags);
		SDL_ShowWindow(window->GetWindow());
		SDL_RaiseWindow(window->GetWindow());
		_activeWindows.push_back(window);
		return window;
	}

	static inline UWU_Window* NewCustomWindow(int2 position, int2 size, Uint32 fullscreenFlags, Uint32 rendererFlags) {

		if (_inactiveWindows.size() == 0) {
			SDL_Window* sdlWindow = SDL_CreateWindow("", position.x * RenderSizeMultiplier,
				position.y * RenderSizeMultiplier,
				size.x * RenderSizeMultiplier,
				size.y * RenderSizeMultiplier,
				fullscreenFlags);
			SDL_Renderer* sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, rendererFlags);
			UWU_TextureManager::getInstance().InitialiseRenderer(sdlRenderer);
			UWU_TextManager::InitialiseRenderer(sdlRenderer);
			UWU_Window* window = new UWU_Window(sdlWindow, sdlRenderer);
			//RemoveWindowAnimation(sdlWindow);
			SDL_RaiseWindow(sdlWindow);
			return window;
		}
		UWU_Window* window = _inactiveWindows[0];
		_inactiveWindows.erase(_inactiveWindows.begin());
		window->SetWindowSize(size * RenderSizeMultiplier);
		window->SetWindowPosition(position * RenderSizeMultiplier);
		SDL_SetWindowFullscreen(window->GetWindow(), fullscreenFlags);
		SDL_ShowWindow(window->GetWindow());
		SDL_RaiseWindow(window->GetWindow());
		return window;
	}

	static inline void RemoveWindow(UWU_Window* window) {
		if (window) {
			SDL_HideWindow(window->GetWindow());
			_activeWindows.erase(std::remove(_activeWindows.begin(), _activeWindows.end(), window));
			_inactiveWindows.push_back(window);
		}
		else {
			UWU_Debug::Error("Tried to remove window null pointer", DebugMask::High);
		}
	}

	static inline void RenderWorld(UWU_World* world) {
		for (auto& window : _activeWindows) {
			world->Render(window);
		}
	}

	static inline void Release() {

		for (auto* window : _activeWindows) {
			delete window;
		}
		for (auto* window : _inactiveWindows) {
			delete window;
		}
	}
};

#endif