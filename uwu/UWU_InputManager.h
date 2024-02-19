#ifndef UWU_INPUTMANAGER
#define UWU_INPUTMANAGER
#include "UWU_Math.h"
#include <unordered_map>
#include <SDL_keycode.h>

const int MAX_AXIS = 0;
enum InputAction {
	A_XMovement,
	B_Right,
	B_Left,
	B_Jump,
	B_Dash,
	B_Attack,
	//B_DebugToggle,
	//B_DebugLag,
	B_DebugAction1,
	B_DebugAction2,
	B_DebugAction3,
	B_DebugAction4,
	B_DebugAction5,
	B_SoundToggle,
	NULL_ACTION
};

static const char* InputActionNames[] = {
	"XMovement",
	"Right",
	"Left",
	"Jump",
	"Dash",
	"Attack",
	//"DebugToggle",
	//"DebugLag",
	"DebugAction1",
	"DebugAction2",
	"DebugAction3",
	"DebugAction4",
	"DebugAction5",
	"Sound Toggle",
	"NULL_ACTION"
};

static std::unordered_map<int, const char*> KeyboardKeyNames = {
	{ SDLK_a, "A" },
	{ SDLK_b, "B" },
	{ SDLK_c, "C" },
	{ SDLK_d, "D" },
	{ SDLK_e, "E" },
	{ SDLK_f, "F" },
	{ SDLK_g, "G" },
	{ SDLK_h, "H" },
	{ SDLK_i, "I" },
	{ SDLK_j, "J" },
	{ SDLK_k, "K" },
	{ SDLK_l, "L" },
	{ SDLK_m, "M" },
	{ SDLK_n, "N" },
	{ SDLK_o, "O" },
	{ SDLK_p, "P" },
	{ SDLK_q, "Q" },
	{ SDLK_r, "R" },
	{ SDLK_s, "S" },
	{ SDLK_t, "T" },
	{ SDLK_u, "U" },
	{ SDLK_v, "V" },
	{ SDLK_w, "W" },
	{ SDLK_x, "X" },
	{ SDLK_y, "Y" },
	{ SDLK_z, "Z" },
	{ SDLK_1, "1" },
	{ SDLK_2, "2" },
	{ SDLK_3, "3" },
	{ SDLK_4, "4" },
	{ SDLK_5, "5" },
	{ SDLK_6, "6" },
	{ SDLK_7, "7" },
	{ SDLK_8, "8" },
	{ SDLK_9, "9" },
	{ SDLK_0, "0" },
	{ SDLK_RETURN, "Return" },
	{ SDLK_ESCAPE, "Escape" },
	{ SDLK_BACKSPACE, "Backspace" },
	{ SDLK_TAB, "Tab" },
	{ SDLK_SPACE, "Space" },
	{ SDLK_MINUS, "-" },
	{ SDLK_EQUALS, "=" },
	{ SDLK_LEFTBRACKET, "[" },
	{ SDLK_RIGHTBRACKET, "]" },
	{ SDLK_BACKSLASH, "\\" },
	{ SDLK_SEMICOLON, ";" },
	{ SDLK_COMMA, "," },
	{ SDLK_PERIOD, "." },
	{ SDLK_SLASH, "/" },
	{ SDLK_CAPSLOCK, "Caps Lock" },
	{ SDLK_UP, "Up" },
	{ SDLK_DOWN, "Down" },
	{ SDLK_LEFT, "Left" },
	{ SDLK_RIGHT, "Right" },
	{ SDLK_LSHIFT, "Left Shift" },
	{ SDLK_RSHIFT, "Right Shift" },
	{ SDLK_LCTRL, "Left Ctrl" },
	{ SDLK_RCTRL, "Right Ctrl" },
	{ SDLK_LALT, "Left Alt" },
	{ SDLK_RALT, "Right Alt" },
	{ SDLK_LGUI, "Left GUI" },
	{ SDLK_RGUI, "Right GUI" },
	{ SDLK_INSERT, "Insert" },
	{ SDLK_HOME, "Home" },
	{ SDLK_PAGEUP, "Page Up" },
	{ SDLK_PAGEDOWN, "Page Down" },
	{ SDLK_DELETE, "Delete" },
	{ SDLK_END, "End" },
	{ SDLK_KP_0, "Keypad 0" },
	{ SDLK_KP_1, "Keypad 1" },
	{ SDLK_KP_2, "Keypad 2" },
	{ SDLK_KP_3, "Keypad 3" },
	{ SDLK_KP_4, "Keypad 4" },
	{ SDLK_KP_5, "Keypad 5" },
	{ SDLK_KP_6, "Keypad 6" },
	{ SDLK_KP_7, "Keypad 7" },
	{ SDLK_KP_8, "Keypad 8" },
	{ SDLK_KP_9, "Keypad 9" },
	{ SDLK_KP_DIVIDE, "Keypad Divide" },
	{ SDLK_KP_MULTIPLY, "Keypad Multiply" },
	{ SDLK_KP_MINUS, "Keypad Minus" },
	{ SDLK_KP_PLUS, "Keypad Plus" },
	{ SDLK_KP_ENTER, "Keypad Enter" },
	{ SDLK_KP_PERIOD, "Keypad Period" },
	{ SDLK_F1, "F1" },
	{ SDLK_F2, "F2" },
	{ SDLK_F3, "F3" },
	{ SDLK_F4, "F4" },
	{ SDLK_F5, "F5" },
	{ SDLK_F6, "F6" },
	{ SDLK_F7, "F7" },
	{ SDLK_F8, "F8" },
	{ SDLK_F9, "F9" },
	{ SDLK_F10, "F10"}
};

