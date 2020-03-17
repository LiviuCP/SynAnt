/*
  This file contains specific items that are mostly used by classes related to data entry
*/

#ifndef DATAENTRYUTILS_H
#define DATAENTRYUTILS_H

#include <QString>

#include "gameutils.h"

/* only add the enum classes here that are shared by multiple classes */

namespace DataEntry
{
    enum class StatusCodes
    {
        NO_DATA_ENTRY_REQUESTED,
        DATA_ENTRY_STARTED,
        DATA_ENTRY_STARTED_SAVE_IN_PROGRESS,
        DATA_ENTRY_RESUMED,
        DATA_ENTRY_STOPPED,
        DATA_ENTRY_STOPPED_SAVE_IN_PROGRESS,
        DATA_ENTRY_DISABLED,
        DATA_ENTRY_ADD_SUCCESS,
        ADD_FAILED_LESS_MIN_CHARS_PER_WORD,
        ADD_FAILED_LESS_MIN_TOTAL_PAIR_CHARS,
        ADD_FAILED_MORE_MAX_TOTAL_PAIR_CHARS,
        ADD_FAILED_INVALID_CHARACTERS,
        ADD_FAILED_PAIR_ALREADY_EXISTS,
        ADD_FAILED_IDENTICAL_WORDS,
        PAIR_ALREADY_ADDED,
        RESET_CACHE_REQUESTED,
        CACHE_RESET,
        FETCHING_DATA,
        DATA_FETCHING_FINISHED,
        DATA_FETCHING_FINISHED_SAVE_IN_PROGRESS,
        DATA_SAVE_IN_PROGRESS,
        DATA_SUCCESSFULLY_SAVED,
        StatusCodesCount
    };
}

#endif // DATAENTRYUTILS_H
