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
    int screenDir = Util::randInt(0, 3);
    switch (screenDir)
    {
    case 0:
        pos = Point{Util::randInt(-150, -20), Util::randInt(-100, SCREEN_HEIGHT+100)};
        break;
    case 1:
        pos = Point{Util::randInt(SCREEN_WIDTH+20, SCREEN_WIDTH+150), Util::randInt(-100, SCREEN_HEIGHT+100)};
        break;
    case 2:
        pos = Point{Util::randInt(-100, SCREEN_WIDTH+100), Util::randInt(-150, -20)};
        break;
    case 4:
        pos = Point{Util::randInt(-100, SCREEN_WIDTH+100), Util::randInt(SCREEN_HEIGHT+20, SCREEN_HEIGHT+150)};
        break;
    }
    rPos = pos;
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
    updateCamera(delta); // delta caused by player movement
    Point prevPos = pos;

    static const int DEADBAND = 5;
    static const Point target = Point{int(SCREEN_WIDTH/2) - int(PLAYER_WIDTH/2), int(SCREEN_HEIGHT/2) - int(PLAYER_HEIGHT/2)};
    if ((rPos.y - target.y) > DEADBAND)
    {
        // need to move UP
        pos.y -= moveSpeed;
        angle = 3;
    }
    else if ((rPos.y - target.y) < DEADBAND)
    {
        // need to move DOWN
        pos.y += moveSpeed;
        angle = 0;
    }
    if ((rPos.x - target.x) < DEADBAND)
    {
        // need to move LEFT
        pos.x += moveSpeed;
        angle = 2;
    }
    else if ((rPos.x - target.x) > DEADBAND)
    {
        // need to move RIGHT
        pos.x -= moveSpeed;
        angle = 1;
    }

    rPos = rPos + (pos - prevPos); // delta caused by zombie movement


    if (Animation::getTicks() % int(20 / moveSpeed) == 0) {
        currAnim = (currAnim + 1) % MOVE_ANIMS;
        // Log::verbose(std::to_string((pos.x / 75)) + " " + std::to_string((pos.y / 75)));
    }
}

void Zombie::updateCamera(Point delta)
{
    // Do path finding. Euclidian Distance
    rPos = rPos - delta; // delta caused by player movement
}

void Zombie::draw(Graphics &graphics)
{
    sprite->change_src(anims[angle][currAnim]);
    sprite->draw(graphics, rPos.x, rPos.y);
}

SDL_Rect Zombie::getDestRect(){
    Point dim = sprite->getDim();
    return SDL_Rect{rPos.x, rPos.y, dim.x, dim.y};
}
