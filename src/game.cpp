#include "game.hpp"
#include "util.hpp"
#include "log.hpp"
#include "input.hpp"
#include "constants.hpp"

#include "player.hpp"
#include "zombie.hpp"
#include "map.hpp"

// TODO: Framerate bug
// TODO: Removed default constructors?
// TODO: Log option just for destructors.

Game::Game()
		:	graphics(GAME_NAME, SCREEN_WIDTH, SCREEN_HEIGHT),
			framerate(30),
			isRunning(false)
{
	Log::verbose("Game Constructor\t| SDL Subsystems initialized");
	isRunning = true;
}

Game::~Game()
{
	Log::debug("~Game\t| Called ");
}

bool Game::menu(const std::string &background_path, const std::string &play_button_path, const std::string &exit_button_path)
{
	Sprite background(graphics, background_path, {0, 256, 1820, 900}, 0.7f);
	Sprite button_1(graphics, play_button_path, {0, 0, 422, 92}, 0.7f);
	Sprite button_2(graphics, exit_button_path, {0, 0, 422, 92}, 0.7f);

	unsigned int last = SDL_GetTicks();
	unsigned int current;

	graphics.setRenderColor(Color("FFFFFF"));

	while (isRunning)
	{
		current = SDL_GetTicks();

		// Update
		handleUserInput();
		if (input.wasKeyPressed(SDL_SCANCODE_Y))
			break;

		if (input.wasKeyPressed(SDL_SCANCODE_N))
			isRunning = false;

		if (current - last >= (1000 / framerate))
		{
			// Render
			graphics.fillBackground();
			background.draw(graphics, 0, 100);
			button_1.draw(graphics, 650, 340);
			button_2.draw(graphics, 650, 420);

			// Display
			graphics.flip();
			last = current;
		}

		SDL_Delay(10);
	}

	return isRunning;
}

bool Game::start_menu() {
	return menu("res/assets/open_page.png", "res/assets/start_button.png", "res/assets/quit_button.png");
}

bool Game::again_menu() {
	return menu("res/assets/game_over.png", "res/assets/playagain_button.png", "res/assets/quit_button.png");
}

bool Game::winner_menu() {
	return menu("res/assets/winner.png", "res/assets/nextlevel_button.png", "res/assets/quit_button.png");
}

void Game::handleUserInput()
{
	input.beginNewFrame();

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			exit();
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.repeat == 0)
				input.keyDownEvent(event);
		}
		else if (event.type == SDL_KEYUP)
		{
			input.keyUpEvent(event);
		}
	}

	if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
	{
		exit();
	}
}

void Game::exit()
{
	isRunning = false;
}

bool Game::running()
{
	return isRunning;
}

int x = 200;
int y = 200;

void Game::run()
{
	Player player = Player(graphics, "res/robot_sprites.png", 2110, 2160, 0.10F);
	Zombie zombie = Zombie(graphics, "res/zombie.png", 30, 32, 4.0F);
	Map map = Map(Point{10, 10});

	map.loadTextures("res/maps/graveyard/graveyard.png", "res/maps/graveyard/graveyard.sprites");
	map.loadMapFile(graphics, "res/maps/test.map");

	unsigned int last = SDL_GetTicks();
	unsigned int current;

	graphics.setRenderColor(Color("65846c"));

	while (isRunning)
	{
		current = SDL_GetTicks();

		// Update
		handleUserInput();
		player.update(input);
		zombie.update(player.getX(), player.getY());

		if (current - last >= (1000 / framerate))
		{
			// Render
			graphics.fillBackground();
			map.draw(graphics);
			player.draw(graphics);
			zombie.draw(graphics);

			// Display
			graphics.flip();
			last = current;
		}

		SDL_Delay(10);
	}
}

// Getters and Setters

void Game::setFramerate(int framerate)
{
	this->framerate = framerate;
}