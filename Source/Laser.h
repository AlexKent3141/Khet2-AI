#ifndef __LASER_H__
#define __LASER_H__

#include "Globals.h"
#include "ILaserable.h"

class Laser
{
public:
    // Fire the laser for the current player on the specified laserable object and return a boolean
    // indicating whether a piece gets captured.
    virtual bool Fire(const Player&, const ILaserable&);
    virtual void OnStep(int, int);

    inline int TargetIndex() const { return _targetIndex; }
    inline Square TargetSquare() const { return _targetSquare; }
    inline int TargetPiece() const { return _targetPiece; }
    inline int PathLength() const { return _pathLength; }

private:
    int _targetIndex;
    Square _targetSquare;
    int _targetPiece;
    int _pathLength;
};

#endif // __LASER_H__
