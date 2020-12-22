#include "Bitboard.h"
#include <cassert>
#include <array>
#include <iostream>
#include <utility>

constexpr uint64_t One = 1;
constexpr int WholeBoard = 80;
constexpr int HalfBoard = WholeBoard/2;
constexpr uint64_t First40Bits = 0xFFFFFFFFFF;

BB::BB()
:
    lower_(0),
    upper_(0)
{
}

BB::BB(const BB& other)
:
    lower_(other.lower_),
    upper_(other.upper_)
{
}

BB& BB::operator==(const BB& other)
{
    if (this != &other)
    {
        lower_ = other.lower_;
        upper_ = other.upper_;
    }

    return *this;
}

BB BB::operator~()
{
    BB inverted;
    inverted.lower_ = (~lower_ & First40Bits);
    inverted.upper_ = (~upper_ & First40Bits);
    return inverted;
}

BB& BB::operator&=(const BB& other)
{
    lower_ &= other.lower_;
    upper_ &= other.upper_;
    return *this;
}

BB& BB::operator|=(const BB& other)
{
    lower_ |= other.lower_;
    upper_ |= other.upper_;
    return *this;
}

BB& BB::operator^=(const BB& other)
{
    lower_ ^= other.lower_;
    upper_ ^= other.upper_;
    return *this;
}

void BB::Set(int i)
{
    assert(i < WholeBoard);
    if (i < HalfBoard)
        lower_ |= (One << i);
    else
        upper_ |= (One << (i - HalfBoard));
}

void BB::Unset(int i)
{
    assert(i < WholeBoard);
    if (i < HalfBoard)
        lower_ &= ~(One << i);
    else
        upper_ &= ~(One << (i - HalfBoard));
}

void BB::Reset()
{
    lower_ = 0;
    upper_ = 0;
}

bool BB::Test(int i) const
{
    assert(i < WholeBoard);
    if (i < HalfBoard)
        return lower_ & (One << i);
    else
        return upper_ & (One << (i - HalfBoard));
}

int BB::LSB() const
{
    return lower_
        ? __builtin_ctzll(lower_)
        : __builtin_ctzll(upper_) + HalfBoard;
}

int BB::MSB() const
{
    return upper_
        ? 63 - __builtin_clzll(upper_) + HalfBoard
        : 63 - __builtin_clzll(lower_);
}

int BB::PopLSB()
{
    int bit = BB::NoBit;
    if (lower_)
    {
        bit = __builtin_ctzll(lower_);
        lower_ &= ~(One << bit);
    }
    else if (upper_)
    {
        bit = __builtin_ctzll(upper_);
        upper_ &= ~(One << bit);
        bit += HalfBoard;
    }

    return bit;
}

int BB::PopMSB()
{
    int bit = BB::NoBit;
    if (upper_)
    {
        bit = 63 - __builtin_clzll(upper_);
        upper_ &= ~(One << bit);
        bit += HalfBoard;
    }
    else if (lower_)
    {
        bit = 63 - __builtin_clzll(lower_);
        lower_ &= ~(One << bit);
    }

    return bit;
}

int BB::PopCount() const
{
    return __builtin_popcountll(lower_) + __builtin_popcountll(upper_);
}

BB operator&(BB a, const BB& b) { return a &= b; }
BB operator|(BB a, const BB& b) { return a |= b; }
BB operator^(BB a, const BB& b) { return a ^= b; }

BB Bitboards::rays[BoardArea][4];
BB Bitboards::neighbours[BoardArea];
BB Bitboards::canMove[2];

void InitialiseRays();
void InitialiseNeighbours();
void InitialiseCanMove();

static bool initialised = false;

void Bitboards::Initialise()
{
    if (initialised) return;
    initialised = true;

    InitialiseRays();
    InitialiseNeighbours();
    InitialiseCanMove();
}

void Bitboards::PrettyPrint(const BB& bb)
{
    for (int y = BoardHeight-1; y >= 0; y--)
    {
        for (int x = 0; x < BoardWidth; x++)
        {
            std::cout << (bb.Test(y*BoardWidth + x) ? "1 " : "0 ");
        }

        std::cout << std::endl;
    }
}

bool OnBoard(int x, int y)
{
    return x >= 0 && x < BoardWidth && y >= 0 && y < BoardHeight;
}

void InitialiseRays()
{
    std::pair<int, int> stepPairs[4] =
    {
        { 0, 1 },
        { 1, 0 },
        { 0, -1 },
        { -1, 0 }
    };

    int x, y, loc, nextX, nextY, dirIndex;
    for (x = 0; x < BoardWidth; x++)
    {
        for (y = 0; y < BoardHeight; y++)
        {
            loc = y*BoardWidth + x;
            for (dirIndex = 0; dirIndex < 4; dirIndex++)
            {
                BB ray;

                const auto& steps = stepPairs[dirIndex];
                nextX = x + steps.first;
                nextY = y + steps.second;
                while (OnBoard(nextX, nextY))
                {
                    ray.Set(nextY*BoardWidth + nextX);
                    nextX += steps.first;
                    nextY += steps.second;
                }

                Bitboards::rays[loc][dirIndex] = ray;
            }
        }
    }
}

void InitialiseNeighbours()
{
    std::pair<int, int> stepPairs[8] =
    {
        { 0, 1 },
        { 1, 0 },
        { 0, -1 },
        { -1, 0 },
        { 1, 1 },
        { 1, -1 },
        { -1, 1 },
        { -1, -1 }
    };

    int x, y, loc, nextX, nextY, dirIndex;
    for (x = 0; x < BoardWidth; x++)
    {
        for (y = 0; y < BoardHeight; y++)
        {
            loc = y*BoardWidth + x;
            BB neighbours;
            for (dirIndex = 0; dirIndex < 8; dirIndex++)
            {
                const auto& steps = stepPairs[dirIndex];
                nextX = x + steps.first;
                nextY = y + steps.second;
                if (OnBoard(nextX, nextY))
                {
                    neighbours.Set(nextY*BoardWidth + nextX);
                }
            }

            Bitboards::neighbours[loc] = neighbours;
        }
    }
}

void InitialiseCanMove()
{
    // Initially turn on all bits.
    BB silverCanMove;
    silverCanMove = ~silverCanMove;

    BB redCanMove(silverCanMove);

    // Turn off the locations where silver cannot move.
    for (int i = 0; i < 8; i++)
        silverCanMove.Unset(BoardWidth*i);

    silverCanMove.Unset(8);
    silverCanMove.Unset(9);
    silverCanMove.Unset(78);

    Bitboards::canMove[int(Player::Silver)] = silverCanMove;

    // Turn off the locations where red cannot move.
    for (int i = 0; i < 8; i++)
        redCanMove.Unset(BoardWidth*i + 9);

    redCanMove.Unset(1);
    redCanMove.Unset(70);
    redCanMove.Unset(71);

    Bitboards::canMove[int(Player::Red)] = redCanMove;
}
