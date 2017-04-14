#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "Board.h"
#include "Evaluator.h"
#include "Move.h"
#include "SearchParams.h"
#include <ctime>

class Search
{
public:
    // Iterative deepening Minimax search.
    Move* Start(const SearchParams&, Board&, int&);

private:
    SearchParams _params;
    clock_t _startTime;

    // Check whether there is still time remaining for the search.
    bool CheckTime() const;

    // This root call to the Alpha-Beta search returns the best root move.
    Move* AlphaBetaRoot(const Evaluator&, Board&, int, int&);

    // Score the given position using NegaMax with Alpha-Beta pruning.
    int AlphaBeta(const Evaluator&, Board&, int depth, int alpha, int beta, int sign);
};

#endif // __SEARCH_H__
