#include "player.hpp"
#include "log.hpp"

// TODO: Migrate to point system.
const int MOVE_ANIMS = 3;
const int NUM_DIR = 4;

Player::Player()
{
}

Player::Player(Graphics &graphics, const std::string &path,int w, int h, float scale)
{
    _x = 200;
    _y = 200;
    _moveSpeed = 2;
    _angle = 0; // starting direction
    _sprite = new Sprite(graphics, path, SDL_Rect{0, 0, w, h}, scale);
    _currAnim = 0;
    _anims.resize( NUM_DIR, std::vector<SDL_Rect>( MOVE_ANIMS ) );

    for (int i = 0; i < NUM_DIR; i++)
    {
        for(int j = 0; j < MOVE_ANIMS; j++){
            _anims[i][j] = SDL_Rect{w*j, h*i, w, h};
        }
    }
}

Player::~Player()
{
    Log::destruct("Player\t| Called");
    _sprite->~Sprite();
}

int counter = 0;
void Player::update(Input input){
    if(input.isKeyHeld(SDL_SCANCODE_W)){
		_y -= _moveSpeed;
        _angle = 3;
    }
	if(input.isKeyHeld(SDL_SCANCODE_S)){
		_y += _moveSpeed;
        _angle = 0;
    }
	if(input.isKeyHeld(SDL_SCANCODE_D)){
		_x += _moveSpeed;
        _angle = 2;
    }
	if(input.isKeyHeld(SDL_SCANCODE_A)){
		_x -= _moveSpeed;
        _angle = 1;
    }

    if (input.isKeyHeld(SDL_SCANCODE_W) ||
        input.isKeyHeld(SDL_SCANCODE_S) ||
        input.isKeyHeld(SDL_SCANCODE_D) ||
        input.isKeyHeld(SDL_SCANCODE_A))
    {
        counter++;
        if (counter >= 10) {
            _currAnim = (_currAnim + 1) % MOVE_ANIMS;
            counter = 0;
        }
    }
    if( input.wasKeyReleased(SDL_SCANCODE_W) ||
        input.wasKeyReleased(SDL_SCANCODE_S) ||
        input.wasKeyReleased(SDL_SCANCODE_D) ||
        input.wasKeyReleased(SDL_SCANCODE_A))
    {
        _currAnim = 0;
    }
}

void Player::draw(Graphics &graphics){
    _sprite->change_src(_anims[_angle][_currAnim]);
    _sprite->draw(graphics, _x, _y);
}

