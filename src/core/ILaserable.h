#ifndef __ILASERABLE_H__
#define __ILASERABLE_H__

#include "Bitboard.h"

// This class is an interface for objects which can be lasered.
class ILaserable
{
public:
    virtual ~ILaserable() {}
    virtual Square Get(int) const = 0;
    virtual BB GetPieces() const = 0;
};

#endif // __ILASERABLE_H__
