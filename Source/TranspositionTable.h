#ifndef __TRANSPOSITION_TABLE_H__
#define __TRANSPOSITION_TABLE_H__

#include <unordered_map>

typedef uint64_t Key;

enum class EntryType
{
    Exact,
    Alpha,
    Beta
};

// The format of a single TT entry.
class Entry
{
public:
    inline EntryType Type() const { return _type; }
    inline Move* HashMove() const { return _move; }
    inline int Depth() const { return _depth; }
    inline int Value() const { return _value; }
    inline int Age() const { return _age; }

    Entry() = delete;

    Entry(EntryType type, Move* move, int depth, int value) :
        _type(type), _move(move), _depth(depth), _value(value), _age(0)
    {
    }

    Entry(const Entry& other)
    {
        CopyFrom(other);
    }

    Entry& operator=(const Entry& other)
    {
        if (this != &other)
        {
            CopyFrom(other);
        }

        return *this;
    }

    Entry& operator++(int)
    {
        ++_age;
        return *this;
    }

    ~Entry()
    {
        if (_move != nullptr)
        {
            delete _move;
            _move = nullptr;
        }
    }

private:
    EntryType _type;
    Move* _move = nullptr;
    int _depth;
    int _value;
    int _age;

    void CopyFrom(const Entry& other)
    {
        _type = other._type;
        _move = new Move(*other._move);
        _depth = other._depth;
        _value = other._value;
        _age = other._age;
    }
};

class TranspositionTable
{
public:
    // Insert the specified key-value pair in the table.
    // This method decides whether the perform replacement.
    void Insert(Key key, EntryType type, Move* move, int depth, int value)
    {
        auto e = _map.find(key);
        if (e != _map.end())
        {
            // Should this entry be replaced with the new one?
            if (e->second.Depth() <= depth)
            {
                e->second = Entry(type, move, depth, value);
            }
        }
        else
        {
            _map.insert(std::make_pair(key, Entry(type, move, depth, value)));
        }
    }

    // Lookup the specified key.
    Entry* Find(Key key)
    {
        auto e = _map.find(key);
        return e != _map.end() ? &e->second : nullptr;
    }

    // Increase the age of all existing entries and remove them if they're too old.
    void Age()
    {
        const int MaxAge = 3;
        auto e = _map.begin();
        while (e != _map.end())
        {
            e->second++;
            e = e->second.Age() > MaxAge
                ? _map.erase(e)
                : std::next(e);
        }
    }

    // Clear the table and free all memory.
    void Clear()
    {
        auto e = _map.begin();
        while (e != _map.end())
        {
            e = _map.erase(e);
        }
    }

private:
    std::unordered_map<Key, Entry> _map;
};

typedef TranspositionTable TT;

#endif // __TRANSPOSITION_TABLE_H__
