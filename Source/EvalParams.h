#ifndef __EVAL_PARAMS_H__
#define __EVAL_PARAMS_H__

#include "Globals.h"
#include "Types.h"
#include <cstring>

// Store the parameters which define how the evaluation function is calculated.
class EvalParams
{
public:
    EvalParams()
    {
        // Setup the default parameter configuration.
        // With the piece vals only the Anubis and Pyramid values are significant.
        const int AnubisVal = 400;
        const int PyramidVal = 1000;
        _pieceVals = new int[7] { 0, 0, AnubisVal, PyramidVal, 0, 0, 0 };
        _laserVal = 40;

        // This evaluation is only attainable if a player is in checkmate.
        _checkmateVal = 3*AnubisVal + 8*PyramidVal + BoardArea*_laserVal;
    }

    ~EvalParams()
    {
        if (_pieceVals != nullptr)
        {
            delete _pieceVals;
            _pieceVals = nullptr;
        }
    }

    EvalParams(const EvalParams& other)
    {
        CopyFrom(other);
    }

    EvalParams& operator=(const EvalParams& other)
    {
        if (this != &other)
        {
            CopyFrom(other);
        }

        return *this;
    }

    inline int PieceVal(Piece p) const { return _pieceVals[(int)p]; }
    inline int LaserVal() const { return _laserVal; }
    inline int CheckmateVal() const { return _checkmateVal; }
    
private:
    int* _pieceVals = nullptr;
    int _laserVal;
    int _checkmateVal;

    void CopyFrom(const EvalParams& other)
    {
        memcpy(_pieceVals, other._pieceVals, 7*sizeof(int));
        _laserVal = other._laserVal;
        _checkmateVal = other._checkmateVal;
    }
};

#endif // __EVAL_PARAMS_H__

