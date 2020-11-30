#pragma once

#include "graphics.hpp"
#include "input.hpp"

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
	bool map_selector_menu();

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

	bool map_selector(const std::string &graveyard_map_btn_path, const std::string &grassland_map_btn_path, const std::string &graveyard_1, const std::string &grassland_2);


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

	void handleCollisions();
	int framerate;
	bool isRunning;
	bool isGrassland;

	Graphics graphics;
	Input input;
	SDL_Event event;

	// Custom game classes
	// End of custom classes
};