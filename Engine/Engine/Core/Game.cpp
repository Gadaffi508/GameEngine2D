#include "Game.h"
#include <iostream>

using namespace std;

Game::Game()
{

}

Game::~Game()
{
	ShutDown();
}

bool Game::Init(const string& title, int width, int height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		cerr << "SDL_INIT failed" << SDL_GetError() << endl;
		return false;
	}

	m_Window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN);

	if (!m_Window)
	{
		cerr << "SDL_Creation failed" << SDL_GetError() << endl;
		return false;
	}

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

	if (!m_Renderer)
	{
		cerr << "Renderer failed" << SDL_GetError() << endl;
		return false;
	}

	m_isRunning = true;
	return true;
}

void Game::Run()
{
	while (m_isRunning)
	{
		ProcessInput();
		Update(0.0f);
		Render();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;


	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			cout << "[EVENT] Quit event" << endl;
			m_isRunning = false;
			break;
		case SDL_KEYDOWN:
			cout << "[EVENT] Key pressed : " << SDL_GetKeyName(event.key.keysym.sym) << endl;
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				m_isRunning = false;
			}
			break;
		case SDL_KEYUP:
			cout << "[EVENT] Key realesed : " << SDL_GetKeyName(event.key.keysym.sym) << endl;
			break;
		case SDL_MOUSEMOTION:
			cout << "[EVENT] Mouse moved : " << event.motion.x << ", " << event.motion.y << endl;
			break;
		case SDL_MOUSEBUTTONDOWN:
			cout << "[EVENT] Mouse button pressed : " << (int)event.button.button << "X=" << event.button.x << "Y=" << event.button.y << endl;
			break;
		default:
			break;
		}
	}
}

void Game::Update(float)
{

}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_Renderer, 30, 30, 30, 255);
	SDL_RenderClear(m_Renderer);
	SDL_RenderPresent(m_Renderer);
}

void Game::ShutDown()
{
	if (m_Renderer)
	{
		SDL_DestroyRenderer(m_Renderer);

		m_Renderer = nullptr;
	}

	if (m_Window)
	{

		SDL_DestroyWindow(m_Window);

		m_Window = nullptr;
	}

	SDL_Quit();
}