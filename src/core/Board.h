#ifndef __BOARD_H__
#define __BOARD_H__

#include "Bitboard.h"
#include "Globals.h"
#include "ILaserable.h"
#include "Laser.h"
#include "Types.h"
#include <array>
#include <string>

// Represents the Khet board state.
// Exposes methods for updating the state.
class Board : public ILaserable
{
public:
    Board() = delete;
    Board(const Board&);
    Board(const std::string&);

    int Compare(const Board&);
    bool IsLegal(Move) const;

    // Accessors.
    inline Square Get(int i) const override { return _board[i]; }

    inline BB GetPieces(Player p, Piece pt) const
    {
        return _pieces[int(p)][int(pt)];
    }

    inline BB GetPieces() const override
    {
        return _playerPieces[0] | _playerPieces[1];
    }

    inline Player PlayerToMove() const { return _playerToMove; }
    inline bool IsCheckmate() const { return _checkmate; }
    inline bool IsDraw() const { return _drawn; }
    inline Square LastCapture() const { return _captureSquare[_moveNumber]; }
    inline uint64_t HashKey() const { return _hashes[_moveNumber]; }
    inline int PharaohPosition(Player player) const { return _pharaohPositions[(int)player]; }

    // Making/unmaking moves.
    void MakeMove(Move);
    void UndoMove();

    std::string ToString() const;
    
private:
    Player _playerToMove;
    bool _checkmate = false, _drawn = false;
    int _moveNumber = 0;
    uint64_t _hashes[MaxGameLength];
    Laser _laser;

    // Mailbox style storage.
    Square _board[BoardArea];

    // All pieces for each player.
    std::array<BB, 2> _playerPieces;

    // Bitboards for each piece type for each player.
    std::array<std::array<BB, 7>, 2> _pieces;

    // Move list.
    Move _moves[MaxGameLength] = { NoMove };

    // The current pharaoh positions.
    int _pharaohPositions[2];

    // Cache the capture square and location so that it can be restored.
    Square _captureSquare[MaxGameLength];
    int _captureLocation[MaxGameLength];

    // Cache the number of moves since the last capture.
    int _movesWithoutCapture[MaxGameLength];

    void Init();

    void CheckForDraw();

    void FromString(const std::string&);

    void RemovePieceFromBB(Player, Piece, int);
    void AddPieceToBB(Player, Piece, int);

    // Serialisation.
    void ParseLine(int, const std::string&);
    Piece PieceFromChar(char) const;
    char CharFromPiece(Player, Piece) const;
};

#endif // __BOARD_H__
