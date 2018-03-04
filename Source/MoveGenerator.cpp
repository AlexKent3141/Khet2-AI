#include "MoveGenerator.h"
#include "MoveHelpers.h"
#include "SquareHelpers.h"
#include <algorithm>
#include <cstring>

MoveGenerator::MoveGenerator(const Board& board, int finalStage)
{
    _stoppedStage = finalStage + 1;
    if (!board.IsCheckmate() && !board.IsDraw())
    {
        Generate(board);
    }
}

MoveGenerator::MoveGenerator(const Board& board, Move priorityMove, int finalStage)
{
    if (priorityMove != NoMove)
        _moveBuffers[Priority].push_back(priorityMove);

    _stoppedStage = finalStage + 1;
    if (!board.IsCheckmate() && !board.IsDraw())
    {
        Generate(board);
    }
}

// Get the next move.
Move MoveGenerator::Next()
{
    // Ensure that the current stage has moves.
    while (_stage != _stoppedStage && ++_moveIndex >= (int)_currentMoves->size())
    {
        _moveIndex = -1;
        NextStage();
    }

    return _stage != _stoppedStage ? (*_currentMoves)[_moveIndex] : NoMove;
}

void MoveGenerator::NextStage()
{
    ++_stage;
    _currentMoves = _stage == Done ? nullptr : &_moveBuffers[_stage];
}

// Generate all of the moves and cache them as either captures or quiet.
void MoveGenerator::Generate(const Board& board)
{
    _playerToMove = board.PlayerToMove();
    _passiveCapture = _laser.Fire(_playerToMove, board);

    // Iterate over the pieces.
    Piece piece;
    for (int i = 0; i < BoardArea; i++)
    {
        Square s = board.Get(i);
        if (IsPiece(s) && GetOwner(s) == _playerToMove)
        {
            piece = GetPiece(s);

            switch (piece)
            {
            case Piece::Anubis:
                GenerateAnubisMoves(board, i);
                break;
            case Piece::Pyramid:
                GeneratePyramidMoves(board, i);
                break;
            case Piece::Scarab:
                GenerateScarabMoves(board, i);
                break;
            case Piece::Pharaoh:
                GeneratePharaohMoves(board, i);
                break;
            case Piece::Sphinx:
                GenerateSphinxMoves(board, i);
                break;
            default:
                break;
            }
        }
    }
}

void MoveGenerator::Sort(Stage stage, const History& history)
{
    if (_moveBuffers[stage].size() > 0)
    {
        std::sort(_moveBuffers[stage].begin(), _moveBuffers[stage].end(),
            [&] (const Move& m1, const Move& m2)
        {
            return history.Score(_playerToMove, m1) > history.Score(_playerToMove, m2);
        });
    }
}

// Add the specified move to one of the caches.
void MoveGenerator::AddMove(const Board& board, int start, int end, int rotation)
{
    // Is this move dynamic and not obviously losing?
    bool capturesOnly = _stoppedStage == Quiet;
    int dirEnd = _laser.PathAt(end);
    if (dirEnd >= 0)
    {
        // Does this piece die?
        Square sq = board.Get(start);
        int p = (int)GetPiece(sq);
        if (rotation != 0)
            sq = Rotate(sq, rotation);
        int o = (int)GetOrientation(sq);

        if (Reflections[dirEnd][p - 2][o] == Dead)
        {
            if (!capturesOnly)
                _moveBuffers[Suicide].push_back(MakeMove(start, end, rotation));
        }
        else
            _moveBuffers[Dynamic].push_back(MakeMove(start, end, rotation));
    }
    else
    {
        if (_passiveCapture)
            _moveBuffers[Dynamic].push_back(MakeMove(start, end, rotation));
        else
        {
            if (!capturesOnly)
                _moveBuffers[Quiet].push_back(MakeMove(start, end, rotation));
        }
    }
}

void MoveGenerator::GenerateAnubisMoves(const Board& board, int loc)
{
    int destIndex;
    int player = (int)board.PlayerToMove();

    // Find the non-rotation moves.
    // These moves can be blocked.
    for (size_t d = 0; d < Directions.size(); d++)
    {
        // Is there a space in this direction?
        destIndex = loc + Directions[d];
        if (board.Get(destIndex) == Empty && CanMove[player][destIndex])
        {
            AddMove(board, loc, destIndex, 0);
        }
    }

    // Find the rotation moves.
    for (size_t r = 0; r < Rotations.size(); r++)
    {
        AddMove(board, loc, loc, Rotations[r]);
    }
}

// Pyramids have the same movement options as Anubis'.
void MoveGenerator::GeneratePyramidMoves(const Board& board, int loc)
{
    GenerateAnubisMoves(board, loc);
}

// Scarabs have the additional ability that they can swap with pieces.
// Only need to consider one rotation direction.
void MoveGenerator::GenerateScarabMoves(const Board& board, int loc)
{
    int destIndex;
    int player = (int)board.PlayerToMove();
    Square sq;

    // Find the non-rotation moves.
    // These moves can be blocked.
    for (size_t d = 0; d < Directions.size(); d++)
    {
        // Is there a space or swappable piece in this direction?
        destIndex = loc + Directions[d];
        sq = board.Get(destIndex);
        if ((sq == Empty || (sq != OffBoard && (int)GetPiece(sq) < 4)) && CanMove[player][destIndex])
        {
            AddMove(board, loc, destIndex, 0);
        }
    }

    // Consider the rotation move.
    AddMove(board, loc, loc, Rotations[0]);
}

// Pharaohs aren't allowed to rotate - it would be equivalent to passing.
void MoveGenerator::GeneratePharaohMoves(const Board& board, int loc)
{
    int destIndex;
    int player = (int)board.PlayerToMove();

    // Find the non-rotation moves.
    // These moves can be blocked.
    for (size_t d = 0; d < Directions.size(); d++)
    {
        // Is there a space in this direction?
        destIndex = loc + Directions[d];
        if (board.Get(destIndex) == Empty && CanMove[player][destIndex])
        {
            AddMove(board, loc, destIndex, 0);
        }
    }
}

// Sphinxes cannot move but do have one rotation available to them on each turn.
void MoveGenerator::GenerateSphinxMoves(const Board& board, int loc)
{
    // There is exactly one rotation move available.
    Player player = board.PlayerToMove();
    Square sq = board.Get(Sphinx[(int)player]);
    int o = GetOrientation(sq);
    int rotation = player == Player::Silver
                   ? (o == Up ? -1 : 1)
                   : (o == Down ? -1 : 1);

    AddMove(board, loc, loc, rotation);
}

