/*
  This file contains specific items that are used by most classes
*/

#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <QMap>
#include <QVector>


/* only add the enum classes here that are shared by multiple classes */

namespace Game
{
    enum class InputWordNumber
    {
        ONE,
        TWO
    };

    enum class PieceTypes
    {
        BEGIN_PIECE,
        MIDDLE_PIECE,
        END_PIECE,
        PieceTypesCount
    };

    enum class Levels
    {
        LEVEL_EASY,
        LEVEL_MEDIUM,
        LEVEL_HARD,
        LEVEL_NONE
    };

    const QMap<Game::Levels, int> c_ScoreIncrements
    {
        {Game::Levels::LEVEL_EASY,   1},
        {Game::Levels::LEVEL_MEDIUM, 2},
        {Game::Levels::LEVEL_HARD,   4}
    };

    // use other increments when a time limit is enabled
    const QMap<Game::Levels, int> c_EnhancedScoreIncrements
    {
        {Game::Levels::LEVEL_EASY,   2},
        {Game::Levels::LEVEL_MEDIUM, 5},
        {Game::Levels::LEVEL_HARD,   12}
    };

    namespace Constraints
    {
        static constexpr int c_MinWordSize{5};
        static constexpr int c_MinPairSize{15};
        static constexpr int c_MaxPairSize{30};
    }
}

#endif // GAMEUTILS_H
