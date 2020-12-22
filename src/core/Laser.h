#ifndef __LASER_H__
#define __LASER_H__

#include "Bitboard.h"
#include "Globals.h"
#include "ILaserable.h"
#include "SquareHelpers.h"

class Laser
{
public:
    virtual ~Laser() = default;

    // Fire the laser for the current player on the specified laserable object
    // and return a boolean indicating whether a piece gets captured.
    virtual bool Fire(const Player&, const ILaserable&);

    inline int TargetIndex() const { return _targetIndex; }
    inline Square TargetSquare() const { return _targetSquare; }
    inline int TargetPiece() const { return _targetPiece; }
    inline BB LaserPath() const { return _laserPath; }
    inline bool PathAt(int loc) const { return _laserPath.Test(loc); }

private:
    int _targetIndex;
    Square _targetSquare;
    int _targetPiece;
    BB _laserPath;
};

#endif // __LASER_H__
