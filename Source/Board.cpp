#include "Board.h"
#include "SquareHelpers.h"
#include "Utils.h"
#include <cctype>
#include <cstring>

Board::Board()
{
    Init();
    FromString(Standard);
}

Board::Board(const Board& other)
{
    Init();
    _playerToMove = other._playerToMove;
    memcpy(_board, other._board, BoardArea*sizeof(Square));
}

Board::Board(const std::string& ks)
{
    Init();
    FromString(ks);
}

void Board::Init()
{
    memset(_captureSquare, Empty, MaxGameLength*sizeof(Square));
}

void Board::MakeMove(Move const* const move)
{
    int start = move->Start();
    int end = move->End();

    Square movingPiece = _board[start];
    if (move->Rotation() != 0)
    {
        movingPiece = Rotate(movingPiece, move->Rotation());
    }

    _board[start] = _board[end];
    _board[end] = movingPiece;

    FireLaser();

    _playerToMove = _playerToMove == Player::Silver ? Player::Red : Player::Silver;

    _moves[_moveNumber++] = move;
}

// Note: The move that needs to be undone should already be cached.
void Board::UndoMove()
{
    Move const* const move = _moves[--_moveNumber];

    int start = move->Start();
    int end = move->End();
    Square movedPiece = _board[end];

    // Reverse the rotation.
    if (move->Rotation() != 0)
    {
        movedPiece = Rotate(movedPiece, -1*move->Rotation());
    }

    _board[end] = _board[start];
    _board[start] = movedPiece;

    // Restore any captured pieces.
    if (_captureSquare[_moveNumber] != Empty)
    {
        _board[end] = _captureSquare[_moveNumber];
    }

    _playerToMove = _playerToMove == Player::Silver ? Player::Red : Player::Silver;
}

void Board::FireLaser()
{
    // Find the starting location and direction for the laser beam.
    int loc = Sphinx[(int)_playerToMove];
    int o = GetOrientation(_board[loc]);
    int dir = Directions[o - 1];
    Square dest = Empty;
    Piece p;
    while (dest != OffBoard && dir != 0 && dir != Absorbed)
    {
        // Take a step with the laser beam.
        loc += dir;

        // Is this location occupied?
        dest = _board[loc];
        if (IsPiece(dest))
        {
            p = GetPiece(dest);
            int po = GetOrientation(dest);
            dir = Reflections[o - 1][(int)p - 2][po - 1];
            o = po;
        }
    }

    // Was there a capture?
    _captureSquare[_moveNumber] = dir == 0 ? dest : Empty;
}

// Serialise the board to a human-readable string.
std::string Board::ToString() const
{
    std::string pieces;
    std::string orientations;

    for (int r = BoardHeight - 1; r > -1; r--)
    {
        for (int c = 0; c < BoardWidth; c++)
        {
            int i = r*BoardWidth + c;
            if (i % BoardWidth == 0)
            {
                pieces += "\n";
                orientations += "\n";
            }

            if (_board[i] == OffBoard)
            {
                pieces += "*";
                orientations += "*";
            }
            else if (_board[i] == Empty)
            {
                pieces += ".";
                orientations += "0";
            }
            else
            {
                Player player = GetOwner(_board[i]);
                Piece piece = GetPiece(_board[i]);
                Orientation orientation = GetOrientation(_board[i]);

                pieces += CharFromPiece(player, piece);
                orientations += std::to_string((int)orientation);
            }
        }
    }

    return pieces + "\n" + orientations;
}

// Initialise the board from the specified Khet string.
void Board::FromString(const std::string& ks)
{
    memset(_board, OffBoard, BoardArea*sizeof(Square));

    auto utils = Utils::GetInstance();
    auto tokens = utils->Split(ks, ' ');
    _playerToMove = tokens[1] == "0" ? Player::Silver : Player::Red;

    tokens = utils->Split(tokens[0], '/');
    for (size_t i = 0; i < tokens.size(); i++)
    {
        ParseLine(i, tokens[i]);
    }
}

void Board::ParseLine(int index, const std::string& line)
{
    // The zero-th index is the back rank.
    // Also need to take into account padding.
    int rowIndex = 1;
    int rowStart = BoardWidth * (BoardHeight - index - 2);

    // Fill the line with empty initially.
    memset(&_board[rowStart + rowIndex], Empty, (BoardWidth - 2)*sizeof(Square));

    bool havePiece = false;
    Piece piece;
    Player player;
    Orientation orientation;

    for (size_t i = 0; i < line.size(); i++)
    {
        if (isalpha(line[i]))
        {
            // Specifying a piece.
            havePiece = true;
            player = isupper(line[i]) != 0 ? Player::Silver : Player::Red;
            piece = PieceFromChar(line[i]);

            if (piece == Piece::Pharaoh)
            {
                _board[rowStart + rowIndex++] = MakeSquare(player, piece, Orientation::Up);
                havePiece = false;
            }
        }
        else if (havePiece)
        {
            // Orientation argument for the piece.
            orientation = (Orientation)(line[i] - '0');
            _board[rowStart + rowIndex++] = MakeSquare(player, piece, orientation);
            havePiece = false;
        }
        else
        {
            // A gap between pieces.
            rowIndex += (line[i] - '0');
        }
    }
}

Piece Board::PieceFromChar(char c) const
{
    Piece p;

    c = tolower(c);
    if (c == 'a')
        p = Piece::Anubis;
    else if (c == 'p')
        p = Piece::Pyramid;
    else if (c == 's')
        p = Piece::Scarab;
    else if (c == 'k')
        p = Piece::Pharaoh;
    else if (c == 'x')
        p = Piece::Sphinx;

    return p;
}

char Board::CharFromPiece(Player player, Piece piece) const
{
    char c;

    if (piece == Piece::Anubis)
        c = 'a';
    else if (piece == Piece::Pyramid)
        c = 'p';
    else if (piece == Piece::Scarab)
        c = 's';
    else if (piece == Piece::Pharaoh)
        c = 'k';
    else if (piece == Piece::Sphinx)
        c = 'x'; 

    if (player == Player::Silver)
        c = toupper(c);

    return c;
}









