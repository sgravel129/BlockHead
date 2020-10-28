#include "game.hpp"
#include "util.hpp"
#include "log.hpp"
#include "input.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

namespace GameVariables
{
	const char *GAME_NAME = "BlockHead";
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
}

Game::Game()
{
}

Game::~Game()
{
}

bool Game::init()
{
	int flags = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Log::error("Game::init | SDL Subsystems failed to init: " + std::string(SDL_GetError()));
		return false;
	}

	Log::verbose("Game::init | SDL Subsystems initialized");

	_graphics = new Graphics(GameVariables::GAME_NAME, GameVariables::SCREEN_WIDTH, GameVariables::SCREEN_HEIGHT);
	_graphics->setRenderColor(Color::white());

	/* Custom class initialization */
	// _sprite = Sprite(*_graphics, "assets/flowers.bmp", 100, 100, 2.0F);

	/* End of class initialization */

	_isRunning = true;
	return true;
}

void Game::handleUserInput()
{
	_input.beginNewFrame();

	if (SDL_PollEvent(&_event))
	{
		if (_event.type == SDL_QUIT)
		{
			exit();
		}
		else if (_event.type == SDL_KEYDOWN)
		{
			if (_event.key.repeat == 0)
				_input.keyDownEvent(_event);
		}
		else if (_event.type == SDL_KEYUP)
		{
			_input.keyUpEvent(_event);
		}
	}

	if (_input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		exit();
	}
}

// int x = 200;
// int y = 200;

void Game::update()
{
	/* Updating of game classes */
	// if(_input.isKeyHeld(SDL_SCANCODE_W))
	// 	y -= 1;
	// if(_input.isKeyHeld(SDL_SCANCODE_S))
	// 	y += 1;
	// if(_input.isKeyHeld(SDL_SCANCODE_D))
	// 	x += 1;
	// if(_input.isKeyHeld(SDL_SCANCODE_A))
	// 	x -= 1;
	/* End of updating */
}

void Game::render()
{
	_graphics->fillBackground();

	/* Rendering of different classes */
	// _sprite.draw(*_graphics, x, y);

	/* End of rendering */
	_graphics->flip();
}

void Game::exit()
{
	_isRunning = false;
	SDL_Quit();
}

bool Game::running()
{
	return _isRunning;
}

void Game::run()
{
	unsigned int last = SDL_GetTicks();
	unsigned int current;

	while (_isRunning)
	{
		current = SDL_GetTicks();

		handleUserInput();
		update();

		if (current - last >= (1000 / _framerate))
		{
			render();
			last = current;
		}

		SDL_Delay(10);
	}
}

// Getters and Setters

void Game::setFramerate(int framerate)
{
	_framerate = framerate;
}