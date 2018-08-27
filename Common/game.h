/*

  This file contains specific items that are used by multiple classes (e.g. enum classes)

*/

#ifndef GAME_H
#define GAME_H

#include <QMap>


namespace Game
{
    enum class Level {
        EASY,
        MEDIUM,
        HARD,
        NrOfLevels
    };

    enum class StatusCodes {
        SUCCESS,
        MISSING_WORDS,
        INCORRECT_WORDS,
        REQUESTED_BY_USER,
        NO_USER_INPUT,
        STATISTICS_RESET,
        LEVEL_CHANGED,
        StatusCodesCount
    };

    enum class StatisticsUpdate {
        FULL_UPDATE,
        PARTIAL_UPDATE
    };

    const QMap<Game::Level, int> c_ScoreIncrements{
                                                      {Game::Level::EASY,   1},
                                                      {Game::Level::MEDIUM, 2},
                                                      {Game::Level::HARD,   4}
                                                  };

    const QMap<Game::Level, int> c_WordPieceSizes{
                                                      {Game::Level::EASY,   3},
                                                      {Game::Level::MEDIUM, 2},
                                                      {Game::Level::HARD,   1}
                                                 };
}

#endif // GAME_H
