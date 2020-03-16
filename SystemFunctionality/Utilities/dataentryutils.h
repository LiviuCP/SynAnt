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
        DATA_ENTRY_ADD_INVALID,
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
        ValidationCodesCount
    };

    namespace Misc
    {
        const QString c_DataEntryDescriptor{"Data Entry"};  // keep default capitalized, use toLower() where required
    }
}

#endif // DATAENTRYUTILS_H
