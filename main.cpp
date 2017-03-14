#include "Globals.h"
#include "Board.h"
#include "MoveGenerator.h"
#include "Move.h"
#include <iostream>

int main()
{
    Board b;
    std::cout << b.ToString() << std::endl;

    MoveGenerator gen(b);
    Move* move;
    while ((move = gen.Next()) != nullptr)
    {
        std::cout << move->ToString() << std::endl;
        delete move;
    }

    return 0;
}
