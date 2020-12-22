#include "catch2/catch.hpp"
#include "core/Board.h"
#include "core/EvalParams.h"
#include "core/Evaluator.h"
#include "core/MoveGenerator.h"
#include "core/Bitboard.h"
#include "core/Zobrist.h"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

Move GetRandomMove(const Board& board)
{
    // Get all of the moves.
    Move move = NoMove;
    MoveGenerator gen(board);
    std::vector<Move> moves;
    while ((move = gen.Next()) != NoMove)
    {
        moves.push_back(move);
    }

    if (moves.size() > 0)
    {
        // Select a move randomly.
        move = moves[rand() % moves.size()];
    }

    return move;
}

// A termination test consists of playing multiple games with randomly
// selected moves and ensuring that each game terminates as expected.
void RunTerminationTest(const std::string& khetPos, Player player)
{
    // Use fixed seed.
    srand(0xC2E1E8AE);

    Zobrist::Initialise();
    Bitboards::Initialise();

    std::string khetPosFull = khetPos + " " + std::to_string(int(player));

    EvalParams params;
    Evaluator eval(params);

    bool success = true;
    const int NumTests = 1000;
    Move move = NoMove;
    int j, playoutLengthTotal = 0, numDraws = 0;
    for (int i = 0; success && i < NumTests; i++)
    {
        Board board(khetPosFull);
        for (j = 0; j < MaxGameLength; j++)
        {
            if ((move = GetRandomMove(board)) != NoMove)
            {
                // Make the move.
                std::cout << "Move: " << ToString(move) << std::endl;
                board.MakeMove(move);
            }
            else
            {
                break;
            }
        }

        if (j < MaxGameLength)
        {
            // Playout terminates successfully.
            playoutLengthTotal += j;

            // Check that the correct terminal evaluation is given.
            int e = eval(board);
            bool isDraw = board.IsDraw();
            if (isDraw)
            {
                ++numDraws;
                success = e == 0;
            }
            else
            {
                success = std::abs(e) == params.CheckmateVal();
            }

            if (!success)
            {
                std::cout << "The wrong evaluation was given!" << std::endl;
                std::cout << "Game was drawn: " << isDraw << std::endl;
                std::cout << "Evaluation: " << e << std::endl;
            }
        }
        else
        {
            std::cout << "Playout failed to terminate!" << std::endl;
            std::cout << board.ToString() << std::endl;
            success = false;
        }
    }

    if (success)
    {
        // Print some statistics.
        std::cout << "Average playout length: "
                  << (double)playoutLengthTotal / NumTests << std::endl;
        std::cout << "Draw ratio: "
                  << (double)numDraws / NumTests << std::endl;
    }

    REQUIRE(success);
}

TEST_CASE("Termination from standard starting position")
{
    const std::string& standard = "x33a3ka3p22/2p37/3P46/p11P31s1s21p21P4/p21P41S2S11p11P3/6p23/7P12/2P4A1KA13X1";
    RunTerminationTest(standard, Player::Silver);
}

TEST_CASE("Termination from dynasty starting position")
{
    const std::string& dynasty = "x33p3a3p23/5k4/p13p3a3s23/p21s11P41P23/3p41p21S11P4/3S2A1P13P3/4K5/3P4A1P13X1";
    RunTerminationTest(dynasty, Player::Silver);
}

TEST_CASE("Termination from imhotep starting position")
{
    const std::string& imhotep = "x33a3ka3s22//3P42p13/p1P32P2s22p2P4/p2P42S2p42p1P3/3P32p23//2S2A1KA13X1";
    RunTerminationTest(imhotep, Player::Silver);
}
