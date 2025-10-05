#pragma once
#include <SDL2/SDL.H>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class Game
{

public:
	Game();
	~Game();
	bool Init(const string&, int width, int height);
	void Run();
	void ShutDown();

private:
	void ProcessInput();
	void Update(float deltaTime);
	void Render();

private:
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	SDL_Texture* LoadTexture(const string& path);

	struct Vec2 { float x{ 0.f }, y{ 0.f }; };
	SDL_Texture* m_BrickTex = nullptr;
	SDL_Rect m_BrickDst{ 0,0,0,0 };
	Vec2 m_BrickPos{ 100.f, 100.f };
	Vec2 m_BrickVel{ 2.f,1.f };

	bool m_isRunning = false;
	int m_WindowWidth = 800;
	int m_WindowHeight = 600;

	bool m_isFullScreen = false;
};