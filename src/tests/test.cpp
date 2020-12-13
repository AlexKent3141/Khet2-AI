#include "MoveGenTest.h"
#include "MakeMoveTest.h"
#include "TerminationTest.h"
#include "TestRunner.h"

int main()
{
    // Execute the unit tests.
    TestRunner runner;
    runner.RunTests<MoveGenTest>();
    runner.RunTests<MakeMoveTest>();
    runner.RunTests<TerminationTest>();

    return 0;
}
