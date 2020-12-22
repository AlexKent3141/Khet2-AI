#include "MoveGenerator.h"
#include "Bitboard.h"
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

MoveGenerator::MoveGenerator(const Board& board, Move hashMove, Move killerMove, int finalStage)
{
    if (hashMove != NoMove) _moveBuffers[Priority].push_back(hashMove);
    if (killerMove != NoMove) _moveBuffers[Priority].push_back(killerMove);

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

    int loc;
    BB pieces = board.GetPieces(_playerToMove, Piece::Anubis);
    while (pieces)
    {
        loc = pieces.PopLSB();
        GenerateAnubisMoves(board, loc);
    }

    pieces = board.GetPieces(_playerToMove, Piece::Pyramid);
    while (pieces)
    {
        loc = pieces.PopLSB();
        GeneratePyramidMoves(board, loc);
    }

    pieces = board.GetPieces(_playerToMove, Piece::Scarab);
    while (pieces)
    {
        loc = pieces.PopLSB();
        GenerateScarabMoves(board, loc);
    }

    pieces = board.GetPieces(_playerToMove, Piece::Pharaoh);
    while (pieces)
    {
        loc = pieces.PopLSB();
        GeneratePharaohMoves(board, loc);
    }

    pieces = board.GetPieces(_playerToMove, Piece::Sphinx);
    while (pieces)
    {
        loc = pieces.PopLSB();
        GenerateSphinxMoves(board, loc);
    }
}

void MoveGenerator::Sort(Stage stage, const History& history)
{
    std::sort(_moveBuffers[stage].begin(), _moveBuffers[stage].end(),
        [&] (const Move& m1, const Move& m2)
    {
        return history.Score(_playerToMove, m1) > history.Score(_playerToMove, m2);
    });
}

// Add the specified move to one of the caches.
void MoveGenerator::AddMove(const Board& board, int start, int end, int rotation)
{
    // Is this move dynamic and not obviously losing?
    bool capturesOnly = _stoppedStage == Quiet;

    if (_laser.PathAt(start) || _laser.PathAt(end))
    {
        // Fire the laser and check whether anything would die.
        Square sq = board.Get(start);
        if (rotation != 0)
            sq = Rotate(sq, rotation);

        int killLoc;
        if (_laser.FireWillKill(
            _playerToMove,
            board,
            start,
            end,
            sq,
            board.Get(end),
            &killLoc))
        {
            // Either capture or suicide.
            if (killLoc == end || GetOwner(board.Get(killLoc)) == _playerToMove)
            {
                if (!capturesOnly)
                    _moveBuffers[Suicide].push_back(MakeMove(start, end, rotation));
            }
            else
            {
                _moveBuffers[Dynamic].push_back(MakeMove(start, end, rotation));
            }
        }
        else
        {
            if (!capturesOnly)
                _moveBuffers[Quiet].push_back(MakeMove(start, end, rotation));
        }
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
    BB dest = Bitboards::neighbours[loc]
            & Bitboards::canMove[player]
            & ~board.GetPieces();

    while (dest)
    {
        // Is there a space in this direction?
        destIndex = dest.PopLSB();
        AddMove(board, loc, destIndex, 0);
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
    BB dest = Bitboards::neighbours[loc]
            & Bitboards::canMove[player];

    while (dest)
    {
        destIndex = dest.PopLSB();

        // If the destination is occupied then ensure it contains a type of
        // piece that can be swapped with.
        sq = board.Get(destIndex);
        if (sq == Empty || int(GetPiece(sq)) < 4)
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
    BB dest = Bitboards::neighbours[loc]
            & Bitboards::canMove[player]
            & ~board.GetPieces();

    while (dest)
    {
        // Is there a space in this direction?
        destIndex = dest.PopLSB();
        AddMove(board, loc, destIndex, 0);
    }
}

// Sphinxes cannot move but do have one rotation available to them on each turn.
void MoveGenerator::GenerateSphinxMoves(const Board& board, int loc)
{
    // There is exactly one rotation move available.
    Player player = board.PlayerToMove();
    Square sq = board.Get(loc);
    int o = GetOrientation(sq);
    int rotation = player == Player::Silver
                   ? (o == Up ? -1 : 1)
                   : (o == Down ? -1 : 1);

    AddMove(board, loc, loc, rotation);
}

