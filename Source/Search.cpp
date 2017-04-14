#include "Search.h"
#include "Evaluator.h"
#include "MoveGenerator.h"
#include <cassert>
#include <climits>

// Iterative deepening framework.
Move* Search::Start(const SearchParams& params, Board& board, int& score)
{
    _params = params;
    _startTime = clock();

    Move* bestMove = nullptr, *tempMove = nullptr;
    int tempScore;
    bool keepSearching = true;
    Evaluator eval;
    for (int d = 1; keepSearching; d++)
    {
        tempMove = AlphaBetaRoot(eval, board, d, tempScore);
        if (CheckTime())
        {
            if (bestMove != nullptr)
                delete bestMove;

            bestMove = tempMove;
            score = tempScore;
        }
        else
        {
            keepSearching = false;
        }

        if (_params.Depth && d >= _params.Depth)
        {
            keepSearching = false;
        }
    }

    return bestMove;
}

// Check whether there is still time remaining for this search.
bool Search::CheckTime() const
{
    bool hasTime = true;
    if (!_params.Infinite && !_params.Depth)
    {
        clock_t elapsed = 1000 * (double)(clock() - _startTime) / CLOCKS_PER_SEC;
        hasTime = elapsed < _params.MoveTime;
    }
    
    return hasTime;
}

// Initial alpha-beta call which assesses the root nodes and returns the best move.
Move* Search::AlphaBetaRoot(const Evaluator& eval, Board& board, int depth, int& score)
{
    score = INT_MIN;
    Move* bestMove = nullptr;
    int sign = board.PlayerToMove() == Player::Silver ? 1 : -1;
    if (depth == 0 || board.IsCheckmate() || board.IsDraw())
    {
        score = sign * eval(board);
    }
    else
    {
        MoveGenerator gen(board);
        Move* move;
        int val;
        while ((move = gen.Next()) != nullptr)
        {
            board.MakeMove(move);
            val = -AlphaBeta(eval, board, depth-1, INT_MIN, INT_MAX, -sign);
            board.UndoMove();

            // If this move is better then store the move and score.
            if (val > score)
            {
                score = std::max(score, val);
                if (bestMove != nullptr)
                    delete bestMove;

                bestMove = new Move(*move);
            }

            delete move;
        }
    }

    return bestMove;
}

// Alpha-beta call which returns the value of the specified board state.
int Search::AlphaBeta(const Evaluator& eval, Board& board, int depth, int alpha, int beta, int sign)
{
    int score = INT_MIN;
    if (depth == 0 || board.IsCheckmate() || board.IsDraw())
    {
        score = sign * eval(board);
    }
    else
    {
        MoveGenerator gen(board);
        Move* move;
        int val;
        while ((move = gen.Next()) != nullptr && CheckTime())
        {
            board.MakeMove(move);
            val = -AlphaBeta(eval, board, depth-1, -beta, -alpha, -sign);
            board.UndoMove();
            delete move;

            // Update the bounds.
            score = std::max(score, val);
            alpha = std::max(alpha, val);
            if (alpha >= beta)
            {
                break;
            }
        }
    }

    return score;
}
