#include "catch2/catch.hpp"
#include "core/Board.h"
#include "core/MoveGenerator.h"
#include <string>

// Make and undo every move and check that the move was reverted correctly.
void MakeUnmake(Board board)
{
    MoveGenerator gen(board);
    int cmp = 0;
    Move move = NoMove;
    while (cmp == 0 && (move = gen.Next()) != NoMove)
    {
        Board original(board);
        board.MakeMove(move);
        board.UndoMove();
        cmp = board.Compare(original);
    }

    REQUIRE(cmp == 0);
}

void RunForPlayer(const std::string& khetPos, Player player)
{
    std::string khetPosFull = khetPos + " " + std::to_string(int(player));
    MakeUnmake(Board(khetPosFull));
}

TEST_CASE("Make-undo for standard position")
{
    const auto& standard = "x33a3ka3p22/2p37/3P46/p11P31s1s21p21P4/p21P41S2S11p11P3/6p23/7P12/2P4A1KA13X1";
    RunForPlayer(standard, Player::Silver);
    RunForPlayer(standard, Player::Red);
}

TEST_CASE("Make-undo for dynasty position")
{
    const auto& dynasty = "x33p3a3p23/5k4/p13p3a3s23/p21s11P41P23/3p41p21S11P4/3S2A1P13P3/4K5/3P4A1P13X1";
    RunForPlayer(dynasty, Player::Silver);
    RunForPlayer(dynasty, Player::Red);
}

TEST_CASE("Make-undo for imhotep position")
{
    const auto& imhotep = "x33a3ka3s22//3P42p13/p1P32P2s22p2P4/p2P42S2p42p1P3/3P32p23//2S2A1KA13X1";
    RunForPlayer(imhotep, Player::Silver);
    RunForPlayer(imhotep, Player::Red);
}

TEST_CASE("Make-undo for position with many captures")
{
    const auto& captures = "x32p2a3ka33/p11p33P43/2P37/7p21P1/4s2S11p11P4/4P11p11S11/3A11P12P31/p12A2K1s12X1";
    RunForPlayer(captures, Player::Silver);
    RunForPlayer(captures, Player::Red);
}
