#include "UWU_Window.h"
#include <SDL.h>
#include "UWU_RectUtil.h"
#include "UWU_Debug.h"
UWU_Window::UWU_Window(SDL_Window* window, SDL_Renderer* renderer)
{
	int x, y, w, h;
	SDL_GetWindowPosition(window, &x, &y);
	SDL_GetWindowSize(window, &w, &h);

	_position = int2(x, y);
	_size = int2(w, h);

	RecaclulateRects();

	_window = window;
	_renderer = renderer;

	UWU_Debug::Message("Window created: <%p>", DebugMask::Low, this);
}

UWU_Window::~UWU_Window() {
	UWU_Debug::Message("Window destroyed: <%p>", DebugMask::Low, this);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}

SDL_Rect UWU_Window::GetRenderRect()
{
	return _renderRect;
}

int2 UWU_Window::GetInversePosition(int2 position)
{
	return int2(position.x - _position.x, position.y - _position.y);
}

float2 UWU_Window::GetInversePosition(float2 position)
{
	return float2(position.x - _position.x, position.y - _position.y);
}
float2 UWU_Window::GetHalfInversePosition(float2 position)
{
	return float2(position.x - _position.x / 2.f, position.y - _position.y / 2.f);
}

SDL_Rect UWU_Window::GetFullRect()
{

	return _fullRect;
}

SDL_Window* UWU_Window::GetWindow()
{
	return _window;
}

SDL_Renderer* UWU_Window::GetRenderer()
{
	return _renderer;
}

void UWU_Window::DrawOverlapWindow(UWU_Window* otherWindow) {
	SDL_Rect otherRect = otherWindow->GetFullRect();
	if (UWU_RectUtil::RectOverlap(GetRenderRect(), &otherRect)) {
		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		otherRect.x -= _renderRect.x;
		otherRect.y -= _renderRect.y;
		SDL_RenderDrawRect(_renderer, &otherRect);
	}
}

void UWU_Window::PrepareForRender(int r, int g, int b)
{
	SDL_SetRenderDrawColor(_renderer, r, g, b, 255);
	SDL_RenderClear(_renderer);
}

void UWU_Window::PresentRender()
{
	SDL_SetWindowSize(_window, _size.x, _size.y);
	SDL_SetWindowPosition(_window, _position.x, _position.y);
	SDL_RenderPresent(_renderer);
}

void UWU_Window::SetWindowPosition(int x, int y)
{
	_position = int2(x, y);
	RecaclulateRects();
}

void UWU_Window::SetWindowPosition(int2 pos)
{
	SetWindowPosition(pos.x, pos.y);
}

void UWU_Window::SetWindowSize(int x, int y)
{
	_size = int2(x, y);
	RecaclulateRects();
}

void UWU_Window::SetWindowSize(int2 size)
{
	SetWindowSize(size.x, size.y);
}

void UWU_Window::InitLoadingScreen(int bars)
{
	int barHeight = GetWindowSize().y / bars;

	for (size_t i = 0; i < bars; i++)
	{
		SDL_Rect rect;
		rect.x = 0;
		rect.y = i * barHeight;
		rect.w = GetWindowSize().x;
		rect.h = barHeight;
		_loadingBars.push_back(rect);
	}
}

void UWU_Window::DrawLoadingScreen(float tick)
{
	SDL_SetRenderDrawColor(_renderer, 171, 117, 55, 255);
	SDL_Rect rect;
	rect.x = -1000;
	rect.y = -1000;
	rect.w = 100000;
	rect.h = 100000;
	SDL_RenderFillRect(_renderer, &rect);
	int barsCount = _loadingBars.size();
	SDL_SetRenderDrawColor(_renderer, 240, 139, 25, 255);
	for (int i = 0; i < barsCount; i++)
	{
		_loadingBars[i].w = std::lerp(_loadingBars[i].w, GetWindowSize().x * (std::sin(tick + (i / (barsCount * 0.2f))) + 1.0f) / 2.0f, 0.2f);
		SDL_RenderFillRect(_renderer, &_loadingBars[i]);
	}
}

void UWU_Window::DestroyLoadingScreen()
{
	_loadingBars.clear();
}

bool UWU_Window::IsOverlapping(int2 position, int2 size)
{
	SDL_Rect rect(position.x, position.y, size.x, size.y);
	return IsOverlapping(rect);
}

bool UWU_Window::IsOverlapping(const SDL_Rect& otherRect)
{
	//returns whether the other rect overlaps with this window
	return UWU_RectUtil::RectOverlap(GetRenderRect(), &otherRect) || SDL_GetWindowFlags(_window) & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP);
}

int2 UWU_Window::GetWindowPosition()
{
	return _position;
}

int2 UWU_Window::GetWindowSize()
{
	return _size;
}

void UWU_Window::RecaclulateRects()
{
	_renderRect.x = _position.x;
	_renderRect.y = _position.y;
	_renderRect.h = _size.x;
	_renderRect.w = _size.y;

	_fullRect = _renderRect;
	_fullRect.y -= 30;
	_fullRect.h += 30;
}
