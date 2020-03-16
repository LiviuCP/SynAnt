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
    enum class StatusCodes
    {
        NO_LANGUAGE_SET,
        FETCHING_DATA,
        DATA_FETCHING_COMPLETE,
        NO_DATA_ENTRIES_FETCHED,
        DATA_FETCHING_ERROR,
        DATA_GOT_AVAILABLE,
        DATA_STILL_UNAVAILABLE,
        GAME_STARTED,
        GAME_PAUSED,
        GAME_RESUMED_COMPLETE_INPUT,
        GAME_RESUMED_INCOMPLETE_INPUT,
        GAME_STOPPED,
        NEW_DATA_SAVE_IN_PROGRESS,
        ADDITIONAL_DATA_SAVE_IN_PROGRESS,
        DATA_SUCCESSFULLY_SAVED,
        DATA_ENTRY_SAVING_ERROR,
        PIECE_NOT_ADDED,
        PIECE_ADDED_COMPLETE_INPUT,
        PIECE_ADDED_INCOMPLETE_INPUT,
        PIECES_REMOVED,
        USER_INPUT_CLEARED,
        CORRECT_USER_INPUT,
        INCORRECT_USER_INPUT,
        SOLUTION_REQUESTED_BY_USER,
        STATISTICS_RESET_COMPLETE_INPUT,
        STATISTICS_RESET_INCOMPLETE_INPUT,
        LEVEL_CHANGED,
        PERSISTENT_MODE_ENTERED,
        PERSISTENT_MODE_EXITED,
        PERSISTENT_INDEX_REQUIRED,
        TIME_LIMIT_ENABLED,
        TIME_LIMIT_DISABLED,
        TIME_LIMIT_REACHED,
        StatusCodesCount
    };

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

    const QMap<Game::Levels, int> c_TimeLimits
    {
        {Game::Levels::LEVEL_EASY, 30},
        {Game::Levels::LEVEL_MEDIUM, 120},
        {Game::Levels::LEVEL_HARD, 300}
    };

    namespace Timing
    {
        static constexpr int c_PaneSwitchingDelay{350};
        static constexpr int c_LoadDataThreadDelay{1000};
        static constexpr int c_WriteDataThreadDelay{1000};
        static constexpr int c_ResetCacheDelay{250};
        static constexpr int c_ShortStatusUpdateDelay{1000};
        static constexpr int c_LongStatusUpdateDelay{2000};
        static constexpr int c_GameQuitDelay{200};
        static constexpr int c_NoDelay{-1};
    }

    namespace Constraints
    {
        static constexpr int c_MinWordSize{5};
        static constexpr int c_MinPairSize{15};
        static constexpr int c_MaxPairSize{30};
    }
}

#endif // GAMEUTILS_H