static const char* ControllerButtonNames[] = {
	"A",
	"B",
	"X",
	"Y",
	"Back",
	"Guide",
	"Start",
	"Left Stick",
	"Right Stick",
	"Left Shoulder",
	"Right Shoulder",
	"DPad Up",
	"DPad Down",
	"DPad Left",
	"DPad Right",
	"Misc",
	"Paddle One",
	"Paddle Two",
	"Paddle Three",
	"Paddle Four",
	"Touchpad",
	"Left Stick X",
	"Left Stick Y",
	"Right Stick X",
	"Right Stick Y",
	"Left Trigger",
	"Right Trigger"
};



class UWU_InputManager
{
public:
	static UWU_InputManager& getInstance()
	{
		static UWU_InputManager instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
	void Initialise();
	bool ProcessEvents();
	void BindAction(InputAction, SDL_Keycode);
	void BindAction(InputAction, SDL_GameControllerButton);
	void BindAction(InputAction, SDL_GameControllerAxis);
	float GetActionState(InputAction action);
	float GetActionStateOnce(InputAction action);
	float GetAxisActionState(InputAction action);
	float GetAxisActionStateOnce(InputAction action);
	int2 GetMousePosition();
private:
	float AxisToScaled(Sint16);
	float2 leftStickValue;
	float2 rightStickValue;
	float leftTriggerValue;
	float rightTriggerValue;
	UWU_InputManager() {}
	UWU_InputManager(UWU_InputManager const&);
	void operator=(UWU_InputManager const&);
	SDL_GameController* _controller;
	SDL_Haptic* _haptic;
	std::unordered_map<InputAction, int> _keyboardBindings;
	std::unordered_map<InputAction, int> _controllerBindings;
	std::unordered_map<int, float> _keyboardStates;
	std::unordered_map<int, float> _controllerStates;
	std::unordered_map<int, bool> _controllerAxisHeld;
	std::unordered_map<int, bool> _keyboardPressedThisFrame;
	std::unordered_map<int, bool> _controllerPressedThisFrame;
	//std::unordered_map<InputAction, std::unordered_map<void*, void*>> _axisStates;
};

#endif // !UWU_INPUTMANAGER


