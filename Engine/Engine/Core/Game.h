#pragma once
#include <SDL2/SDL.H>
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
	bool m_isRunning = false;
	int m_WindowWidth = 800;
	int m_WindowHeight = 600;

	bool m_isFullScreen = false;
};