#include "player.hpp"
#include "log.hpp"
#include "animation.hpp"
#include "constants.hpp"

// TODO: Migrate to point system.
const int MOVE_ANIMS = 3;
const int NUM_DIR = 4;

Player::Player()
{
}

Player::Player(Graphics &graphics, const std::string &path,int w, int h, float scale)
{
    Log::verbose("Player\t| Called");

    //pos = Point{525,180};
    pos = Point{ 575,270 };

    _w = w; _h = h; _scale = scale;
    moveSpeed = 3;
    angle = 0; // starting direction
    sprite = new Sprite(graphics, path, SDL_Rect{0, 0, w, h}, scale);
    currAnim = 0;
    anims.resize( NUM_DIR, std::vector<SDL_Rect>( MOVE_ANIMS ) );

    for (int i = 0; i < NUM_DIR; i++)
    {
        for(int j = 0; j < MOVE_ANIMS; j++){
            anims[i][j] = SDL_Rect{w*j, h*i, w, h};
        }
    }
}

Player::~Player()
{
    Log::destruct("Player\t| Called");
    delete sprite;
}

int counter = 0;
void Player::update(Input input){
    prevPos = Point{pos.x, pos.y};

    if(input.isKeyHeld(SDL_SCANCODE_W)){
		pos.y -= moveSpeed;
        angle = 3;
    }
	if(input.isKeyHeld(SDL_SCANCODE_S)){
		pos.y += moveSpeed;
        angle = 0;
    }
	if(input.isKeyHeld(SDL_SCANCODE_D)){
		pos.x += moveSpeed;
        angle = 2;
    }
	if(input.isKeyHeld(SDL_SCANCODE_A)){
		pos.x -= moveSpeed;
        angle = 1;
    }

    if (input.isKeyHeld(SDL_SCANCODE_W) ||
        input.isKeyHeld(SDL_SCANCODE_S) ||
        input.isKeyHeld(SDL_SCANCODE_D) ||
        input.isKeyHeld(SDL_SCANCODE_A))
    {
        if (Animation::getTicks() % int(20 / moveSpeed) == 0) {
            currAnim = (currAnim + 1) % MOVE_ANIMS;
        }
    }
    if( input.wasKeyReleased(SDL_SCANCODE_W) ||
        input.wasKeyReleased(SDL_SCANCODE_S) ||
        input.wasKeyReleased(SDL_SCANCODE_D) ||
        input.wasKeyReleased(SDL_SCANCODE_A))
    {
        currAnim = 0;
    }
}

void Player::draw(Graphics &graphics){
    sprite->change_src(anims[angle][currAnim]);
    sprite->draw(graphics, int(SCREEN_WIDTH/2) - int(PLAYER_WIDTH/2), int(SCREEN_HEIGHT/2) - int(PLAYER_HEIGHT/2));
}
