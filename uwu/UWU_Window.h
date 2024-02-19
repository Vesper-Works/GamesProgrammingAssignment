#ifndef UWU_WINDOW
#define UWU_WINDOW
#include <SDL.h>
#include "UWU_Math.h"
#include <vector>

class UWU_Window
{
public:
	UWU_Window(SDL_Window*, SDL_Renderer*);
	~UWU_Window();
	SDL_Rect GetFullRect();
	SDL_Rect GetRenderRect();
	int2 GetInversePosition(int2);
	float2 GetInversePosition(float2);
	float2 GetHalfInversePosition(float2);
	SDL_Window* GetWindow();
	SDL_Renderer* GetRenderer();
	void DrawOverlapWindow(UWU_Window*);
	void PrepareForRender(int = 0,int = 0,int = 0);
	void PresentRender();
	void SetWindowPosition(int, int);
	void SetWindowSize(int, int);
	void SetWindowPosition(int2);
	void SetWindowSize(int2);
	void InitLoadingScreen(int);
	void DrawLoadingScreen(float);
	void DestroyLoadingScreen();
	bool IsOverlapping(int2, int2);
	bool IsOverlapping(const SDL_Rect&);
	int2 GetWindowPosition();
	int2 GetWindowSize();

private:
	void RecaclulateRects();
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	SDL_Rect _fullRect;
	SDL_Rect _renderRect;
	std::vector<SDL_Rect> _loadingBars;
	int2 _position;
	int2 _size;
};


#endif // UWU_WINDOW

