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

	bool init();

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
	
	// collision detection(this is where i call my aabb colision check to see if the maptile has collision component and if collision occoured 
	// if yes player spped is set to negative one, sendin them backwards)
	// if (Collision::AABB(player<Collision_Component>, maptile <Colider_componet>))
	//{
	//	player._moveSpeed=-1
	
	//}
	// this function gets called again for zombies collision 
	// if collsion occours health decreases
	// if (Collision::AABB(player, zombie)
	//{
	//	player._moveSpeed=-1
	//	player._health--
	//}
}
	int _framerate;

	bool _isRunning;
	Graphics *_graphics;
	Input _input;

	SDL_Event _event;

	// Custom game classes
	Player *_player;
	Zombie *_zombie;
	Map *_map;
	// End of custom classes
};
