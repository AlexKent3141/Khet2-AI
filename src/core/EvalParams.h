#ifndef __EVAL_PARAMS_H__
#define __EVAL_PARAMS_H__

#include "Globals.h"
#include "Types.h"
#include <array>

// Store the parameters which define how the evaluation function is calculated.
class EvalParams
{
public:
    EvalParams() = default;

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
    inline int PiecePharaohVal(int dist) const { return _pieceToPharaohVals[dist]; }
    inline int LaserPharaohVal(int dist) const { return _laserToPharaohVals[dist]; }
    inline int LaserVal() const { return _laserVal; }
    inline int CheckmateVal() const { return _checkmateVal; }
    
private:
    // Setup the default parameter configuration.
    // With the piece vals only the Anubis and Pyramid values are significant.
    const int AnubisVal = 500;
    const int PyramidVal = 1000;

    std::array<int, 9> _pieceVals =
        { 0, 0, AnubisVal, PyramidVal, 0, 0, 0 };
    std::array<int, 16> _pieceToPharaohVals =
        { 0, 50, 40, 30, 20, 10, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::array<int, 16> _laserToPharaohVals =
        { 0, 200, 50, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    int _laserVal = 20;
    int _checkmateVal = 1000000;

    void CopyFrom(const EvalParams& other)
    {
        _pieceVals = other._pieceVals;
        _pieceToPharaohVals = other._pieceToPharaohVals;
        _laserToPharaohVals = other._laserToPharaohVals;
        _laserVal = other._laserVal;
        _checkmateVal = other._checkmateVal;
    }
};

#endif // __EVAL_PARAMS_H__
