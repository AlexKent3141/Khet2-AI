#ifndef __PATH_LASER_H__
#define __PATH_LASER_H__

#include "Globals.h"
#include "Laser.h"
#include <cstring>

class PathLaser : public Laser
{
public:
    bool Fire(
        const Player& player,
        const ILaserable& board) override
    {
        return Laser::Fire(player, board);
    }

    // This method calculates whether a piece will die when the laser is fired
    // after the specified move has been made.
    bool FireWillKill(
        const Player& player,
        const ILaserable& board,
        int start,
        int end,
        Square finalSq,
        Square initialEndSq,
        int* killLoc)
    {
        BB allPieces = board.GetPieces();

        allPieces.Unset(start);

        if (allPieces.Test(end))
        {
            // It's a swap.
            allPieces.Set(start);
        }

        allPieces.Set(end);

        // Find the starting location and direction for the laserbeam.
        int ti = Sphinx[(int)player];
        int ts = Empty;
        int dirIndex = GetOrientation(board.Get(ti));
        while (dirIndex >= 0)
        {
            BB attacked = Bitboards::rays[ti][dirIndex] & allPieces;

            if (!attacked)
            {
                // Nothing is killed - not sure what to return!
                return false;
            }

            ti = dirIndex < 2
                ? attacked.LSB()
                : attacked.MSB();

            // Is this location occupied?
            if (ti == start && start != end)
            {
                ts = initialEndSq; // This way we support swaps.
            }
            else if (ti == end)
            {
                ts = finalSq;
            }
            else
            {
                ts = board.Get(ti);
            }

            if (IsPiece(ts))
            {
                int tp = (int)GetPiece(ts);
                dirIndex = Reflections[dirIndex][tp - 2][GetOrientation(ts)];
            }
        }

        *killLoc = ti;
        return true;
    }
};

#endif // __PATH_LASER_H__
