// #pragma once

// #include "graphics.hpp"
// #include "input.hpp"
// #include "color.hpp"

// // Game Objects

// #include "map.hpp"

// struct SDL_Window;
// struct SDL_Renderer;

// class Game
// {
// public:
// 	Game();
// 	~Game();

// 	bool init();

// 	/*
// 		Destroy current renderer and render, unload textures from GPU
// 	 */
// 	void exit();

// 	/*
// 		Returns true if game is currently running
// 	 */
// 	bool running();

// 	/*
// 		Runs main game loop
// 	 */
// 	void run();

// 	// Setters and Getters

// 	void setFramerate(int framerate);

// private:
// 	/*
// 		Configure starting game conditions such as spawn point and labirinth
// 	 */
// 	void startGame();

// 	/*
// 		Handles user input as SDL's events
// 	*/
// 	void handleUserInput();

// 	/*
// 		Update all game objects
// 	 */
// 	void update();

// 	/*
// 		Render all game objects to screen
// 	 */
// 	void render();

// 	/*
// 		Process collisions between game objects
// 	 */
// 	void handleCollisions();
// 	int _framerate;

// 	bool _isRunning;
// 	Graphics *_graphics;
// 	Input _input;

// 	SDL_Event _event;

// 	// Custom game classes
// 	Map *_map;
// 	// End of custom classes
// };
