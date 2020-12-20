#ifndef __KHET_BIT_BOARD_H__
#define __KHET_BIT_BOARD_H__

#include "Globals.h"
#include <cstdint>

// The Khet board is 10x8, so no single integer type can mask the whole board.
// The approach I'm taking is to use one 64 bit word to cover the lower 5x8
// half of the board and another for the upper 5x8 half.
struct BB
{
    uint64_t lower;
    uint64_t upper;

    BB& operator&=(const BB& other);
    BB& operator|=(const BB& other);
    BB& operator^=(const BB& other);

    void Set(int i);
    void Unset(int i);
    bool Test(int i) const;
};

BB operator&(BB a, const BB& b);
BB operator|(BB a, const BB& b);
BB operator^(BB a, const BB& b);

namespace Bitboards
{
    // Rays from each location in each orthogonal direction.
    extern BB rays[BoardArea][4];

    // The neighbours of each location.
    extern BB neighbours[BoardArea];

    // The locations that each player can move to.
    extern BB canMove[2];

    void Initialise();

    void PrettyPrint(const BB&);
}

#endif // __KHET_BIT_BOARD_H__
