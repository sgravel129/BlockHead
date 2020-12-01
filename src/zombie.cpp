#include <cmath>
#include "zombie.hpp"
#include "log.hpp"
#include "util.hpp"
#include "animation.hpp"
#include "constants.hpp"

#define TLENGTH 75

// TODO: Migrate to point system.
const int MOVE_ANIMS = 3;
const int NUM_DIR = 4;

const int dir = 8;  // number of possible directions to go in
const int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };              // dx and dy representing 8 cardinal directions, starting East then proceeding clockwise
const int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };


static const int DEADBAND = 5;
static const Point target = Point{ int(SCREEN_WIDTH / 2) - int(PLAYER_WIDTH / 2), int(SCREEN_HEIGHT / 2) - int(PLAYER_HEIGHT / 2) };

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
    Log::destruct("Zombie\t| Called");
    delete sprite;
}

Zombie::Zombie(Graphics &graphics, const std::string &path, int w, int h, float scale, Point playerP) { 
   
    //renderPos = Point{Util::randInt(0, SCREEN_WIDTH), Util::randInt(0, SCREEN_HEIGHT)};
    //renderPos = Point{ 200, 200 };
    //renderPos = Point{ 575,270 };
    
    // Making sure we are not adding a zombie outside the map
    do {
        int screenDir = Util::randInt(0, 3);
        switch (screenDir)
        {
        case 0:
            rPos = Point{ Util::randInt(25, 150), Util::randInt(0, SCREEN_HEIGHT + 100) };
            break;
        case 1:
            rPos = Point{ Util::randInt(SCREEN_WIDTH + 20, SCREEN_WIDTH + 150), Util::randInt(25, SCREEN_HEIGHT + 100) };
            break;
        case 2:
            rPos = Point{ Util::randInt(25, SCREEN_WIDTH + 100), Util::randInt(25, 150) };
            break;
        case 4:
            rPos = Point{ Util::randInt(25, SCREEN_WIDTH + 100), Util::randInt(SCREEN_HEIGHT + 20, SCREEN_HEIGHT + 150) };
            break;
        }
        pos = rPos + playerP - target;
    } while (pos.x <= 0 || pos.y <= 0);
    

    angle = 0; // starting direction
    currAnim = 0;
    moveSpeed = 1;
    
    _scale = scale; _w = w; _h = h;
    
    //pos = { pos.x + int(w * scale / 2), pos.y + int(h * scale / 2) };

    sprite = new Sprite(graphics, path, SDL_Rect{0, 0, w, h}, scale);
    anims.resize( NUM_DIR, std::vector<SDL_Rect>( MOVE_ANIMS ) );

    for (int i = 0; i < NUM_DIR; i++)
    {
        for(int j = 0; j < MOVE_ANIMS; j++){
            anims[i][j] = SDL_Rect{w*j+96, h*i, w, h};
        }
    }
}
void Zombie::update(Point delta, Point playerPos)
{
    //playerPos = Point {playerPos.x+}
    
    
    pos = rPos + playerPos - target;
    //pos = { pos.x + int(_w * _scale / 2), pos.y + int(_h * _scale / 2) };
    
    updateCamera(delta); // delta caused by player movement
    Point prevPos = pos;


    
    if (!_pathToPlayer.empty()) {
        rPos.x += dx[_pathToPlayer[0]] * moveSpeed;
        rPos.y += dy[_pathToPlayer[0]] * moveSpeed;
        if (_pathToPlayer[0] == 0 || _pathToPlayer[0] == 1 || _pathToPlayer[0] == 7) angle = 2;
        else if (_pathToPlayer[0] == 4 || _pathToPlayer[0] == 5 || _pathToPlayer[0] == 3) angle = 1;
        else if (_pathToPlayer[0] == 2) angle = 0;
        else angle = 3;
    }
    /*
    if (abs(playerPos.x - pos.x) < DEADBAND && abs(playerPos.y - pos.y) < DEADBAND) {
        if (pos.y < playerPos.y)
            angle = 0;
        else
            angle = 3;
        if (pos.x < playerPos.x)
            angle = 2;
        else
            angle = 1;
    }
    */

    if (posToTile(pos) != posToTile(prevPos))   // when crossed over a full tile, pop the first element of path array
        _pathToPlayer.erase(_pathToPlayer.begin());     
    
    /*
    if ((pos.y - target.y) > DEADBAND)
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
<<<<<<< HEAD
    */

    if (Animation::getTicks() % int(20 / moveSpeed) == 0) {
        currAnim = (currAnim + 1) % MOVE_ANIMS;
        // Log::verbose(std::to_string((pos.x / 75)) + " " + std::to_string((pos.y / 75)));
    }
    
}

void Zombie::updateCamera(Point delta)
{
    rPos = rPos - delta; // delta caused by player movement
}

void Zombie::draw(Graphics &graphics)
{
    sprite->change_src(anims[angle][currAnim]);
    sprite->draw(graphics, rPos.x, rPos.y);
}

void Zombie::setPath(const std::vector<int>& path) { _pathToPlayer = path; }


// VERY IMPORTANT FUNCTION
// translates sprite pixel position (which is typically top left corner) to tile position
// can change values here if we want middle of sprite to be center of collision, or feet!
Point Zombie::posToTile(const Point& p) {
    double x = (static_cast<double>(p.x) + _w * _scale / 2) / static_cast<double>(TLENGTH);
    double y = (static_cast<double>(p.y) + _h * _scale * 0.95) / static_cast<double>(TLENGTH);
    return { static_cast<int>(floor(x)), static_cast<int>(floor(y)) };
}

SDL_Rect Zombie::getDestRect(){
    Point dim = sprite->getDim();
    return SDL_Rect{rPos.x, rPos.y, dim.x, dim.y};
}

