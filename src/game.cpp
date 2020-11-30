#include "game.hpp"
#include "util.hpp"
#include "log.hpp"
#include "color.hpp"
#include "input.hpp"
#include "constants.hpp"
#include "animation.hpp"

#include "player.hpp"
#include "zombie.hpp"
#include "map.hpp"

#define NUM_ZOMBIES 5

Game::Game()
		:	graphics(GAME_NAME, SCREEN_WIDTH, SCREEN_HEIGHT),
			framerate(30),
			isRunning(false),
			isGrassland(false)
{
	Log::verbose("Game Constructor\t| SDL Subsystems initialized");
	isRunning = true;
	isGrassland = true;
}

Game::~Game()
{
	Log::destruct("Game\t| Called ");
}

bool Game::map_selector(const std::string &graveyard_map_btn_path, const std::string &grassland_map_btn_path, const std::string &graveyard_1, const std::string &grassland_2)
{
	Sprite graveyard_map_btn(graphics, graveyard_map_btn_path, {0, 0, 600, 600}, 0.7f);
	Sprite grassland_map_btn(graphics, grassland_map_btn_path, {0, 0, 600, 600}, 0.7f);
	Sprite graveyard1_btn(graphics, graveyard_1, {0, 0, 600, 100}, 0.7f);
	Sprite grassland2_btn(graphics, grassland_2, {0, 0, 600, 100}, 0.7f);

	unsigned int last = SDL_GetTicks();
	unsigned int current;

	graphics.setRenderColor(Color("FFFFFF"));
	graphics.fillBackground();

	Map map = Map(Point{10, 10});
    int map_num;
	while (isRunning)
	{
		current = SDL_GetTicks();

		handleUserInput();

		if (input.wasKeyPressed(SDL_SCANCODE_2)){
			isGrassland = true;
			break;
		}
		if (input.wasKeyPressed(SDL_SCANCODE_1)){
			isGrassland = false;
			break;
		}
		if (current - last >= (1000 / framerate))
		{
			graveyard1_btn.draw(graphics, 120, 550);
			grassland2_btn.draw(graphics, 720, 550);
			graveyard_map_btn.draw(graphics, 120, 90);
			grassland_map_btn.draw(graphics, 720, 90);
			graphics.flip();
			last = current;
		}
		SDL_Delay(10);
	}

	return isGrassland;
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

bool Game::map_selector_menu() {
	return map_selector("res/assets/themes_graveyard.png", "res/assets/themes_forest.png", "res/assets/graveyard_theme_button.png", "res/assets/grassland_theme_button.png");
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
	SDL_Quit();
}

bool Game::running()
{
	return isRunning;
}

void Game::run()
{
	Player player = Player(graphics, "res/robot_sprites.png", 1953, 2192, 0.10F);

	// Create Zombies
	Log::verbose("Creating zombies");
	std::vector<Zombie*> zombies;
	for (int i = 0; i < NUM_ZOMBIES; i++)
	{
		zombies.push_back(new Zombie(graphics, "res/zombie.png", 30, 32, 4.0F));
	}

	// Create Map
	Map map = Map(Point{10, 10});

	if (isGrassland){
		map.loadTextures("res/maps/grassland/grassland.png", "res/maps/grassland/grassland.sprites");
		map.loadMapFile(graphics, "res/maps/test.map");
	}
	if (!isGrassland){
		map.loadTextures("res/maps/graveyard/graveyard.png", "res/maps/graveyard/graveyard.sprites");
		map.loadMapFile(graphics, "res/maps/test.map");
	}

	unsigned int last = SDL_GetTicks();
	unsigned int current;

	graphics.setRenderColor(Color("65846c"));

	while (isRunning)
	{
		current = SDL_GetTicks();

		// Update
		handleUserInput();
		Animation::updateTicks();
		player.update(input);
		map.update(player.getDeltaPos());

		for (auto& zombie : zombies){
			zombie->update(player.getDeltaPos());
		}

		// Check collision between:
		// player & zombies
		// player & map
		// bullets & zombies

		if (current - last >= (1000 / framerate))
		{
			// Render
			graphics.fillBackground();
			map.draw(graphics);

			for (auto& zombie : zombies){
				zombie->draw(graphics);
			}

			player.draw(graphics);

			// Display
			graphics.flip();
			last = current;
		}

		SDL_Delay(10);
	}

	// Destroy Zombies
	for (auto& zombie : zombies){
		delete zombie;
	}
}

// Getters and Setters

void Game::setFramerate(int framerate)
{
	this->framerate = framerate;
}
