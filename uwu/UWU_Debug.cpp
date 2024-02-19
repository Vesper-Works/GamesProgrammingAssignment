#include "UWU_Debug.h"
#include <time.h>
#include <wtypes.h>
#include <ctime>
#include <iostream>
#include <cstdarg>
#include <string>
#include <cstring>
#include <format>

#pragma region Colours
#define BLACK 0x00
#define BLUE 0x01
#define GREEN 0x02
#define CYAN 0x03
#define RED 0x04
#define MAGENTA 0x05
#define YELLOW 0x06
#define LIGHT_GRAY 0x07
#define DARK_GRAY 0x08
#define LIGHT_BLUE 0x09
#define LIGHT_GREEN 0x0A
#define LIGHT_CYAN 0x0B
#define LIGHT_RED 0x0C
#define LIGHT_MAGENTA 0x0D
#define LIGHT_YELLOW 0x0E
#define WHITE 0x0F
#pragma endregion

#define CHECK_BIT(bit) ((currentMask >> bit) & 1U)
using namespace std;

static bool debugActive;
static UWU_Window* DebugWindow;
static int currentMask = ~0;


void UWU_Debug::Message(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	DebugMask type = va_arg(args, DebugMask);
	if (!debugActive || !CHECK_BIT(type)) { return; }
	ChangeTextColour(LIGHT_GREEN);

	vfprintf(stdout, std::format("{0} | {1}\n", GetCurrentTime(), message).c_str(), args);
	va_end(args);
}
void UWU_Debug::Warning(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	DebugMask type = va_arg(args, DebugMask);

	if (!debugActive || !CHECK_BIT(type)) { return; }
	ChangeTextColour(LIGHT_YELLOW);

	vfprintf(stdout, std::format("{0} | {1}\n", GetCurrentTime(), message).c_str(), args);
	va_end(args);
}
void UWU_Debug::Error(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	DebugMask type = va_arg(args, DebugMask);

	if (!debugActive || !CHECK_BIT(type)) { return; }
	ChangeTextColour(LIGHT_RED);

	vfprintf(stdout, std::format("{0} | {1}\n", GetCurrentTime(), message).c_str(), args);
	va_end(args);
}

void UWU_Debug::DrawSeperator(const DebugMask level)
{
	if (!debugActive || !CHECK_BIT(level)) { return; }
	ChangeTextColour(LIGHT_YELLOW);
	std::cout << "\n=======================================================================================================================\n";
}

void UWU_Debug::DrawGap(const DebugMask level)
{
	if (!debugActive || !CHECK_BIT(level)) { return; }
	std::cout << "\n";
}

void UWU_Debug::DrawSeperator(const char* title, ...)
{
	va_list args;
	va_start(args, title);
	DebugMask level = va_arg(args, DebugMask);
	if (!debugActive || !CHECK_BIT(level)) { return; }
	ChangeTextColour(LIGHT_GRAY);
	string formattedTitle = title;
    formattedTitle.insert(0, 60 - (formattedTitle.size()/2), ' ');
	formattedTitle.insert(0, "\n");
    formattedTitle += "\n";
	vfprintf(stdout, formattedTitle.c_str(), args);
	std::cout << "=======================================================================================================================\n";
	va_end(args);
}

bool UWU_Debug::Debug()
{
	return debugActive;
}

void UWU_Debug::SetDebug(bool active)
{
	ChangeTextColour(LIGHT_MAGENTA);
	std::cerr << "[[DEBUG MODE " << (active ? "Active]]" : "Inactive]]") << std::endl;
	debugActive = active;
}

void UWU_Debug::ToggleDebugMask(DebugMask level)
{
	currentMask ^= 1 << (level - 1);
}

void UWU_Debug::ToggleDebugMask(unsigned int level)
{
	currentMask ^= 1U << (level - 1);
}

void UWU_Debug::SetWindow(UWU_Window* window)
{
	DebugWindow = window;
}

UWU_Window* UWU_Debug::GetWindow()
{
	return DebugWindow;
}

UWU_Debug::UWU_Debug() {}
void UWU_Debug::ChangeTextColour(int colour) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, colour);
}


std::string UWU_Debug::GetCurrentTime()
{
	// Get the current time as a time_t value
	time_t now;
	time(&now);

	// Convert the time_t value to a struct tm structure representing the current local time
	struct tm timeinfo;
	char buffer[80];
	if (localtime_s(&timeinfo, &now) == 0) {
		// Format the struct tm structure as a string
		strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &timeinfo);
		return buffer;
	}
	else {
		// Handle the error
		return "<Time Error>";
	}
}

int UWU_Debug::GetDebugMask()
{
	return currentMask;
}
