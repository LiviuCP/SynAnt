/*
  This file contains specific items that are mostly used by classes related to data entry
*/

#ifndef DATAENTRYUTILS_H
#define DATAENTRYUTILS_H

#include <QString>

#include "gameutils.h"

/* only add the enum classes here that are shared by multiple classes */

static const QString c_RawDataEntryHelpMessage          {
                                                             "\nPlease read following data entry instructions: \n\n"
                                                             "\n 1) Enter each required words pair within the provided text fields. "
                                                             "Choose the language from dropdown (click or hit ALT+L to open it) and select if the words are synonyms or antonyms. "
                                                             "Press click Add pair or hit ALT+A to validate the pair.\n"
                                                             "\n 2) In order to choose if the words are synonyms of antonyms by keyboard please navigate between radio buttons "
                                                             "and text fields by using TAB/SHIFT+TAB. When the required radio button is selected hit SPACE to have it toggled on.\n"
                                                             "\n 3) Press Save or hit CTRL+S to save the validated entries (pairs) to database. \n"
                                                             "\n 4) Press Discard or hit CTRL+D to cancel saving the validated entries "
                                                             "to database and clear them from buffer.\n"
                                                             "\n 5) Press Clear or hit ALT+X to erase the content of the text fields.\n"
                                                             "\n 6) Press Back or hit ALT+K to return to previous page.\n"
                                                             "\n 7) Following rules apply when adding new word pairs:\n"
                                                             " - minimum word size is %1 characters\n"
                                                             " - minimum total pair size is %2 characters\n"
                                                             " - the total pair size should not exceed %3 characters\n"
                                                             " - the words should not contain other characters than small letters\n"
                                                             " - the words from each pair should not be identical\n"
                                                             " - no duplicate pairs are allowed even if the order of the two words is different\n"
                                                             " - both words should be entered\n"
                                                             "\n 8) When quitting the application from the data entry dialog or pressing the Back button you will be prompted "
                                                             "to save any validated (but unsaved) entries to database. Three options are available:\n"
                                                             " - Yes (ALT+Y): save added pairs and then execute the requested operation (back/quit)\n"
                                                             " - No (ALT+N): discard added pairs and execute the requested operation\n"
                                                             " - Cancel(ALT+W): return to data entry dialog and resume operations\n"
                                                             "\n 9) When discarding unsaved entries (by using Discard button) you will be asked for confirmation. "
                                                             "Two options are available: \n"
                                                             " - Yes (CTRL+Y): discard pairs from data entry buffer\n"
                                                             " - Cancel (CTRL+W): cancel discarding, keep entries in the buffer\n"
                                                             "In both cases you will return to the data entry dialog after choosing the option.\n"
                                                             "\n 10) When exiting data entry the language is restored to default, namely then one setup in the "
                                                             "intro/main dialogs.\n"
                                                             "\n 11) While the data for a specific language is being loaded certain controls are disabled. "
                                                             "This includes: language change dropdown, data entry button, add pair, etc.\n"
                                                             "\nPress Ok to return to previous page.\n"
                                                        };

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

    namespace Messages
    {
        // help pane content when accessed from data entry dialog
        const QString c_DataEntryHelpMessage                {
                                                                 c_RawDataEntryHelpMessage
                                                                     .arg(Game::Constraints::c_MinWordSize)
                                                                     .arg(Game::Constraints::c_MinPairSize)
                                                                     .arg(Game::Constraints::c_MaxPairSize)
                                                            };

        // status messages
        const QString c_DataEntryStartMessage               {    "Welcome to data entry"                                                                    };
        const QString c_DataEntrySaveInProgressStartMessage {    "Welcome back to data entry. Please wait, data is currently being saved..."                };
        const QString c_DataEntryResumeMessage              {    "Word pairs entry resumed. There are %1 entries pending to be saved!"                      };
        const QString c_DataEntryStopMessage                {    "Word pairs entry stopped"                                                                 };
        const QString c_DataEntrySaveInProgressStopMessage  {    "Word pairs entry stopped, data will be continue to be saved"                              };

        const QString c_DataEntryDisabledMessage            {    "Entry disabled until fetch completes."                                                    };

        const QString c_DataEntryRequestMessage             {
                                                                 "Please enter a new pair of words in the required language.\n\n"
                                                                 "Choose if they are synonyms or antonyms."
                                                            };

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
        const QString c_DataSuccessfullySavedMessage        {    "%1 word pairs successfully saved to database, %2 pairs added to current game language"    };

        // invalid pair entry reason messages
        const QString c_WordHasLessThanMinCharacters        {    "At least one word has less than minimum required number of characters."                   };
        const QString c_PairHasLessThanMinCharacters        {    "The entered word pair has less than the minimum required number of characters."           };
        const QString c_PairHasMoreThanMaxCharacters        {    "The entered word pair has more than the allowed maximum number of characters."            };
        const QString c_InvalidCharactersEntered            {    "At least one of the words contains invalid characters."                                   };
        const QString c_PairHasIdenticalWords               {    "The entered word pair has identical words."                                               };
        const QString c_PairAlreadyExists                   {    "The entered word pair already exists."                                                    };
    }

    namespace Misc
    {
        const QString c_DataEntryDescriptor{"Data Entry"};  // keep default capitalized, use toLower() where required
    }
}

#endif // DATAENTRYUTILS_H
