/*
  This file contains specific items that are mostly used by classes related to data entry
*/

#ifndef DATAENTRYUTILS_H
#define DATAENTRYUTILS_H

#include <QString>

/* only add the enum classes here that are shared by multiple classes */

namespace DataEntry
{
    enum class StatusCodes
    {
        NO_DATA_ENTRY_REQUESTED,
        DATA_ENTRY_STARTED,
        DATA_ENTRY_RESUMED,
        DATA_ENTRY_ADD_SUCCESS,
        DATA_ENTRY_ADD_INVALID,
        PAIR_ALREADY_ADDED,
        RESET_CACHE_REQUESTED,
        CACHE_RESET,
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

    namespace Messages
    {
        const QString c_DataEntryStartMessage               {    "Word pairs entry started"                                                                 };
        const QString c_DataEntryResumeMessage              {    "Data entry resumed. There are %1 entries pending to be saved!"                            };
        const QString c_DataEntryRequestMessage             {    "Please enter a new pair of words.\n\nChoose if they are synonyms or antonyms"             };

        const QString c_DataEntrySuccessMessage             {
                                                                 "The new pair has been successfully added to validated pairs.\n\n"
                                                                 "There are %1 entries pending to be saved.\n\n"
                                                                 "Press Save to add the already validated pairs to game database "
                                                                 "and make the available for playing."
                                                            };

        const QString c_DataEntryInvalidPairMessage         {
                                                                 "The new words pair has not been correctly entered.\n\n"
                                                                 "%1\n\n"
                                                                 "Please correct and try again!"
                                                            };

        const QString c_DataEntryPairAlreadyAddedMessage    {    "The pair has already been added. Please save added pairs to database or discard them"     };

        const QString c_DataCacheResetInProgressMessage     {    "Discarding unsaved entries"                                                               };
        const QString c_DataCacheResetMessage               {    "The unsaved entries been discarded"                                                       };
        const QString c_DataSaveInProgressMessage           {    "Data is currently being saved..."                                                         };
        const QString c_DataSuccessfullySavedMessage        {    "%1 word pairs have been successfully saved to database"                                   };

        // invalid pair entry reason messages
        const QString c_WordHasLessThanMinCharacters        {    "At least one word has less than minimum required number of characters."                   };
        const QString c_PairHasLessThanMinCharacters        {    "The entered word pair has less than the minimum required number of characters."           };
        const QString c_PairHasMoreThanMaxCharacters        {    "The entered word pair has more than the allowed maximum number of characters."            };
        const QString c_InvalidCharactersEntered            {    "At least one of the words contains invalid characters."                                   };
        const QString c_PairHasIdenticalWords               {    "The entered word pair has identical words."                                               };
        const QString c_PairAlreadyExists                   {    "The entered word pair already exists."                                                    };
    }
}

#endif // DATAENTRYUTILS_H
