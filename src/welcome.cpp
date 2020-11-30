// #include "welcome.hpp"
// #include "util.hpp"
// #include "log.hpp"
// #include "input.hpp"
// #include "constants.hpp"

// // TODO: Framerate bug
// // TODO: Removed default constructors?
// // TODO: Log option just for destructors.

// Welcome::Welcome()
// {
// }

// Welcome::~Welcome()
// {
// 	Log::debug("~Welcome\t| Called ");
// 	_map->~Map();
// 	_graphics->~Graphics();
// }

// bool Welcome::init()
// {
// 	int flags = 0;

// 	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
// 	{
// 		Log::error("Welcome::init | SDL Subsystems failed to init: " + std::string(SDL_GetError()));
// 		return false;
// 	}

// 	Log::verbose("Welcome::init | SDL Subsystems initialized");

// 	_graphics = new Graphics(GAME_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
// 	_graphics->setRenderColor(Color("FFFFFF"));

// 	/* Custom class initialization */
// 	_map = new Map(Point{10, 10});
// 	_map->loadTextures("assets/open_page.png");
// 	_map->loadMapFile(*_graphics, "res/maps/test.map");

// 	/* End of class initialization */

// 	_isRunning = true;
// 	return true;
// }

// void Welcome::handleUserInput()
// {
// 	_input.beginNewFrame();

// 	if (SDL_PollEvent(&_event))
// 	{
// 		if (_event.type == SDL_QUIT)
// 		{
// 			exit();
// 		}
// 		else if (_event.type == SDL_KEYDOWN)
// 		{
// 			if (_event.key.repeat == 0)
// 				_input.keyDownEvent(_event);
// 		}
// 		else if (_event.type == SDL_KEYUP)
// 		{
// 			_input.keyUpEvent(_event);
// 		}
// 	}

// 	if (_input.wasKeyPressed(SDL_SCANCODE_ESCAPE))
// 	{
// 		exit();
// 	}
// }

// int x = 200;
// int y = 200;

// void Welcome::update()
// {
// 	/* Updating of Welcome classes */
// 	_player->update(_input);
// 	_zombie->update(_player->getX(), _player->getY());
// 	/* End of updating */
// }

// void Welcome::render()
// {
// 	_graphics->fillBackground();

// 	_map->draw(*_graphics);
// 	/* Rendering of different classes */
// 	_player->draw(*_graphics);
// 	_zombie->draw(*_graphics);

// 	/* End of rendering */
// 	_graphics->flip();
// }

// void Welcome::exit()
// {
// 	_isRunning = false;
// 	SDL_Quit();
// }

// bool Welcome::running()
// {
// 	return _isRunning;
// }

// void Welcome::run()
// {
// 	unsigned int last = SDL_GetTicks();
// 	unsigned int current;

// 	while (_isRunning)
// 	{
// 		current = SDL_GetTicks();

// 		handleUserInput();
// 		update();

// 		if (current - last >= (1000 / _framerate))
// 		{
// 			render();
// 			last = current;
// 		}

// 		SDL_Delay(10);
// 	}
// }

// // Getters and Setters

// void Welcome::setFramerate(int framerate)
// {
// 	_framerate = framerate;
// }