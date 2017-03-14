#include "MoveGenerator.h"
#include "BoardHelpers.h"

MoveGenerator::MoveGenerator(const Board& board)
{
    _board = new Board(board);
    _hasPieces = NextPiece();
}

MoveGenerator::~MoveGenerator()
{
    if (_board != nullptr)
    {
        delete _board;
        _board = nullptr;
    }
}

// Get the next move.
Move* MoveGenerator::Next()
{
    Move* move = nullptr;

    if (_hasPieces && (move = NextMoveForPiece()) == nullptr)
    {
        _hasPieces = NextPiece();
        move = Next();
    }

    return move;
}

// Try to find the next move for the current piece.
Move* MoveGenerator::NextMoveForPiece()
{
    Move* move = nullptr;

    // This is piece specific.
    switch (_pieceType)
    {
    case Piece::Anubis:
        move = NextMoveForAnubis();
        break;
    case Piece::Pyramid:
        move = NextMoveForPyramid();
        break;
    case Piece::Scarab:
        move = NextMoveForScarab();
        break;
    case Piece::Pharaoh:
        move = NextMoveForPharaoh();
        break;
    case Piece::Sphinx:
        move = NextMoveForSphinx();
        break;
    default:
        break;
    }

    return move;
}

// Find the next piece.
bool MoveGenerator::NextPiece()
{
    bool pieceFound = false;
    while (!pieceFound && ++_pieceIndex < BoardArea)
    {
        Square s = _board->Get(_pieceIndex);
        if (IsPiece(s) && GetOwner(s) == _board->GetPlayerToMove())
        {
            _directionIndex = 0;
            _rotationIndex = 0;
            _pieceType = GetPiece(s);
            pieceFound = true;
        }
    }

    return pieceFound;
}

Move* MoveGenerator::NextMoveForAnubis()
{
    Move* move = nullptr;
    int destIndex;

    // Find the non-rotation moves.
    // These moves can be blocked.
    while (move == nullptr && _directionIndex < Directions.size())
    {
        // Is there a space in this direction?
        destIndex = _pieceIndex + Directions[_directionIndex++];
        if (_board->Get(destIndex) == Empty)
        {
            move = new Move(_pieceIndex, destIndex, 0);
        }
    }

    // Find the rotation moves.
    if (move == nullptr && _rotationIndex < Rotations.size())
    {
        move = new Move(_pieceIndex, _pieceIndex, Rotations[_rotationIndex++]);
    }

    return move;
}

// Pyramids have the same movement options as Anubis'.
Move* MoveGenerator::NextMoveForPyramid()
{
    return NextMoveForAnubis();
}

// Scarabs have the additional ability that they can swap with pieces.
// Only need to consider one rotation direction.
Move* MoveGenerator::NextMoveForScarab()
{
    Move* move = nullptr;
    int destIndex;
    Square sq;

    // Find the non-rotation moves.
    // These moves can be blocked.
    while (move == nullptr && _directionIndex < Directions.size())
    {
        // Is there a space or swappable piece in this direction?
        destIndex = _pieceIndex + Directions[_directionIndex++];
        sq = _board->Get(destIndex);
        if (sq == Empty || (sq != OffBoard && (int)GetPiece(sq) < 4))
        {
            move = new Move(_pieceIndex, destIndex, 0);
        }
    }

    // Consider the rotation move.
    if (move == nullptr && _rotationIndex == 0)
    {
        move = new Move(_pieceIndex, _pieceIndex, Rotations[_rotationIndex++]);
    }
    return move;
}

// Pharaohs aren't allowed to rotate - it would be equivalent to passing.
Move* MoveGenerator::NextMoveForPharaoh()
{
    Move* move = nullptr;
    int destIndex;

    // Find the non-rotation moves.
    // These moves can be blocked.
    while (move == nullptr && _directionIndex < Directions.size())
    {
        // Is there a space in this direction?
        destIndex = _pieceIndex + Directions[_directionIndex++];
        if (_board->Get(destIndex) == Empty)
        {
            move = new Move(_pieceIndex, destIndex, 0);
        }
    }

    return move;
}

Move* MoveGenerator::NextMoveForSphinx()
{
    return nullptr;
}







