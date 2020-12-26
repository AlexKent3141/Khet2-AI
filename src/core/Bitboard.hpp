#ifndef __KHET_BIT_BOARD_H__
#define __KHET_BIT_BOARD_H__

#include "Globals.h"
#include <cassert>
#include <cstdint>

#ifdef _MSC_VER
#include "intrin.h"

    inline int ctz(uint64_t word)
    {
        unsigned long i;
        _BitScanForward64(&i, word);
        return i;
    }

    inline int clz(uint64_t word)
    {
        return __lzcnt64(word);
    }

    inline int popcnt(uint64_t word)
    {
        return __popcnt64(word);
    }
#elif __GNUC__
    inline int ctz(uint64_t word)
    {
        return __builtin_ctzll(word);
    }

    inline int clz(uint64_t word)
    {
        return __builtin_clzll(word);
    }

    inline int popcnt(uint64_t word)
    {
        return __builtin_popcountll(word);
    }
#else
#error "No implementation of ctz, clz & popcnt."
#endif

// The Khet board is 10x8, so no single integer type can mask the whole board.
// The approach I'm taking is to use one 64 bit word to cover the lower 5x8
// half of the board and another for the upper 5x8 half.
class BB
{
public:
    BB();
    BB(const BB&);
    BB& operator==(const BB&);

    inline operator bool() const { return lower_ || upper_; }

    inline BB operator~() const
    {
        BB inverted;
        inverted.lower_ = (~lower_ & First40Bits);
        inverted.upper_ = (~upper_ & First40Bits);
        return inverted;
    }

    inline BB& operator&=(const BB& other)
    {
        lower_ &= other.lower_;
        upper_ &= other.upper_;
        return *this;
    } 
    inline BB& operator|=(const BB& other)
    {
        lower_ |= other.lower_;
        upper_ |= other.upper_;
        return *this;
    }

    inline BB& operator^=(const BB& other)
    {
        lower_ ^= other.lower_;
        upper_ ^= other.upper_;
        return *this;
    }

    inline void Set(int i)
    {
        assert(i < WholeBoard);
        if (i < HalfBoard)
            lower_ |= (One << i);
        else
            upper_ |= (One << (i - HalfBoard));
    }

    inline void Unset(int i)
    {
        assert(i < WholeBoard);
        if (i < HalfBoard)
            lower_ &= ~(One << i);
        else
            upper_ &= ~(One << (i - HalfBoard));
    }

    inline bool Test(int i) const
    {
        assert(i < WholeBoard);
        if (i < HalfBoard)
            return lower_ & (One << i);
        else
            return upper_ & (One << (i - HalfBoard));
    }

    inline void Reset()
    {
        lower_ = 0;
        upper_ = 0;
    }

    inline int LSB() const
    {
        return lower_
            ? ctz(lower_)
            : ctz(upper_) + HalfBoard;
    }

    inline int MSB() const
    {
        return upper_
            ? 63 - clz(upper_) + HalfBoard
            : 63 - clz(lower_);
    }

    inline int PopLSB()
    {
        int bit;
        if (lower_)
        {
            bit = ctz(lower_);
            lower_ &= ~(One << bit);
        }
        else
        {
            bit = ctz(upper_);
            upper_ &= ~(One << bit);
            bit += HalfBoard;
        }

        return bit;
    }

    inline int PopMSB()
    {
        int bit;
        if (upper_)
        {
            bit = 63 - clz(upper_);
            upper_ &= ~(One << bit);
            bit += HalfBoard;
        }
        else
        {
            bit = 63 - clz(lower_);
            lower_ &= ~(One << bit);
        }

        return bit;
    }

    inline int PopCount() const
    {
        return popcnt(lower_)
             + popcnt(upper_);
    }

private:
    static constexpr uint64_t One = 1;
    static constexpr int WholeBoard = 80;
    static constexpr int HalfBoard = WholeBoard/2;
    static constexpr uint64_t First40Bits = 0xFFFFFFFFFF;

    uint64_t lower_;
    uint64_t upper_;
};

BB operator&(BB a, const BB& b);
BB operator|(BB a, const BB& b);
BB operator^(BB a, const BB& b);

namespace Bitboards
{
    // Rays from each location in each orthogonal direction.
    extern BB rays[BoardArea][4];

    // The neighbours of each location.
    extern BB neighbours[BoardArea];

    // The locations that each player can move to.
    extern BB canMove[2];

    void Initialise();

    void PrettyPrint(const BB&);
}

#endif // __KHET_BIT_BOARD_H__
