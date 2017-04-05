#include "Board.h"
#include "SquareHelpers.h"
#include "Zobrist.h"
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
    memcpy(_hashes, other._hashes, MaxGameLength*sizeof(uint64_t));
    memcpy(_board, other._board, BoardArea*sizeof(Square));
}

Board::Board(const std::string& ks)
{
    Init();
    FromString(ks);
}

// If there is no difference then return zero. 
// The comparison takes into account the squares, the player to move and the hash.
int Board::Compare(const Board& other)
{
    int ret = 0;

    // Compare the board squares.
    while (ret++ < BoardArea + 1 && _board[ret - 1] == other._board[ret - 1]);

    if (ret > BoardArea)
    {
        ret = _playerToMove == other._playerToMove && 
            _hashes[_moveNumber] != other._hashes[_moveNumber] ?  -1 : 0;
    }

    return ret;
}

void Board::Init()
{
    memset(_hashes, 0, MaxGameLength*sizeof(uint64_t));
    memset(_captureSquare, Empty, MaxGameLength*sizeof(Square));
    memset(_captureLocation, -1, MaxGameLength*sizeof(int));
}

void Board::MakeMove(Move const* const move)
{
    auto z = Zobrist::Instance();
    uint64_t hash = _hashes[_moveNumber];

    int start = move->Start();
    int end = move->End();

    Square movingPiece = _board[start];
    hash ^= z->Key(movingPiece, start);

    if (move->Rotation() != 0)
    {
        movingPiece = Rotate(movingPiece, move->Rotation());
    }

    _board[start] = _board[end];
    _board[end] = movingPiece;
    hash ^= z->Key(movingPiece, end);

    // Check whether pieces are captured.
    ++_moveNumber;
    FireLaser(hash);

    _playerToMove = _playerToMove == Player::Silver ? Player::Red : Player::Silver;

    _moves[_moveNumber] = move;
    _hashes[_moveNumber] = hash;
}

// Note: The move that needs to be undone should already be cached.
void Board::UndoMove()
{
    Move const* const move = _moves[_moveNumber];

    // Restore any captured pieces.
    Square cap = _captureSquare[_moveNumber];
    int capLoc = _captureLocation[_moveNumber];

    --_moveNumber;
    if (cap != Empty)
    {
        _board[capLoc] = cap;
    }

    int start = move->Start();
    int end = move->End();
    Square movedPiece = _board[end];

    if (move->Rotation() != 0)
    {
        // Reverse the rotation.
        movedPiece = Rotate(movedPiece, -1*move->Rotation());
    }

    _board[end] = _board[start];
    _board[start] = movedPiece;

    _checkmate = false;
    _playerToMove = _playerToMove == Player::Silver ? Player::Red : Player::Silver;
}

// Check whether the position is drawn by repetition or inactivity.
// Need to loop backwards through the moves until the last capture/inactivity limit is reached.
bool Board::IsDraw() const
{
    bool isDraw = false;
    int numRepeats = 1;
    uint64_t repeatHash = _hashes[_moveNumber];
    int captureMoveLimit = std::max(0, _moveNumber - TimeSinceCaptureLimit - 1);
    int i;
    for (i = _moveNumber - 1; i >= captureMoveLimit && !isDraw && _captureSquare[i] == Empty; i--)
    {
        if (_hashes[i] == repeatHash)
        {
            ++numRepeats;
            isDraw = numRepeats == RepetitionLimit;
        }
    }
    
    if (i < captureMoveLimit)
    {
        isDraw = true;
    }

    return isDraw;
}

void Board::FireLaser(uint64_t& hash)
{
    // Find the starting location and direction for the laser beam.
    int loc = Sphinx[(int)_playerToMove];
    int dirIndex = GetOrientation(_board[loc]);
    int dir, p;
    Square dest = Empty;
    while (dest != OffBoard && dirIndex >= 0)
    {
        dir = Directions[dirIndex];

        // Take a step with the laser beam.
        loc += dir;

        // Is this location occupied?
        dest = _board[loc];
        if (IsPiece(dest))
        {
            p = (int)GetPiece(dest);
            dirIndex = Reflections[dirIndex][p - 2][GetOrientation(dest)];
        }
    }

    // Was there a capture?
    _captureSquare[_moveNumber] = Empty;
    if (dirIndex == Dead)
    {
        _captureSquare[_moveNumber] = dest;
        _captureLocation[_moveNumber] = loc;
        _board[loc] = Empty;
        _checkmate = p == (int)Piece::Pharaoh;
        hash ^= Zobrist::Instance()->Key(dest, loc);
    }
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
    int boardIndex = BoardWidth * (BoardHeight - index - 2) + 1;

    // Fill the line with empty initially.
    memset(&_board[boardIndex], Empty, (BoardWidth - 2)*sizeof(Square));

    auto z = Zobrist::Instance();
    uint64_t hash = 0;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (isalpha(line[i]))
        {
            // Specifying a piece.
            Player player = isupper(line[i]) ? Player::Silver : Player::Red;
            Piece piece = PieceFromChar(line[i]);

            Orientation orientation = piece == Piece::Pharaoh ? 
                Orientation::Up : (Orientation)(line[++i] - 1 - '0');

            Square sq = MakeSquare(player, piece, orientation);
            hash ^= z->Key(sq, boardIndex);
            _board[boardIndex++] = sq;
        }
        else
        {
            // A gap between pieces.
            boardIndex += (line[i] - '0');
        }
    }

    _hashes[0] = hash;
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

    return player == Player::Silver ? toupper(c) : c;
}

