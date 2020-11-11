#include "game.hpp"
#include "util.hpp"
#include "log.hpp"
#include "input.hpp"
#include "constants.hpp"

// TODO: Framerate bug
// TODO: Removed default constructors?
// TODO: Log option just for destructors.

Game::Game()
{
}

Game::~Game()
{
	Log::debug("~Game\t| Called ");
	_player->~Player();
	_zombie->~Zombie();
	_map->~Map();
	_graphics->~Graphics();
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

	_graphics = new Graphics(GAME_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
	_graphics->setRenderColor(Color("65846c"));

	/* Custom class initialization */
	_player = new Player(*_graphics, "res/zombie.png", 30, 32, 4.0F);
	_zombie = new Zombie(*_graphics, "res/zombie.png", 30, 32, 4.0F);
	_map = new Map(Point{10, 10});
	_map->loadTextures("res/maps/graveyard/graveyard.png", "res/maps/graveyard/graveyard.sprites");
	_map->loadMapFile(*_graphics, "res/maps/test.map");

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

int x = 200;
int y = 200;

void Game::update()
{
	/* Updating of game classes */
	_player->update(_input);
	_zombie->update(_player->getX(), _player->getY());
	/* End of updating */
}

void Game::render()
{
	_graphics->fillBackground();

	_map->draw(*_graphics);
	/* Rendering of different classes */
	_player->draw(*_graphics);
	_zombie->draw(*_graphics);

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