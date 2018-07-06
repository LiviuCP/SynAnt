/*

  This file contains specific items that are used by multiple classes (e.g. enum classes)

*/

#ifndef GAME_H
#define GAME_H


namespace Game
{
    enum class Level {                                                                              // used for updating the parameters when the difficulty level changes
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
}

#endif // GAME_H
