#ifndef __PATH_LASER_H__
#define __PATH_LASER_H__

#include "Globals.h"
#include "Laser.h"
#include <cstring>

class PathLaser : public Laser
{
public:
    bool Fire(const Player& player, const ILaserable& board)
    {
        memset(_laserPath, -1, BoardArea*sizeof(int));
        return Laser::Fire(player, board);
    }

    void OnStep(int targetIndex, int postDir)
    {
        _laserPath[targetIndex] = postDir;
    }

    inline int PathAt(int loc) const { return _laserPath[loc]; }

private:
    int _laserPath[BoardArea];
};

#endif // __PATH_LASER_H__
