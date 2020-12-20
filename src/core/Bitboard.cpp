#include "Bitboard.h"
#include <array>
#include <iostream>
#include <utility>

constexpr uint64_t One = 1;
constexpr int HalfBoard = 40;

BB& BB::operator&=(const BB& other)
{
    lower &= other.lower;
    upper &= other.upper;
    return *this;
}

BB& BB::operator|=(const BB& other)
{
    lower |= other.lower;
    upper |= other.upper;
    return *this;
}

BB& BB::operator^=(const BB& other)
{
    lower ^= other.lower;
    upper ^= other.upper;
    return *this;
}

void BB::Set(int i)
{
    if (i < HalfBoard)
        lower |= (One << i);
    else
        upper |= (One << (i - HalfBoard));
}

void BB::Unset(int i)
{
    if (i < HalfBoard)
        lower &= ~(One << i);
    else
        upper &= ~(One << (i - HalfBoard));
}

bool BB::Test(int i) const
{
    if (i < HalfBoard)
        return lower & (One << i);
    else
        return upper & (One << (i - HalfBoard));
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

void Bitboards::Initialise()
{
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
                BB ray = { 0, 0 };

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
            BB neighbours = { 0, 0 };
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
    BB silverCanMove = { 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF };
    BB redCanMove(silverCanMove);

    // Turn off the locations where silver cannot move.
    for (int i = 0; i < 8; i++)
        silverCanMove.Unset(BoardWidth*i);

    silverCanMove.Unset(8);
    silverCanMove.Unset(78);

    Bitboards::canMove[int(Player::Silver)] = silverCanMove;

    // Turn off the locations where red cannot move.
    for (int i = 0; i < 8; i++)
        redCanMove.Unset(BoardWidth*i + 9);

    redCanMove.Unset(1);
    redCanMove.Unset(71);

    Bitboards::canMove[int(Player::Red)] = redCanMove;
}
