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
        NO_DATA_LOADING_REQUESTED,
        LOADING_DATA,
        DATA_LOADING_COMPLETE,
        NO_VALID_DATA_ENTRIES_LOADED,
        DATA_LOADING_ERROR,
        DATA_SAVE_IN_PROGRESS,
        DATA_SUCCESSFULLY_SAVED,
        DATA_ENTRY_SAVING_ERROR,
        DATA_GOT_AVAILABLE,
        DATA_ENTRY_STARTED,
        DATA_ENTRY_RESUMED,
        DATA_ENTRY_SUCCESS,
        PAIR_ALREADY_ADDED,
        DATA_ENTRY_INVALID,
        CACHE_RESET,
        GAME_STARTED,
        GAME_PAUSED,
        GAME_RESUMED_COMPLETE_INPUT,
        GAME_RESUMED_INCOMPLETE_INPUT,
        GAME_STOPPED,
        CORRECT_USER_INPUT,
        INCORRECT_USER_INPUT,
        SOLUTION_REQUESTED_BY_USER,
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

    enum class ValidationCodes
    {
        NO_PAIR_VALIDATED,
        VALID_PAIR,
        LESS_MIN_CHARS_PER_WORD,
        LESS_MIN_TOTAL_PAIR_CHARS,
        MORE_MAX_TOTAL_PAIR_CHARS,
        INVALID_CHARACTERS,
        PAIR_ALREADY_EXISTS,
        IDENTICAL_WORDS,
        ReasonCodesCount
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
    static constexpr int c_WriteDataThreadDelay{500};
    static constexpr int c_ToolTipTimeout{4000};
    static constexpr int c_ShortStatusUpdateDelay{1000};
    static constexpr int c_LongStatusUpdateDelay{2000};
    static constexpr int c_GameQuitDelay{200};
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
