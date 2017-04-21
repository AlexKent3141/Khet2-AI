#ifndef __MOVE_H__
#define __MOVE_H__

#include <cassert>

// This class describes a move.
class Move
{
public:
    Move() = default;

    Move(int start, int end, int rotation) : _start(start), _end(end), _rotation(rotation)
    {
    }

    Move(const Move& other)
    {
        CopyFrom(other);
    }

    Move& operator=(const Move& other)
    {
        if (this != &other)
        {
            CopyFrom(other);
        }

        return *this;
    }

    Move(const std::string& s)
    {
        assert(s.size() >= 3);
        _start = StringToLocation(s[0], s[1]);
        _end = s.size() == 3 ? _start : StringToLocation(s[2], s[3]);
        _rotation = 0;
        if (_start == _end)
            _rotation = s[2] == '+' ? 1 : -1;
    }

    inline int Start() const { return _start; }
    inline int End() const { return _end; }
    inline int Rotation() const { return _rotation; }

    std::string ToString() const
    {
        std::string s = LocationToString(_start);
        s += _start != _end ? LocationToString(_end) : (_rotation > 0 ? "+" : "-");
        return s;
    }

private:
    int _start, _end, _rotation;

    void CopyFrom(const Move& other)
    {
        _start = other._start;
        _end = other._end;
        _rotation = other._rotation;
    }

    std::string LocationToString(int loc) const
    {
        const std::string Files = "XabcdefghijX";
        return Files[loc % BoardWidth] + std::to_string(loc / BoardWidth);
    }

    int StringToLocation(char file, char rank)
    {
        return (rank - 48)*BoardWidth + (file - 96);
    }
};

#endif // __MOVE_H__

