#include "UWU_InputManager.h"
#include <SDL.h>
#include "UWU_Debug.h"
#include <algorithm>
#define AXIS_MAX 32767.0


void UWU_InputManager::Initialise()
{

	UWU_Debug::Message("%i joysticks were found.\n\n", DebugMask::High, SDL_NumJoysticks());
	UWU_Debug::Message("The names of the joysticks are:\n", DebugMask::High);


	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		UWU_Debug::Message("    %s\n", DebugMask::High, SDL_JoystickNameForIndex(i));
	}

	SDL_JoystickEventState(SDL_ENABLE);
	_controller = SDL_GameControllerOpen(0);
	_haptic = SDL_HapticOpen(0);
	if (SDL_HapticRumbleInit(_haptic) < 0) {
		UWU_Debug::Error("Haptic rumble init failed: %s", DebugMask::High, SDL_GetError());
	}

	BindAction(B_Left, SDLK_LEFT);
	BindAction(B_Right, SDLK_RIGHT);
	BindAction(B_Left, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	BindAction(B_Right, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	BindAction(A_XMovement, SDL_CONTROLLER_AXIS_LEFTX);
	BindAction(B_Dash, SDLK_c);
	BindAction(B_Dash, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	BindAction(B_Jump, SDL_CONTROLLER_BUTTON_A);
	BindAction(B_Jump, SDLK_z);
	BindAction(B_Attack, SDLK_x);
	BindAction(B_Attack, SDL_CONTROLLER_BUTTON_X);
	//BindAction(B_DebugToggle, SDLK_l);
	//BindAction(B_DebugLag, SDLK_SEMICOLON);
	BindAction(B_DebugAction1, SDLK_q);
	BindAction(B_DebugAction2, SDLK_w);
	BindAction(B_DebugAction3, SDLK_e);
	BindAction(B_DebugAction4, SDLK_r);
	BindAction(B_DebugAction5, SDLK_f);
	BindAction(B_SoundToggle, SDLK_m);

	UWU_Debug::DrawGap(DebugMask::High);
	UWU_Debug::DrawSeperator("Keyboard Map", DebugMask::High);

	for (int i = MAX_AXIS + 1; i < NULL_ACTION; i++) {
		InputAction action = InputAction(i);
		if (!_keyboardBindings.count(action)) {
			UWU_Debug::Error("No keyboard binding for action: %s", DebugMask::High, InputActionNames[action]);
			_keyboardBindings[action] = -1;
		}
		else {
			UWU_Debug::Message("Keyboard binding for action: %s is %s", DebugMask::High, InputActionNames[action], KeyboardKeyNames[_keyboardBindings[action]]);
		}
	}

	UWU_Debug::DrawSeperator("Controller Map", DebugMask::High);

	for (int i = 0; i < NULL_ACTION; i++) {
		InputAction action = InputAction(i);
		if (!_controllerBindings.count(action)) {
			UWU_Debug::Error("No controller binding for action: %s", DebugMask::High, InputActionNames[action]);
			_controllerBindings[action] = -1;
		}
		else {
			UWU_Debug::Message("Controller binding for action: %s is %s", DebugMask::High, InputActionNames[action], ControllerButtonNames[_controllerBindings[action]]);
		}
	}

	UWU_Debug::DrawGap(DebugMask::High);
}

bool UWU_InputManager::ProcessEvents()
{
	SDL_Event event;
	_keyboardPressedThisFrame.clear();
	_controllerPressedThisFrame.clear();
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			return true;
			break;
		case SDL_KEYDOWN:

			if (_keyboardStates[event.key.keysym.sym] == 1) {
				continue;
			}
			_keyboardStates[event.key.keysym.sym] = 1;
			_keyboardPressedThisFrame[event.key.keysym.sym] = true;
			UWU_Debug::Message("Key Down: %s", DebugMask::Medium, KeyboardKeyNames[event.key.keysym.sym]);
			if (event.key.keysym.sym >= SDL_KeyCode::SDLK_1 && event.key.keysym.sym <= SDL_KeyCode::SDLK_0 + DebugMask::END) {
				UWU_Debug::ToggleDebugMask(event.key.keysym.sym - 48);
			}
			break;
		case SDL_KEYUP:

			_keyboardStates[event.key.keysym.sym] = false;
			break;
		case SDL_JOYDEVICEADDED:

			if (_controller == nullptr) {
				UWU_Debug::Message("Controller reconnected", DebugMask::High);
				_controller = SDL_GameControllerOpen(0);
			}
			break;
		case SDL_JOYDEVICEREMOVED:
			UWU_Debug::Message("Controller disconnected", DebugMask::High);
			_controller = nullptr;
			break;
		case SDL_CONTROLLERAXISMOTION:
			_controllerStates[event.caxis.axis + SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX] = AxisToScaled(event.caxis.value);
			if (event.caxis.value < 3200 && event.caxis.value > -3200) { //Arbitrary value for game feel
				_controllerAxisHeld[event.caxis.axis] = false;
			}
			else if (!_controllerAxisHeld[event.caxis.axis]) {
				_controllerPressedThisFrame[event.caxis.axis + SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX] = true;
				_controllerAxisHeld[event.caxis.axis] = true;
			}
			break;
		case SDL_CONTROLLERBUTTONDOWN:

			if (_controllerStates[event.key.keysym.sym] == 1) {
				continue;
			}
			UWU_Debug::Message("Button Down: %i", DebugMask::Medium, event.cbutton.button);
			_controllerStates[event.cbutton.button] = 1;
			_controllerPressedThisFrame[event.cbutton.button] = true;
			break;
		case SDL_CONTROLLERBUTTONUP:
			UWU_Debug::Message("Button Up: %i", DebugMask::Medium, event.cbutton.button);
			_controllerStates[event.cbutton.button] = false;
			break;

			case SDL_WINDOWEVENT:

				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_CLOSE:
						return true;
						break;
				//case SDL_WINDOWEVENT_ENTER:
				//	UWU_Debug::Message("Window %d gained keyboard focus", DebugMask::High,
				//		event.window.windowID);
				//	SDL_SetWindowAlwaysOnTop(SDL_GetWindowFromID(event.window.windowID), SDL_TRUE);

				//	break;
				//case SDL_WINDOWEVENT_LEAVE:
				//	UWU_Debug::Message("Window %d lost keyboard focus", DebugMask::High,
				//		event.window.windowID);
				//	SDL_SetWindowAlwaysOnTop(SDL_GetWindowFromID(event.window.windowID), SDL_FALSE);
				//	break;
				}
		}

	}
	return false;
}

