#include "catch2/catch.hpp"
#include "core/Board.h"
#include "core/MoveGenerator.h"
#include "core/Bitboard.h"
#include "core/Zobrist.h"
#include <iostream>

// Count the number of moves available in the position.
int CountMoves(const Board& board)
{
    Zobrist::Initialise();
    Bitboards::Initialise();

    MoveGenerator gen(board);
    int count = 0;
    Move move = NoMove;
    while ((move = gen.Next()) != NoMove)
    {
        std::cout << GetStart(move) << " "
                  << GetEnd(move) << " "
                  << GetRotation(move) << std::endl;
        ++count;
    }

    return count;
}

void RunForPlayer(const std::string& khetPos, Player player, int expectedMoves)
{
    std::string khetPosFull = khetPos + " " + std::to_string((int)player);
    Board board(khetPosFull);
    int numMoves = CountMoves(board);
    std::cout << "Expected: " << expectedMoves << ", "
              << "Got: " << numMoves << std::endl;

    REQUIRE(numMoves == expectedMoves);
}

TEST_CASE("Move generation standard")
{
    const std::string& standard = "x33a3ka3p22/2p37/3P46/p11P31s1s21p21P4/p21P41S2S11p11P3/6p23/7P12/2P4A1KA13X1";
    RunForPlayer(standard, Player::Silver, 77);
    RunForPlayer(standard, Player::Red, 77);
}

TEST_CASE("Move generation dynasty")
{
    const std::string& dynasty = "x33p3a3p23/5k4/p13p3a3s23/p21s11P41P23/3p41p21S11P4/3S2A1P13P3/4K5/3P4A1P13X1";
    RunForPlayer(dynasty, Player::Silver, 70);
    RunForPlayer(dynasty, Player::Red, 70);
}

TEST_CASE("Move generation imhotep")
{
    const std::string& imhotep = "x33a3ka3s22//3P42p13/p1P32P2s22p2P4/p2P42S2p42p1P3/3P32p23//2S2A1KA13X1";
    RunForPlayer(imhotep, Player::Silver, 73);
    RunForPlayer(imhotep, Player::Red, 73);
}

TEST_CASE("Move generation many captures")
{
    const std::string& captures = "x32p2a3ka33/p11p33P43/2P37/7p21P1/4s2S11p11P4/4P11p11S11/3A11P12P31/p12A2K1s12X1";
    RunForPlayer(captures, Player::Silver, 79);
    RunForPlayer(captures, Player::Red, 71);
}
