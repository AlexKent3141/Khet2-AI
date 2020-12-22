#include "Zobrist.h"
#include "Globals.h"
#include "RandomGenerator.h"
#include <limits>

static_assert(
    std::numeric_limits<Square>::max() == 0xFF,
    "Square is the wrong size.");

uint64_t silver;
uint64_t keys[0xFF][BoardArea];

static bool initialised = false;

void Zobrist::Initialise()
{
    if (initialised) return;
    initialised = true;

    // Note: This is the seed that Stockfish uses.
    RandomGenerator gen(1070372);
    silver = gen.Next();
    for (int sq = 0; sq < 0xFF; sq++)
    {
        for (int loc = 0; loc < BoardArea; loc++)
        {
            keys[sq][loc] = gen.Next();
        }
    }
}

uint64_t Zobrist::Silver()
{
    return silver;
}

uint64_t Zobrist::Key(Square sq, int loc)
{
    return keys[sq][loc];
}
