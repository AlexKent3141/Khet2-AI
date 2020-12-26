#include "Evaluator.h"
#include "Globals.h"
#include "Laser.h"
#include "SquareHelpers.h"
#include <algorithm>

int Distance(int loc1, int loc2)
{
    int xDiff = std::abs((loc1 - loc2) % BoardWidth);
    int yDiff = std::abs((loc1 - loc2) / BoardWidth);
    return std::max(xDiff, yDiff);
}

Laser Evaluator::_laser = Laser();

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
            : _params.CheckmateVal() * (GetOwner(board.LastCapture()) == Player::Silver ? -1 : 1);
    }

    return terminal;
}

int Evaluator::MaterialScore(const Board& board) const
{
    int eval = 0, pieceVal, pharaohVal, pharaohLoc, loc, sign;
    BB pieces;

    for (Player player : { Player::Silver, Player::Red })
    {
        sign = player == Player::Silver ? 1 : -1;
        pharaohLoc = board.PharaohPosition(player);
        for (Piece piece : { Piece::Anubis, Piece::Pyramid, Piece::Scarab })
        {
            pieces = board.GetPieces(player, piece);
            while (pieces)
            {
                loc = pieces.PopLSB();
                pieceVal = _params.PieceVal(piece);
                pharaohVal = piece == Piece::Scarab
                    ? _params.PiecePharaohVal(Distance(loc, pharaohLoc)) : 0;
                eval += (pieceVal + pharaohVal) * sign;
            }
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
    int bonus = 0;
    Player other = player == Player::Silver ? Player::Red : Player::Silver;
    int enemyPharaoh = board.PharaohPosition(other);

    _laser.Fire(player, board);

    BB path = _laser.LaserPath();
    int loc, pathLength = 0;
    while (path)
    {
        loc = path.PopLSB();
        bonus += _params.LaserPharaohVal(Distance(enemyPharaoh, loc));
        ++pathLength;
    }

    return _params.LaserVal() * pathLength  + bonus;
}
