#ifndef __MOVE_GENERATOR_H__
#define __MOVE_GENERATOR_H__

#include "Board.h"
#include "Globals.h"
#include "History.h"
#include "PathLaser.h"
#include "Types.h"
#include <array>

// Generate all of the moves for the specified position in order of urgency.
class MoveGenerator
{
public:
    // Which stage of moves we are currently on.
    enum Stage
    {
        Priority = 0,
        Dynamic = 1,
        Quiet = 2,
        Suicide = 3,
        Done = 4
    };

    MoveGenerator() = delete;
    MoveGenerator(const Board&, int finalStage = Suicide);
    MoveGenerator(const Board&, Move, Move, int finalStage = Suicide);

    // Sort the moves in the specified stage according to historical score.
    void Sort(Stage, const History&);

    Move Next();

private:
    bool _passiveCapture = false;
    int _moveIndex = -1;
    int _stage = Priority;
    int _stoppedStage;
    Player _playerToMove;
    PathLaser _laser;

    // Maintain a buffer for each stage.
    std::array<Move, Stage::Done*MaxMoves> _moves;
    std::array<int, Stage::Done> _numMovesPerStage;
    Move* _currentMoves = &_moves[0];

    void AddMove(const Board&, int, int, int);

    void NextStage();
    void Generate(const Board&);
    void GenerateAnubisMoves(const Board&, int);
    void GeneratePyramidMoves(const Board&, int);
    void GenerateScarabMoves(const Board&, int);
    void GeneratePharaohMoves(const Board&, int);
    void GenerateSphinxMoves(const Board&, int);
};

#endif // __MOVE_GENERATOR_H__
