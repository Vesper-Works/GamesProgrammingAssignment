#ifndef UWU_DEBUG
#define UWU_DEBUG
#include <wtypes.h>
#include <string>
#include "UWU_Window.h"
#include "UWU_Timer.hpp"

enum DebugMask {
	High, //For messages that are important to the game
	Medium, //For message that could be important, but will either fill the console or are optional
	Low, //For messages that are not important, but are useful for debugging, such as constructors/destructors
	END
};

static const std::string DebugMaskNames[] = {
	"High",
	"Medium",
	"Low"
};

inline float RenderSizeMultiplier;
inline int Score;
inline long long MaxTimeLeft = 45LL;
inline UWU_Timer GameTimer;

class UWU_Debug {
public:
	static void Message(const char* message, ...);
	static void Warning(const char*message, ...);
	static void Error(const char* message, ...);
	static void DrawGap(const DebugMask level);
	static void DrawSeperator(const DebugMask level);
	static void DrawSeperator(const char* title, ...);
	static bool Debug();
	static void SetDebug(bool active);
	static void ToggleDebugMask(DebugMask level);
	static void ToggleDebugMask(unsigned int level);
	static void SetWindow(UWU_Window* window);
	static int GetDebugMask();
	static UWU_Window* GetWindow();
private:
	//static bool debugActive;
	UWU_Debug();
	static void ChangeTextColour(int colour);
	static std::string GetCurrentTime();
};
#endif // !UWU_DEBUG

