#include "Laser.h"
#include "SquareHelpers.h"
#include <cstring>

bool Laser::Fire(const Player& player, const ILaserable& board)
{
	memset(_laserPath, -1, BoardArea*sizeof(int));
    _pathLength = 0;

    // Find the starting location and direction for the laserbeam.
    _targetIndex = Sphinx[(int)player];
    _targetSquare = Empty;
    int dirIndex = GetOrientation(board.Get(_targetIndex));
    int dir;
    while (_targetSquare != OffBoard && dirIndex >= 0)
    {
        ++_pathLength;
        _laserPath[_targetIndex] = dirIndex;

        dir = Directions[dirIndex];

        // Take a step with the laserbeam.
        _targetIndex += dir;

        // Is this location occupied?
        _targetSquare = board.Get(_targetIndex);
        if (IsPiece(_targetSquare))
        {
            _targetPiece = (int)GetPiece(_targetSquare);
            dirIndex = Reflections[dirIndex][_targetPiece - 2][GetOrientation(_targetSquare)];
        }
    }

	return dirIndex == Dead;
}