void UWU_InputManager::BindAction(InputAction action, SDL_Keycode key)
{
	_keyboardBindings[action] = key;
}

void UWU_InputManager::BindAction(InputAction action, SDL_GameControllerButton button)
{
	_controllerBindings[action] = button;
}

void UWU_InputManager::BindAction(InputAction action, SDL_GameControllerAxis axis)
{
	_controllerBindings[action] = axis + SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX;
}

float UWU_InputManager::GetActionState(InputAction action) {
	if (_controller == nullptr) {

		return _keyboardStates[_keyboardBindings[action]];
	}
	return _controllerStates[_controllerBindings[action]];
}

float UWU_InputManager::GetActionStateOnce(InputAction action) {
	if (_controller == nullptr) {

		return _keyboardPressedThisFrame[_keyboardBindings[action]];
	}
	return std::clamp(_controllerPressedThisFrame[_controllerBindings[action]] + GetAxisActionStateOnce(action), 0.f, 1.f);
}

float UWU_InputManager::GetAxisActionState(InputAction action) {
	if (_controller == nullptr) {

		return GetActionState((InputAction)((int)action + 1)) - GetActionState((InputAction)((int)action + 2));
	}
	return _controllerStates[_controllerBindings[action]];
}

float UWU_InputManager::GetAxisActionStateOnce(InputAction action) {
	if (_controller == nullptr) {

		return GetActionStateOnce((InputAction)((int)action + 1)) - GetActionStateOnce((InputAction)((int)action + 2));
	}
	return _controllerPressedThisFrame[_controllerBindings[action]];
}

int2 UWU_InputManager::GetMousePosition()
{
	int2 mousePos;
	SDL_GetGlobalMouseState(&mousePos.x, &mousePos.y);
	return mousePos;
}

float UWU_InputManager::AxisToScaled(Sint16 value) {
	float scaled = value / AXIS_MAX;
	if (scaled < 0.2f && scaled > -0.2f) {
		scaled = 0;
	}
	return scaled;
}
