#ifndef __ZOBRIST_H__
#define __ZOBRIST_H__

#include "Types.h"

namespace Zobrist
{

void Initialise();
uint64_t Silver();
uint64_t Key(Square sq, int loc);

}

#endif // __ZOBRIST_H__
