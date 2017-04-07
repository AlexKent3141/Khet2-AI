#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include "Board.h"
#include "Globals.h"

// This class contains methods that are used to evaluate a board position.
class Evaluator
{
public:
    // Evaluate the specified position.
    int operator()(const Board&) const;

private:
    // The value of each piece type.
    // Only necessary to given the Anubis and Pyramid pieces values - the other piece types cannot
    // be captured.
    const int AnubisVal = 400;
    const int PyramidVal = 1000;
    const int PieceVals[7] = { 0, 0, AnubisVal, PyramidVal, 0, 0, 0 };

    // The value of a single laserable square.
    const int LaserVal = 40;

    // This value is above any achievable in the game normally.
    const int CheckmateVal = 3*AnubisVal + 10*PyramidVal + BoardArea*LaserVal;

    // Check if the board is in a terminal state and score appropriately if so.
    bool TerminalScore(const Board&, int*) const;

    // Compute the material part of the evaluation.
    int MaterialScore(const Board&) const;

    // Compute the "laserability" part of the evaluation.
    int LaserableScore(const Board&) const;

    // Compute the number of laserable squares for the specified player.
    int LaserableSquares(Player, const Board&) const;
};

#endif // __EVALUATOR_H__
