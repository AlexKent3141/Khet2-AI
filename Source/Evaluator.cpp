#include "Evaluator.h"
#include "Globals.h"
#include "SquareHelpers.h"

Evaluator::Evaluator()
{
    _params = EvalParams();
}

Evaluator::Evaluator(const EvalParams& params)
{
    _params = params;
}

int Evaluator::operator()(const Board& board) const
{
    int terminalScore = 0;
    return TerminalScore(board, &terminalScore) 
           ? terminalScore
           : MaterialScore(board) + LaserableScore(board);
}

bool Evaluator::TerminalScore(const Board& board, int* score) const
{
    bool terminal = board.IsCheckmate() || board.IsDraw();
    if (terminal)
    {
        *score = board.IsDraw() 
            ? 0 
            : _params.CheckmateVal() * (board.PlayerToMove() == Player::Silver ? -1 : 1);
    }

    return terminal;
}

int Evaluator::MaterialScore(const Board& board) const
{
    int eval = 0, pieceVal;
    Piece piece;
    Player player;
    Square sq;
    for (size_t i = 0; i < BoardArea; i++)
    {
        sq = board.Get(i);
        if (sq != OffBoard && sq != Empty)
        {
            player = GetOwner(sq);
            piece = GetPiece(sq);
            pieceVal = _params.PieceVal(piece);
            eval += pieceVal * (player == Player::Silver ? 1 : -1);
        }
    }

    return eval;
}

int Evaluator::LaserableScore(const Board& board) const
{
    return _params.LaserVal() * 
        (LaserableSquares(Player::Silver, board) - LaserableSquares(Player::Red, board));
}

int Evaluator::LaserableSquares(Player player, const Board& board) const
{
    int squares = 0;

    // Find the starting location and direction for the laser beam.
    int loc = Sphinx[(int)player];
    int dirIndex = GetOrientation(board.Get(loc));
    int dir, p;
    Square dest = Empty;
    while (dest != OffBoard && dirIndex >= 0)
    {
        dir = Directions[dirIndex];

        // Take a step with the laser beam.
        loc += dir;

        // Is this location occupied?
        dest = board.Get(loc);
        if (IsPiece(dest))
        {
            p = (int)GetPiece(dest);
            dirIndex = Reflections[dirIndex][p - 2][GetOrientation(dest)];
        }
        else
        {
            ++squares;
        }
    }

    return squares;
}
