/*
  This file contains specific items that are used by most classes
*/

#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <QMap>
#include <QVector>

static const QString c_RawGameHelpMessage                   {
                                                                 "\nPlease read following game instructions: \n\n"
                                                                 "\n 1) At game startup the language should be chosen via dropdown from the intro window  (click or hit ALT+L to open it). "
                                                                 "The language can then be changed from the main page. If the language contains no word pairs "
                                                                 "use the the data entry dialog to add data to it. In main game window it is not possible to "
                                                                 "switch to a language that doesn't contain any valid pairs.\n"
                                                                 "\n 2) Minimum word size is %1 characters. Minimum total pair size is %2 characters. "
                                                                 "The total pair size should not exceed %3 characters.\n"
                                                                 "\n 3) Each word is divided in equal parts, "
                                                                 "except the last part, which might have a smaller number of characters.\n"
                                                                 "\n 4) The parts (pieces) from both words are mixed with each other.\n"
                                                                 "\n 5) The first (beginning) part of each word is marked blue.\n"
                                                                 "\n 6) The last (end) part of each word is marked red.\n"
                                                                 "\n 7) The words can either be synonyms or antonyms.\n"
                                                                 "\n 8) Please build the two words by using the displayed word pieces.\n"
                                                                 "\n 9) Single-click on each word piece to add it to the first word. "
                                                                 "Double-click for adding the piece to the second word. "
                                                                 "If the word piece is accepted to the user input (according to below rules) "
                                                                 "it will be removed from the mixed words area and will be assigned "
                                                                 "to the appropriate word in the input area below. "
                                                                 "The borders of the first word pieces which are added to input are colored green "
                                                                 "while the borders of the second word input pieces are colored orange.\n"
                                                                 "\n10) For removing one or more pieces from an input word click on "
                                                                 "the first piece to be removed. "
                                                                 "Only a contiguous chunk of pieces (ending with the last input piece) can be erased. "
                                                                 "When hovering with the mouse on the first piece to be deleted the chunk "
                                                                 "is highlighted with brown color.\n"
                                                                 "\n11) For removing all input pieces at once please press the Clear input button. Also for "
                                                                 "removing individual word inputs use the shortcuts ALT+4 for the first word and ALT+5 for "
                                                                 "the second word input.\n"
                                                                 "\n12) There is also an interactive mode (a.k.a. cursor mode) for adding and removing word pieces to/from input: \n"
                                                                 " - click C to enter cursor mode. Navigate with the TAB key between the three containers: mixed pieces, "
                                                                 "first word input and second word input.\n"
                                                                 " - In mixed pieces container navigate with the left/right arrows until the required piece "
                                                                 "is selected. Then click the down arrow to add it to the first word or up to add it to the second word.\n"
                                                                 " - when entering a input container last piece is selected. To select more pieces navigate with the left arrow. "
                                                                 "To select less pieces use the right arrow. Hit up/down to remove the selected pieces from input.\n"
                                                                 " - to exit the interactive mode click ESC.\n"
                                                                 "\n13) In interactive mode:\n"
                                                                 " - the mouse click on any pieces from mixed words container or input containers is disabled\n"
                                                                 " - however the clear input button and shortcut can be used the same way as in non-cursor mode\n"
                                                                 " - same applies to the shortcuts used for removing the whole input of one of the words\n"
                                                                 " - when hitting tab if a container is empty it is ignored and the cursor will go to the next available container. "
                                                                 " If only one container is available the cursor will remain there.\n"
                                                                 " - when adding the last piece to input the cursor will automatically jump to the last piece of the first word "
                                                                 "input container.\n"
                                                                 " - the last cursor position within a container is not memorized when the user moves to a different container.\n"
                                                                 " - when deleting a word piece from input the cursor automatically moves to the mixed pieces container.\n"
                                                                 " - the cursor can be moved in a circular fashion within the mixed pieces container and in a left/right way in the "
                                                                 "input containers.\n"
                                                                 " - the cursor position within the mixed pieces container is updated to the first available start piece if this is "
                                                                 "the only possible piece type to be added to input in the current state.\n"
                                                                 " - if one word is complete the cursor moves to an available middle-piece each time a new part is added to input unless the "
                                                                 "only remaining part to be added is the end piece.\n"
                                                                 "\n14) You need to build both words correctly to win the round.\n"
                                                                 "\n15) Following rules are enforced by app when building the user input:\n"
                                                                 " - each word should start with a start piece (blue text)\n"
                                                                 " - each word should end with an end piece (red text)\n"
                                                                 " - each word contains zero or more middle-pieces (black text) in between\n"
                                                                 " - if the first word is closed you cannot close the second one if there "
                                                                 "is more than one piece left to add to input\n"
                                                                 "\n16) After building the two words, press Submit for validating input.\n"
                                                                 "\n17) If the input is not correct, the user will be asked "
                                                                 "to re-enter the words. Use the chunk delete functionality mentioned above "
                                                                 "and then re-enter erased pieces to correct the input. Press Submit again once finished.\n"
                                                                 "\n18) If the two words have been correctly determined, the program will "
                                                                 "confirm the success and display 2 new mixed words for guessing.\n"
                                                                 "\n19) Two new words will also be presented for guessing "
                                                                 "if the Show results button is pressed or when the level is changed.\n"
                                                                 "\n20) The status box is located on the right side of the radio buttons. "
                                                                 "It displays if the words have been correctly entered or not and other status messages.\n"
                                                                 "\n21) The status box also displays the right solution "
                                                                 "when the Show results button is pressed.\n"
                                                                 "\n22) The scores and number of word pairs are displayed in the upper part "
                                                                 "of the window.\n"
                                                                 "\n23) The format of the score and number of word pairs is: "
                                                                 "guessed/total.\n"
                                                                 "\n24) The guessed score and number of pairs are increased only when "
                                                                 "a correct guess is made. The total score and number of pairs are increased each time "
                                                                 "a new pair of words is provided to the user for guessing without having the level changed. "
                                                                 "When the level is changed all statistics remain constant. \n"
                                                                 "\n25) Press the Reset button to set the statistics (scores and number of pairs) to 0.\n"
                                                                 "\n26) Choose the desired difficulty level by using the radio buttons.\n"
                                                                 "\n27) The difficulty level determines the size of the parts in which "
                                                                 "the words are divided.\n"
                                                                 "\n28) It also determines the obtained score for correctly guessing "
                                                                 "a pair of words.\n"
                                                                 "\n29) The  score obtained for each correctly guessed pair is: "
                                                                 "%4 point/s (easy), %5 points (medium), %6 points (hard).\n"
                                                                 "\n30) When changing the difficulty level, the score and number of word "
                                                                 "pairs remain unchanged.\n"
                                                                 "\n31) However, when the level is changed, a new pair of words "
                                                                 "(divided according to the new level) will be presented to the user. Same thing happens when "
                                                                 "changing the language.\n"
                                                                 "\n32) Each button has an appropriate key combination (shortcut).\n"
                                                                 "\n33) Unless mentioned otherwise for most push buttons the key combination is "
                                                                 "ALT+[first character of the button name]. "
                                                                 "For example the key combination for Quit is ALT+Q.\n"
                                                                 "\n34) For the Show results button the key combination is ALT+B. "
                                                                 "For Clear input the shortcut ALT+Z could be used.\n"
                                                                 "\n35) Hit ALT+1/2/3 for setting the difficulty levels to "
                                                                 "easy/medium/hard.\n"
                                                                 "\n36) It is possible to setup a time limit for guessing each word pair. This means the user should enter the "
                                                                 "correct words in the given time. Use the switch from the intro or main pane to enable/disable this feature. "
                                                                 "Alternatively you can hit ALT+T to toggle the button.\n"
                                                                 "\n37) Following should be taken into consideration when setting up a time limit: \n"
                                                                 " - after enabling the time limit from intro pane the counter will start once entering the main pane\n"
                                                                 " - at timeout a new pair is provided to user but no points are earned. The countdown is restarted.\n"
                                                                 " - the counter starts/stops immediately after time limit has been enabled/disabled from main pane\n"
                                                                 " - the counter pauses when temporarily leaving the main pane (to help or data entry) and resumes once "
                                                                 "returning to main\n"
                                                                 " - the time limit depends on level (a higher level means a higher limit)\n"
                                                                 " - the counter gets reset each time a new words pair is provided to user\n"
                                                                 " - the time limit switch is disabled while a new language is being setup in the main pane\n"
                                                                 " - for the same level the user gets more points when correctly guessing the pair while the time limit is enabled. "
                                                                 "The obtained score per pair is: %7 points (easy), %8 points (medium), %9 points (hard).\n"
                                                                 "\n38) Use the data entry page to enter additional word pairs. "
                                                                 "Press the Enter new pairs button or hit ALT+E to access the page. "
                                                                 "For more details please access the Help section from the data entry page. \n"
                                                                 "\n39) The data entry dialog is available only after initial loading of the database content at game start.\n"
                                                                 "\n40) Press Quit or hit ALT+Q for exiting the application. Please note that if data saving or loading "
                                                                 "is in progress the exit process will take place only after the load or save operation is complete. "
                                                                 "Please refer to the data entry help menu for more details "
                                                                 "regarding closing the application from data entry dialog.\n"
                                                                 "\nPress Ok to return to previous page.\n"
                                                        };

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

    namespace Colors
    {
        const QString c_BeginPieceTextColor                 {    "#0000ff"                                                                                  };
        const QString c_MiddlePieceTextColor                {    "#000000"                                                                                  };
        const QString c_EndPieceTextColor                   {    "#ff0000"                                                                                  };
    }

    namespace Database
    {
        const QString c_DbDriverName                        {    "QSQLITE"                                                                                  };
        const QString c_DatabaseName                        {    "data.db"                                                                                  };
        const QString c_TableName                           {    "GameDataTable"                                                                            };
        const QString c_IdFieldName                         {    "rowId"                                                                                    };
        const QString c_FirstWordFieldName                  {    "firstWord"                                                                                };
        const QString c_SecondWordFieldName                 {    "secondWord"                                                                               };
        const QString c_AreSynonymsFieldName                {    "areSynonyms"                                                                              };
        const QString c_LanguageFieldName                   {    "language"                                                                                 };
        const QString c_FirstWordFieldPlaceholder           {    ":firstWord"                                                                               };
        const QString c_SecondWordFieldPlaceholder          {    ":secondWord"                                                                              };
        const QString c_AreSynonymsFieldPlaceholder         {    ":areSynonyms"                                                                             };
        const QString c_LanguageFieldPlaceholder            {    ":language"                                                                                };

        const QString c_CreateTableQuery                    {
                                                                 "CREATE TABLE GameDataTable (rowId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
                                                                 "firstWord TEXT, secondWord TEXT, areSynonyms INTEGER, language TEXT)"
                                                            };

        const QString c_RetrieveEntriesFromLanguageQuery    {    "SELECT * FROM GameDataTable WHERE language = '%1'"                                        };
        const QString c_InsertEntryIntoDbQuery              {
                                                                 "INSERT INTO GameDataTable(firstWord, secondWord, areSynonyms, language) "
                                                                 "VALUES(:firstWord, :secondWord, :areSynonyms, 'ANY')"
                                                            };
        const QString c_InsertEntryForLanguageIntoDbQuery   {
                                                                 "INSERT INTO GameDataTable(firstWord, secondWord, areSynonyms, language) "
                                                                 "VALUES(:firstWord, :secondWord, :areSynonyms, :language)"
                                                            };

        const QVector<QString> c_LanguageCodes              {
                                                                 "EN",  // English
                                                                 "DE",  // German
                                                                 "RO",  // Romanian
                                                                 "IT"   // Italian
                                                            };
    }

    namespace Constraints
    {
        static constexpr int c_MinWordSize{5};
        static constexpr int c_MinPairSize{15};
        static constexpr int c_MaxPairSize{30};
    }

    namespace Titles
    {
        const QString c_IntroWindowTitle                    {    "SynAnt - Welcome"                                                                         };
        const QString c_HelpWindowTitle                     {    "SynAnt - %1 Help"                                                                         };
        const QString c_MainWindowTitle                     {    "SynAnt - Let's Play!"                                                                     };
        const QString c_DataEntryWindowTitle                {    "SynAnt - Enter New Word Pairs"                                                            };
        const QString c_PromptSaveExitWindowTitle           {    "SynAnt - Save Data?"                                                                      };
        const QString c_PromptDiscardWindowTitle            {    "SynAnt - Discard Data?"                                                                   };
        const QString c_FatalErrorWindowTitle               {    "Fatal Error!"                                                                             };
    }

    namespace Messages
    {
        // pane messages
        const QString c_GameHelpMessage                     {
                                                                 c_RawGameHelpMessage
                                                                     .arg(Game::Constraints::c_MinWordSize)
                                                                     .arg(Game::Constraints::c_MinPairSize)
                                                                     .arg(Game::Constraints::c_MaxPairSize)
                                                                     .arg(Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY])
                                                                     .arg(Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM])
                                                                     .arg(Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD])
                                                                     .arg(Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_EASY])
                                                                     .arg(Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_MEDIUM])
                                                                     .arg(Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_HARD])
                                                            };

        const QString c_GameHelpPaneToolTip                 {    "Please read the detailed %1 instructions."                                                };
        const QString c_GameHelpButtonToolTip               {    "Click or hit ALT+H for accessing the detailed %1 instructions."                           };



        // status messages
        const QString c_FetchingDataMessage                 {    "\n\n\n\nFetching data.\n\nLanguage change disabled.\n\nPlease wait..."                    };

        const QString c_NoValidEntriesFetchedMessage        {
                                                                 "\n\n\n\nThe selected language has no valid pairs.\n\n"
                                                                 "Please select another language or use the data entry page to add pairs.\n\n"
                                                                 "Then return to intro page and press Play to start the game."
                                                            };

        const QString c_NoValidEntriesSavedForGameLanguage  {
                                                                 "\n\n\n\nNo valid entries saved for current game language.\n\n"
                                                                 "Please select another language or use the data entry page to add pairs.\n\n"
                                                                 "Then return to intro page and press Play to start the game."
                                                            };

        const QString c_CannotChangeLanguageMessage         {    "Cannot change language. No word pairs are available."                                     };

        const QString c_WelcomeMessage                      {
                                                                 "\n\nWelcome to SynAnt!\n\n"
                                                                 "\nIt's simple: two words are divided into equal pieces and mixed with each other.\n"
                                                                 "\nYou just need to guess them.\n"
                                                                 "\nThe words can either be synonyms or antonyms.\n\n"
                                                                 "For more details click Help.\n"
                                                                 "\n\nPlease select a language."
                                                            };

        const QString c_AdditionalDataAvailableMessage      {    "\n\nAdditional data available"                                                            };

        const QString c_PleasePlayOrEnterDataMessage        {
                                                                 "\n\nPress Play to enter the game or Enter new pairs to add more data.\n"
                                                                 "\nUse the dropdown to change the language if required"
                                                                 "\n\nFor more information click Help."
                                                            };

        const QString c_GameStartedMessage                  {    "Game started"                                                                             };
        const QString c_GamePausedMessage                   {    "Game paused"                                                                              };
        const QString c_GameResumedMessage                  {    "Game resumed"                                                                             };
        const QString c_GameStoppedMessage                  {    "Exiting game"                                                                             };
        const QString c_SelectOrDeleteWordPiecesMessage     {    "Please select a new word piece or delete one/more if necessary"                           };

        const QString c_NewDataSaveInProgressMessage        {
                                                                 "\n\nData is currently being saved.\n\n"
                                                                 "Once ready playing will be enabled.\n\n"
                                                                 "Please wait..."
                                                            };

        const QString c_AdditionalDataSaveInProgressMessage {
                                                                 "\n\nAdditional data is currently being saved...\n\n"
                                                                 "Playing is enabled with already existing data."
                                                            };

        const QString c_PieceNotAddedToInputMessage         {    "The selected wordpiece cannot be added to the input of the required word!"                };
        const QString c_PieceAddedToInputMessage            {    "The selected wordpiece has been successfully added to user input"                         };
        const QString c_PiecesRemovedFromInputMessage       {    "One or more wordpieces has been removed from input"                                       };
        const QString c_AllPiecesAddedMessage               {    "All pieces selected. Hit Submit or correct the input if needed"                           };
        const QString c_AllPiecesRemovedMessage             {    "All pieces have been removed from user input"                                             };

        const QString c_CorrectUserInputMessage             {
                                                                 "Congrats! You entered the correct words."
                                                                 "\n\nThe two words are:\n\n"
                                                                 "\t%1\n"
                                                                 "\t%2\n"
                                                                 "\nThe words are: \n\n\t"
                                                                 "%3"
                                                                 "\n\nA new pair of words is available below."
                                                            };

        const QString c_IncorrectUserInputMessage           {    "You didn't enter the correct words.\n\nPlease correct input and try again!"               };

        const QString c_ShowPairRequestedByUserMessage      {
                                                                 "\nThe correct words are:\n\n"
                                                                 "\t%1\n"
                                                                 "\t%2\n"
                                                                 "\nThe words are: \n\n\t"
                                                                 "%3"
                                                                 "\n\nA new pair of words is available below."
                                                            };

        const QString c_StatisticsResetMessage              {    "The statistics have been reset!\n"                                                        };

        const QString c_LevelChangedMessage                 {
                                                                 "Level changed\n\n"
                                                                 "A new pair of words is available below"
                                                            };

        const QString c_LanguageChangedMessage              {    "Language changed\n\n"                                                                     };

        const QString c_TimeLimitEnabledMessage             {    "The time limit has been enabled. Countdown has started!"                                  };
        const QString c_TimeLimitDisabledMessage            {    "The time limit has been disabled!"                                                        };
        const QString c_TimeLimitReachedMessage             {    "The time limit has been reached. A new pair of words has been provided."                   };

        const QString c_CursorModeEnabledMessage            {    "Cursor enabled."                                                                          };
        const QString c_CursorModeDisabledMessage           {    "Cursor disabled."                                                                         };
        const QString c_ClickSelectOrRemoveDisabledMessage  {    "Mouse click selection or removal of word pieces disabled while cursor mode is active!"    };
    }

    namespace Error
    {
        // fatal error messages
        const QString c_FileExceptionRawMessage             {    "\n%1\n\nFile: %2\n"                                                                       };
        const QString c_DatabaseDriverNotAvailable          {    "The SQLite driver is not available!"                                                      };
        const QString c_CannotOpenDatabase                  {    "The database cannot be opened!"                                                           };
        const QString c_CannotCreateTable                   {    "Cannot create database table!"                                                            };
        const QString c_TableIsInvalid                      {    "The database table is invalid!"                                                           };
        const QString c_CannotFetchDataMessage              {    "An error occured when fetching the word pair(s)!\n"                                       };
        const QString c_CannotSaveDataMessage               {    "An error occured when saving the word pair(s)!\n"                                         };
    }

    namespace Misc
    {
        static constexpr int c_RequiredNrOfDbTableFields{5};

        const QString c_GameDescriptor{"Game"};             // keep default capitalized, use toLower() where required
        const QString c_SynonymsDescriptor{"synonyms"};
        const QString c_AntonymsDescriptor{"antonyms"};
    }
}

#endif // GAMEUTILS_H
