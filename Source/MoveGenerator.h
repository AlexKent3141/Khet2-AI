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
    enum class Stage
    {
        Dynamic, // Moves that cause a capture/otherwise redirect the laser beam (not suicide).
        Quiet,
        Done
    };

    MoveGenerator() = delete;
    MoveGenerator(const Board&, Stage finalStage = Stage::Quiet);
    ~MoveGenerator();

    Move* Next();

private:
    bool _passiveCapture = false;
    int _moveIndex = -1;
    Stage _stage = Stage::Dynamic;
    Stage _stoppedStage;

    std::vector<Move*> _dynamicMoves;
    std::vector<Move*> _quietMoves;

    std::vector<Move*>* _currentMoves = &_dynamicMoves;

    // Cache the path of the laser.
    // Store the direction of the laser at each point.
    int _laserPath[BoardArea];

    void FireLaser(const Board&);

    void AddMove(const Board&, Move*);

    void NextStage();
    void Generate(const Board&);
    void GenerateAnubisMoves(const Board&, int);
    void GeneratePyramidMoves(const Board&, int);
    void GenerateScarabMoves(const Board&, int);
    void GeneratePharaohMoves(const Board&, int);
    void GenerateSphinxMoves(const Board&, int);
};

#endif // __MOVE_GENERATOR_H__
