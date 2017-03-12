#ifndef __MOVE_GENERATOR_H__
#define __MOVE_GENERATOR_H__

#include "Types.h"
#include "Globals.h"
#include "Board.h"

// The generator class generates moves incrementally.
// It enumerates piece-wise.
class MoveGenerator
{
public:
    MoveGenerator(const Board&);
    ~MoveGenerator();
    Move* Next();

private:
    Board* _board = nullptr;
    bool _hasPieces;
    int _pieceIndex = -1;
    int _directionIndex;
    int _rotationIndex;
    Piece _pieceType;

    Move* NextMoveForPiece();
    bool NextPiece();
};

#endif // __MOVE_GENERATOR_H__
