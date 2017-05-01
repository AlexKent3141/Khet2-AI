#ifndef __MOVE_GENERATOR_H__
#define __MOVE_GENERATOR_H__

#include "Globals.h"
#include "Board.h"
#include "Move.h"

// Generate all of the moves for the specified position in order of urgency.
class MoveGenerator
{
public:
    // Which stage of moves we are currently on.
    static const int Priority = 0;
    static const int Dynamic = 1;
    static const int Quiet = 2;
    static const int Suicide = 3;
    static const int Done = 4;

    MoveGenerator() = delete;
    MoveGenerator(const Board&, int finalStage = Suicide);
    MoveGenerator(const Board&, Move*, int finalStage = Suicide);
    ~MoveGenerator();

    Move* Next();

private:
    bool _passiveCapture = false;
    int _moveIndex = -1;
    int _stage = Priority;
    int _stoppedStage;

    // Maintain a buffer for each stage.
    std::vector<Move*> _moveBuffers[Done];
    std::vector<Move*>* _currentMoves = &_moveBuffers[Priority];

    // Cache the path of the laser.
    // Store the direction of the laser at each point.
    int _laserPath[BoardArea];

    void FireLaser(const Board&);

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
