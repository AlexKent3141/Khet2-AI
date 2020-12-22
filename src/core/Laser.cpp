#include "Laser.h"
#include <cstring>

bool Laser::Fire(const Player& player, const ILaserable& board)
{
    const BB allPieces = board.GetPieces();

    // Find the starting location and direction for the laserbeam.
    _targetIndex = Sphinx[(int)player];
    _targetSquare = Empty;
    int dirIndex = GetOrientation(board.Get(_targetIndex));
    while (dirIndex >= 0)
    {
        BB fullRay = Bitboards::rays[_targetIndex][dirIndex];
        BB attacked = fullRay & allPieces;

        if (!attacked)
        {
            // No pieces are killed.
            _laserPath |= fullRay;
            return false;
        }

        // Find the first piece along this ray that is attacked.
        _targetIndex = dirIndex < 2
            ? attacked.LSB()
            : attacked.MSB();

        _laserPath |= (fullRay & ~Bitboards::rays[_targetIndex][dirIndex]);

        // Check whether the piece is killed or reflects the laser.
        _targetSquare = board.Get(_targetIndex);
        _targetPiece = (int)GetPiece(_targetSquare);
        dirIndex = Reflections[dirIndex][_targetPiece - 2][GetOrientation(_targetSquare)];
    }

    return true;
}
