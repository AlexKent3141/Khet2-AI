#include "Globals.h"
#include "Tests/TestBase.h"
#include "Tests/MoveGenTest.h"
#include "Tests/TestRunner.h"
#include <iostream>

int main()
{
    TestRunner runner;
    runner.RunTests<MoveGenTest>();

    return 0;
}
