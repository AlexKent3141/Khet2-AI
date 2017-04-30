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
    Board() = delete;
    Board(const Board&);
    Board(const std::string&);

    int Compare(const Board&);

    // Accessors.
    inline Player PlayerToMove() const { return _playerToMove; }
    inline Square Get(int i) const { return _board[i]; }
    inline bool IsCheckmate() const { return _checkmate; }
    inline bool IsDraw() const { return _drawn; }
    inline Square LastCapture() const { return _captureSquare[_moveNumber]; }
    inline uint64_t HashKey() const { return _hashes[_moveNumber]; }

    // Making/unmaking moves.
    void MakeMove(Move const* const);
    void UndoMove();

    std::string ToString() const;
    
private:
    Player _playerToMove;
    bool _checkmate = false, _drawn = false;
    int _moveNumber = 0;
    uint64_t _hashes[MaxGameLength];

    // Mailbox style storage is used with one layer of padding.
    Square _board[BoardArea];

    // Move list.
    const Move* _moves[MaxGameLength];

    // Cache the capture square and location so that it can be restored.
    Square _captureSquare[MaxGameLength];
    int _captureLocation[MaxGameLength];

    // Cache the number of moves since the last capture.
    int _movesWithoutCapture[MaxGameLength];

    void Init();

    bool FireLaser(uint64_t&);

    void CheckForDraw();

    void FromString(const std::string&);

    // Serialisation.
    void ParseLine(int, const std::string&);
    Piece PieceFromChar(char) const;
    char CharFromPiece(Player, Piece) const;
};

#endif // __BOARD_H__
