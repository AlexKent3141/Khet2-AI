#include "MoveGenerator.h"
#include "BoardHelpers.h"

MoveGenerator::MoveGenerator(const Board& board)
{
    _board = new Board(board);
    _hasPieces = NextPiece();
}

MoveGenerator::~MoveGenerator()
{
    if (_board == nullptr)
    {
        delete _board;
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

    // TODO: There will need to be some piece specific functionality here.

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
            _directionIndex = -1;
            _rotationIndex = -1;
            _pieceType = GetPiece(s);
            pieceFound = true;
        }
    }

    return pieceFound;
}
