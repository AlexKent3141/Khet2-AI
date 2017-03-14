#ifndef __MOVE_GENERATOR_H__
#define __MOVE_GENERATOR_H__

#include "Globals.h"
#include "Board.h"
#include "Move.h"

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
    Player _player;
    bool _hasPieces;
    int _pieceIndex = -1;
    size_t _directionIndex;
    size_t _rotationIndex;
    Piece _pieceType;

    bool NextPiece();
    Move* NextMoveForPiece();
    Move* NextMoveForAnubis();
    Move* NextMoveForPyramid();
    Move* NextMoveForScarab();
    Move* NextMoveForPharaoh();
    Move* NextMoveForSphinx();
};

#endif // __MOVE_GENERATOR_H__
