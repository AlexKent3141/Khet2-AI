#include "CommsHandler.h"
#include "core/MoveHelpers.h"
#include "core/Utils.h"
#include "core/Types.h"
#include <cassert>
#include <iostream>

bool CommsHandler::Process(const std::string& message)
{
    bool alive = true;

    auto tokens = Utils::Split(message, ' ');
    std::string messageType = tokens[0];
    if (messageType == "newgame")
    {
        _calculator.Stop();
        _table.Clear();
    }
    else if (messageType == "position")
    {
        _board = CreatePosition(tokens);
    }
    else if (messageType == "go")
    {
        assert(_board != nullptr);
        _table.Age();
        auto params = CreateSearchParameters(tokens);
        _calculator.Start(_table, std::move(params), *_board);
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

std::shared_ptr<Board> CommsHandler::CreatePosition(
    const std::vector<std::string>& tokens) const
{
    assert(tokens.size() > 1);
    std::shared_ptr<Board> board;
    size_t movesIndex = 2;
    if (tokens[1] == "standard")
        board = std::make_shared<Board>(Standard);
    else if (tokens[1] == "dynasty")
        board = std::make_shared<Board>(Dynasty);
    else if (tokens[1] == "imhotep")
        board = std::make_shared<Board>(Imhotep);
    else
    {
        // It's a Khet string.
        std::string ks = tokens[1] + " " + tokens[2];
        board = std::make_shared<Board>(ks);
        movesIndex = 3;
    }

    // Apply the moves that have been specified.
    Move move = NoMove;
    for (size_t i = movesIndex; i < tokens.size(); i++)
    {
        move = MakeMove(tokens[i]);
        board->MakeMove(move);
    }

    return board;
}

std::unique_ptr<SearchParams> CommsHandler::CreateSearchParameters(
    const std::vector<std::string>& tokens) const
{
    assert(tokens.size() > 1);
    auto params = std::make_unique<SearchParams>();
    std::string paramType = tokens[1];
    if (paramType == "infinite")
    {
        params->Infinite(true);
    }
    else
    {
        assert(tokens.size() == 3);
        int val = stoi(tokens[2]);
        if (paramType == "movetime")
        {
            params->MoveTime(val);
        }
        else if (paramType == "depth")
        {
            params->Depth(val);
        }
    }

    return params;
}
