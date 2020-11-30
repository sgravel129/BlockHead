#include "animation.hpp"

unsigned int Animation::_ticks = 0; // initialize to zero

unsigned int Animation::getTicks(){
    return _ticks;
}
void Animation::updateTicks(){
    _ticks += 1;
}