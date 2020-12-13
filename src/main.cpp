#include "CommsHandler.h"
#include "Tests/TestBase.h"
#include "Tests/MoveGenTest.h"
#include "Tests/MakeMoveTest.h"
#include "Tests/TerminationTest.h"
#include "Tests/TestRunner.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc == 2 && std::string(argv[1]) == "test")
    {
        // Execute the unit tests.
        TestRunner runner;
        runner.RunTests<MoveGenTest>();
        runner.RunTests<MakeMoveTest>();
        runner.RunTests<TerminationTest>();
    }
    else
    {
        // Start listening for commands.
        CommsHandler handler;
        std::string message;
        while (std::getline(std::cin, message) && handler.Process(message));
    }

    return 0;
}
