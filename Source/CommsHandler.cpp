#include "CommsHandler.h"
#include "Globals.h"
#include "Utils.h"
#include <cassert>
#include <iostream>

bool CommsHandler::Process(const std::string& message)
{
    bool alive = true;

    auto tokens = Utils::GetInstance()->Split(message, ' ');
    std::string messageType = tokens[0];
    if (messageType == "newgame")
    {
        if (_board != nullptr)
            delete _board;
    }
    else if (messageType == "position")
    {
        if (_board != nullptr)
            delete _board;

        _board = CreatePosition(tokens);
        std::cout << _board->ToString() << std::endl;
    }
    else if (messageType == "go")
    {
        assert(_board != nullptr);
        SearchParams params;
        params.Depth(4);
        _calculator.Start(params, *_board);
    }
    else if (messageType == "stop")
    {
        _calculator.Stop();
    }
    else if (messageType == "quit")
    {
        _calculator.Stop();
        alive = false;
    }
    else
    {
        std::cout << "info string " << "Unrecognised message type: " << message << std::endl;
    }

    return alive;
}

Board* CommsHandler::CreatePosition(const std::vector<std::string>& tokens)
{
    assert(tokens.size() > 1);
    Board* board = nullptr;
    size_t movesIndex = 2;
    if (tokens[1] == "standard")
    {
        board = new Board(Standard);
    }
    else
    {
        // It's a Khet string.
        std::string ks = tokens[1] + " " + tokens[2];
        board = new Board(ks);
        movesIndex = 3;
    }

    // Apply the moves that have been specified.
    Move* move = nullptr;
    for (size_t i = movesIndex; i < tokens.size(); i++)
    {
        move = new Move(tokens[i]);
        board->MakeMove(move);
        delete move;
    }

    return board;
}

