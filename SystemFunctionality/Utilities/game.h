/*

  This file contains specific items that are used by multiple classes (e.g. enum classes)

*/

#ifndef GAME_H
#define GAME_H

#include <QMap>


namespace Game
{
    enum class InputWordNumber
    {
        ONE,
        TWO,
        NrOfWords
    };

    enum class Level
    {
        EASY,
        MEDIUM,
        HARD,
        NrOfLevels
    };

    enum class StatusCodes
    {
        INVALID,
        SUCCESS,
        INCORRECT_WORDS,
        REQUESTED_BY_USER,
        LOADING_DATA,
        DATA_LOAD_COMPLETE,
        GAME_STARTED,
        GAME_PAUSED,
        GAME_RESUMED_COMPLETE_INPUT,
        GAME_RESUMED_INCOMPLETE_INPUT,
        STATISTICS_RESET_COMPLETE_INPUT,
        STATISTICS_RESET_INCOMPLETE_INPUT,
        LEVEL_CHANGED,
        PIECE_NOT_ADDED,
        PIECE_ADDED_COMPLETE_INPUT,
        PIECE_ADDED_INCOMPLETE_INPUT,
        PIECES_REMOVED,
        USER_INPUT_CLEARED,
        StatusCodesCount
    };

    enum class StatisticsUpdate
    {
        FULL_UPDATE,
        PARTIAL_UPDATE
    };

    enum class PieceTypes
    {
        BEGIN_PIECE,
        MIDDLE_PIECE,
        END_PIECE,
        PieceTypesCount
    };

    static constexpr int c_MinWordSize{5};
    static constexpr int c_MinPairSize{15};
    static constexpr int c_MaxPairSize{30};

    static constexpr int c_ToolTipDelay{1000};
    static constexpr int c_PaneSwitchingDelay{250};
    static constexpr int c_LoadDataThreadDelay{4000};
    static constexpr int c_ToolTipTimeout{4000};
    static constexpr int c_ShortStatusUpdateDelay{1000};
    static constexpr int c_LongStatusUpdateDelay{2000};
    static constexpr int c_NoDelay{-1};

    const QMap<Game::Level, int> c_ScoreIncrements
    {
        {Game::Level::EASY,   1},
        {Game::Level::MEDIUM, 2},
        {Game::Level::HARD,   4}
    };

    const QMap<Game::Level, int> c_WordPieceSizes
    {
        {Game::Level::EASY,   3},
        {Game::Level::MEDIUM, 2},
        {Game::Level::HARD,   1}
    };
}

#endif // GAME_H
