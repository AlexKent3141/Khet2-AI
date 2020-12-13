#include "CommsHandler.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    CommsHandler handler;
    std::string message;
    while (std::getline(std::cin, message) && handler.Process(message));

    return 0;
}
