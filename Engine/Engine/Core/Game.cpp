#include "Game.h"
#include <iostream>
#include <SDL2/SDL.H>

constexpr int TARGET_FPS = 60;
constexpr int FRAME_DELAY = 1000 / TARGET_FPS;

using namespace std;

Game::Game() {}

Game::~Game() { ShutDown(); }

bool Game::Init(const string& title, int width, int height)
{
	m_Physics.Init(PPM, 0.f, 0.f);

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

	m_BrickBody = m_Physics.CreateBox(
		m_BrickPos.x + m_BrickDst.w / 2,
		m_BrickPos.y + m_BrickDst.h / 2,
		(float)m_BrickDst.w, (float)m_BrickDst.h,
		true, 1.f, 0.8f);

	m_Physics.CreateBox(m_WindowWidth / 2, 0, m_WindowWidth, 10, false);
	m_Physics.CreateBox(m_WindowWidth / 2, m_WindowHeight, m_WindowWidth, 10, false);
	m_Physics.CreateBox(0, m_WindowHeight / 2, 10, m_WindowHeight, false);
	m_Physics.CreateBox(m_WindowWidth, m_WindowHeight / 2, 10, m_WindowHeight, false);

	m_isRunning = true;
	return true;
}

void Game::Run()
{
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 freq = SDL_GetPerformanceFrequency();


	Uint32 fpsTimer = SDL_GetTicks();
	int FrameCount = 0;


	while (m_isRunning)
	{
		Uint64 newNow = SDL_GetPerformanceCounter();
		float dt = static_cast <float> (newNow - now) / freq;
		now = newNow;

		FrameCount++;

		Uint32 frameStart = SDL_GetTicks();

		ProcessInput();
		Update(dt);
		Render();

		Uint32 elapsed = SDL_GetTicks() - frameStart;
		if (elapsed < FRAME_DELAY)
		{
			SDL_Delay(FRAME_DELAY - elapsed);
		}

		if (SDL_GetTicks() - fpsTimer >= 1000)
		{
			cout << "FPS : " << FrameCount << endl;
			FrameCount = 0;
			fpsTimer += 1000;
		}
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

		const Uint8* keys = SDL_GetKeyboardState(nullptr);

		m_InputDir = { 0.f,0.f };
		if (keys[SDL_SCANCODE_W]) m_InputDir.y = -1.f;
		if (keys[SDL_SCANCODE_S]) m_InputDir.y = 1.f;
		if (keys[SDL_SCANCODE_A]) m_InputDir.x = -1.f;
		if (keys[SDL_SCANCODE_D]) m_InputDir.x = 1.f;
	}
}

void Game::Update(float dt)
{
	m_Physics.Step(dt);

	b2Vec2 impulse{ m_InputDir.x * 0.5f, m_InputDir.y * 0.5f };
	if (impulse.LengthSquared() > 0)
	{
		m_BrickBody->ApplyLinearImpulseToCenter(impulse, true);
	}

	b2Vec2 p = m_BrickBody->GetPosition();
	m_BrickPos.x = p.x * PPM - m_BrickDst.w / 2;
	m_BrickPos.y = p.y * PPM - m_BrickDst.h / 2;
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_Renderer, 30, 30, 30, 255);
	SDL_RenderClear(m_Renderer);

	m_RQ.Clear();

	m_BrickDst.x = static_cast<int> (m_BrickPos.x);
	m_BrickDst.y = static_cast<int> (m_BrickPos.y);
	m_RQ.Add({m_BrickTex, m_BrickDst, SDL_Rect{0,0,0,0},10});
	
	m_RQ.Flus(m_Renderer);

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

	m_Physics.ShutDown();

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