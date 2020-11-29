#include "zombie.hpp"
#include "log.hpp"
#include "util.hpp"
#include "animation.hpp"
#include "constants.hpp"

// TODO: Migrate to point system.
const int MOVE_ANIMS = 3;
const int NUM_DIR = 4;

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
    Log::destruct("Zombie\t| Called");
    delete sprite;
}

Zombie::Zombie(Graphics &graphics, const std::string &path, int w, int h, float scale)
{
    pos = Point{Util::randInt(0, SCREEN_WIDTH), Util::randInt(0, SCREEN_HEIGHT)};
    angle = 0; // starting direction
    currAnim = 0;
    moveSpeed = 1;

    sprite = new Sprite(graphics, path, SDL_Rect{0, 0, w, h}, scale);
    anims.resize( NUM_DIR, std::vector<SDL_Rect>( MOVE_ANIMS ) );

    for (int i = 0; i < NUM_DIR; i++)
    {
        for(int j = 0; j < MOVE_ANIMS; j++){
            anims[i][j] = SDL_Rect{w*j+96, h*i, w, h};
        }
    }
}
void Zombie::update(Point delta)
{
    // Do path finding. Euclidian Distance
    pos = pos - delta;

    static const int DEADBAND = 5;
    static const Point target = Point{int(SCREEN_WIDTH/2) - int(PLAYER_WIDTH/2), int(SCREEN_HEIGHT/2) - int(PLAYER_HEIGHT/2)};
    if ((pos.y - target.y) > DEADBAND)
    {
        // need to move UP
        pos.y -= moveSpeed;
        angle = 3;
    }
    else if ((pos.y - target.y) < DEADBAND)
    {
        // need to move DOWN
        pos.y += moveSpeed;
        angle = 0;
    }
    if ((pos.x - target.x) < DEADBAND)
    {
        // need to move LEFT
        pos.x += moveSpeed;
        angle = 2;
    }
    else if ((pos.x - target.x) > DEADBAND)
    {
        // need to move RIGHT
        pos.x -= moveSpeed;
        angle = 1;
    }


    if (Animation::getTicks() % int(20 / moveSpeed) == 0) {
        currAnim = (currAnim + 1) % MOVE_ANIMS;
    }
}

void Zombie::draw(Graphics &graphics)
{
    sprite->change_src(anims[angle][currAnim]);
    sprite->draw(graphics, pos.x, pos.y);
}
