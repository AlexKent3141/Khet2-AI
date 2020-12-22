#include "CommsHandler.h"
#include "core/Zobrist.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    Zobrist::Initialise();
    Bitboards::Initialise();

    CommsHandler handler;
    std::string message;
    while (std::getline(std::cin, message) && handler.Process(message));

    return 0;
}
