#pragma once

#include "graphics.hpp"
#include "input.hpp"
#include "color.hpp"

// Game Objects
#include "player.hpp"
#include "zombie.hpp"
#include "map.hpp"

struct SDL_Window;
struct SDL_Renderer;

class Game
{
public:
	Game();
	~Game();

	bool start_menu();
	bool again_menu();
	bool winner_menu();

	/*
		Destroy current renderer and render, unload textures from GPU
	 */
	void exit();

	/*
		Returns true if game is currently running
	 */
	bool running();

	/*
		Runs main game loop
	 */
	void run();

	// Setters and Getters

	void setFramerate(int framerate);

private:
	
	bool menu(const std::string &background_path, const std::string &exit_button, const std::string &play_button);

	/*
		Configure starting game conditions such as spawn point and labirinth
	 */
	void startGame();

	/*
		Handles user input as SDL's events
	*/
	void handleUserInput();

	/*
		Update all game objects
	 */
	void update();

	/*
		Render all game objects to screen
	 */
	void render();

	/*
		Process collisions between game objects
	 */
	void handleCollisions();
	int framerate;

	bool isRunning;
	Graphics graphics;
	Input input;

	SDL_Event event;

	// Custom game classes
	Player player;
	Zombie zombie;
	Map map;
	// End of custom classes
};
