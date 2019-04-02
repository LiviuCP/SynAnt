/*

  This file contains specific items that are used by multiple classes (e.g. enum classes)

*/

#ifndef GAME_H
#define GAME_H

#include <QMap>


namespace Game
{
    static constexpr int c_MinWordSize{5};

    enum class Level
    {
        EASY,
        MEDIUM,
        HARD,
        NrOfLevels
    };

    enum class StatusCodes
    {
        SUCCESS,
        INCORRECT_WORDS,
        REQUESTED_BY_USER,
        NO_USER_INPUT,
        STATISTICS_RESET,
        LEVEL_CHANGED,
        StatusCodesCount
    };

    enum class StatisticsUpdate
    {
        FULL_UPDATE,
        PARTIAL_UPDATE
    };

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

    static constexpr int c_ToolTipDelay{1000};
    static constexpr int c_ToolTipTimeout{4000};

    enum class PieceTypes
    {
        BEGIN_PIECE,
        MIDDLE_PIECE,
        END_PIECE,
        PieceTypesCount
    };

    struct WordPiece
    {
        QString content;
        PieceTypes pieceType;
        bool isSelected;
    };
}

#endif // GAME_H
