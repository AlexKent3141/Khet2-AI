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

int Evaluator::Distance(int loc1, int loc2) const
{
    int xDiff = std::abs((loc1 - loc2) % BoardWidth);
    int yDiff = std::abs((loc1 - loc2) / BoardWidth);
    return xDiff + yDiff;
}

int Evaluator::FindPharaoh(Player player, const Board& board) const
{
    int pharaohLoc = -1;
    Square sq;
    for (size_t i = 0; i < BoardArea; i++)
    {
        sq = board.Get(i);
        if (sq != OffBoard && sq != Empty)
        {
            if (GetOwner(sq) == player && GetPiece(sq) == Piece::Pharaoh)
            {
                pharaohLoc = i;
                break;
            }
        }
    }

    return pharaohLoc;
}

bool Evaluator::TerminalScore(const Board& board, int* score) const
{
    bool terminal = board.IsCheckmate() || board.IsDraw();
    if (terminal)
    {
        *score = board.IsDraw() 
            ? 0 
            : _params.CheckmateVal() * (GetOwner(board.LastCapture()) == Player::Silver ? -1 : 1);
    }

    return terminal;
}

int Evaluator::MaterialScore(const Board& board) const
{
    int pharaohLocs[2] = { FindPharaoh(Player::Silver, board),
                           FindPharaoh(Player::Red, board) };

    int eval = 0, pieceVal, pharaohVal, pharaohLoc;
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
            pharaohLoc = pharaohLocs[1 - (int)player];
            pharaohVal = _params.PiecePharaohVal(Distance(i, pharaohLoc));
            eval += (pieceVal + pharaohVal) * (player == Player::Silver ? 1 : -1);
        }
    }

    return eval;
}

int Evaluator::LaserableScore(const Board& board) const
{
    return LaserableScore(Player::Silver, board) - LaserableScore(Player::Red, board);
}

int Evaluator::LaserableScore(Player player, const Board& board) const
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

    return _params.LaserVal() * squares;
}
