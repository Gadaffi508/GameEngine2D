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

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!m_Renderer)
	{
		cerr << "Renderer failed" << SDL_GetError() << endl;
		return false;
	}

	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	m_BrickTex = LoadTexture("Assets/Textures/brick.png");
	if (!m_BrickTex)
	{
		return false;
	}

	SDL_QueryTexture(m_BrickTex, nullptr, nullptr, &m_BrickDst.w, &m_BrickDst.h);

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
	m_BrickPos.x += m_BrickVel.x;
	m_BrickPos.y += m_BrickVel.y;

	if (m_BrickPos.x <0 || m_BrickPos.x + m_BrickDst.w > m_WindowWidth)
	{
		m_BrickVel.x *= -1;
	}

	if (m_BrickPos.y <0 || m_BrickPos.y + m_BrickDst.h > m_WindowHeight)
	{
		m_BrickVel.y *= -1;
	}
}

void Game::Render()
{

	SDL_SetRenderDrawColor(m_Renderer, 30, 30, 30, 255);
	SDL_RenderClear(m_Renderer);

	m_BrickDst.x = static_cast<int> (m_BrickPos.x);
	m_BrickDst.y = static_cast<int> (m_BrickPos.y);

	SDL_RenderCopy(m_Renderer,m_BrickTex,nullptr,&m_BrickDst);

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

SDL_Texture* Game::LoadTexture(const string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());

	if (!surface)
	{
		cerr << IMG_GetError() << "\n";
		return nullptr;
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(m_Renderer, surface);
	SDL_FreeSurface(surface);

	return tex;
}