#ifndef __BOARD_H__
#define __BOARD_H__

#include "Globals.h"
#include "Types.h"
#include "Move.h"
#include <string>

// Represents the Khet board state.
// Exposes methods for updating the state.
class Board
{
public:
    Board();
    Board(const Board&);
    Board(const std::string&);

    // Accessors.
    inline Player PlayerToMove() const { return _playerToMove; }
    inline Square Get(int i) const { return _board[i]; }

    // Making/unmaking moves.
    void MakeMove(const Move&);
    void UndoMove();

    std::string ToString() const;
    
private:
    Player _playerToMove;
    int _moveNumber = 0;

    // Mailbox style storage is used with one layer of padding.
    Square _board[BoardArea];

    // Cache capture piece types, orientations and locations.
    int _captureLoc[MaxGameLength];
    int _captureOrientation[MaxGameLength];
    int _captureType[MaxGameLength];

    void Init();

    void FireLaser();

    void FromString(const std::string&);

    // Serialisation.
    void ParseLine(int, const std::string&);
    Piece PieceFromChar(char) const;
    char CharFromPiece(Player, Piece) const;
};

#endif // __BOARD_H__
