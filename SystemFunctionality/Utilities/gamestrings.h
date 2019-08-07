/*
  String literals used by the game window classes etc.
*/

#ifndef GAMESTRINGS_H
#define GAMESTRINGS_H

#include "game.h"

#include <QString>

static const QString c_HelpWindowMessageString          {
                                                             "\n\n 1) The words can be in any language depending on the data file content.\n"
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
                                                             "\n12) You need to build both words correctly to win the round.\n"
                                                             "\n13) Following rules are enforced by app when building the user input:\n"
                                                             " - each word should start with a start piece (blue text)\n"
                                                             " - each word should end with an end piece (red text)\n"
                                                             " - each word contains zero or more middle-pieces (black text) in between\n"
                                                             " - if the first word is closed you cannot close the second one if there "
                                                             "is more than one piece left to add to input\n"
                                                             "\n14) After building the 2 words, press Submit for validating input.\n"
                                                             "\n15) If the input is not correct, the user will be asked "
                                                             "to re-enter the words. Use the chunk delete functionality mentioned above "
                                                             "and then re-enter erased pieces to correct the input. Press Submit again once finished.\n"
                                                             "\n16) If the 2 words have been correctly determined, the program will "
                                                             "confirm the success and display 2 new mixed words for guessing.\n"
                                                             "\n17) 2 new words will also be presented for guessing "
                                                             "if the Show results button is pressed or when the level is changed.\n"
                                                             "\n18) The status box is located on the right side of the radio buttons. "
                                                             "It displays if the words have been correctly entered or not and other status messages.\n"
                                                             "\n19) The status box also displays the right solution "
                                                             "when the Show results button is pressed.\n"
                                                             "\n20) The scores and number of word pairs are displayed in the upper part "
                                                             "of the window.\n"
                                                             "\n21) The format of the score and number of word pairs is: "
                                                             "guessed/total.\n"
                                                             "\n22) The guessed score and number of pairs are increased only when "
                                                             "a correct guess is made. The total score and number of pairs are increased each time "
                                                             "a new pair of words is provided to the user for guessing without having the level changed. "
                                                             "When the level is changed all statistics remain constant. \n"
                                                             "\n23) Press the Reset button to set the statistics (scores and number of pairs) to 0.\n"
                                                             "\n24) Choose the desired difficulty level by using the radio buttons.\n"
                                                             "\n25) The difficulty level determines the size of the parts in which "
                                                             "the words are divided.\n"
                                                             "\n26) It also determines the obtained score for correctly guessing "
                                                             "a pair of words.\n"
                                                             "\n27) The  score obtained for each correctly guessed pair is: "
                                                             "%4 point/s (easy), %5 points (medium), %6 points (hard).\n"
                                                             "\n28) When changing the difficulty level, the score and number of word "
                                                             "pairs remain unchanged.\n"
                                                             "\n29) However, when the level is changed, a new pair of words "
                                                             "(divided according to the new level) will be presented to the user.\n"
                                                             "\n30) Each button has an appropriate key combination (shortcut).\n"
                                                             "\n31) Unless mentioned otherwise for most push buttons the key combination is "
                                                             "ALT+[first character of the button name]. "
                                                             "For example the key combination for Quit is ALT+Q.\n"
                                                             "\n32) For the Show results button the key combination is ALT+B. "
                                                             "For Clear input the shortcut ALT+Z could be used.\n"
                                                             "\n33) Hit ALT+1/2/3 for setting the difficulty levels to "
                                                             "easy/medium/hard.\n"
                                                             "\n34) Use the data entry page to enter additional word pairs. "
                                                             "Press the Enter new pairs button or hit ALT+E to access the page. "
                                                             "Press Add pair or hit ALT+A to validate each new pair. "
                                                             "Press Save or hit CTRL+S to save the validated entries to database. "
                                                             "Press Discard or hit CTRL+D to cancel saving the validated entries to database and clear them. "
                                                             "Press Clear or hit CTRL+Q to erase the content of the text fields. "
                                                             "Press Back or hit ALT+K to return to previous page.\n"
                                                             "\n35) The data entry dialog is available only after initial loading of the database content at game start.\n"
                                                             "\n36) Following rules should be followed when adding new word pairs:\n"
                                                             " - the constraints mentioned at point 2) are enforced \n"
                                                             " - the words should not contain other characters than small letters \n"
                                                             " - the words from each pair should not be identical \n"
                                                             " - no duplicate pairs are allowed even if the order of the two words is different \n"
                                                             " - both words should be entered, otherwise the Add pair button is disabled \n"
                                                             "\n37) Press Quit or hit ALT+Q for exiting the application. Please note that if data saving or loading is in progress "
                                                             "the exit process will take place only after the load or save operation is complete. "
                                                             "During this time the Quit button will be disabled.\n"
                                                             "\n38) When quitting the application from the data entry dialog or pressing the Back button the user will be prompted "
                                                             "to save any validated (but unsaved) entries to database. Three options are available: \n"
                                                             " - Yes (CTRL+Y): save added pairs and execute the requested operation (back/quit)\n"
                                                             " - No (CTRL+N): discard added pairs and execute the requested operation\n"
                                                             " - Cancel(CTRL+W): return to data entry dialog and resume operations\n"
                                                             "\nPress Ok to return to previous page.\n"
                                                        };

namespace GameStrings {

    // database related names
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

    const QString c_CreateTableQuery                    {
                                                             "CREATE TABLE GameDataTable (rowId INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, "
                                                             "firstWord TEXT, secondWord TEXT, areSynonyms INTEGER, language TEXT)"
                                                        };

    const QString c_RetrieveAllEntriesQuery             {    "SELECT * FROM GameDataTable WHERE language = 'EN'"                                        };
    const QString c_InsertEntryIntoDbQuery              {
                                                             "INSERT INTO GameDataTable(firstWord, secondWord, areSynonyms, language) "
                                                             "VALUES(:firstWord, :secondWord, :areSynonyms, 'EN')"
                                                        };

    // general window messages
    const QString c_HelpWindowMessage                   {
                                                             c_HelpWindowMessageString
                                                                 .arg(Game::c_MinWordSize)
                                                                 .arg(Game::c_MinPairSize)
                                                                 .arg(Game::c_MaxPairSize)
                                                                 .arg(Game::c_ScoreIncrements[Game::Level::EASY])
                                                                 .arg(Game::c_ScoreIncrements[Game::Level::MEDIUM])
                                                                 .arg(Game::c_ScoreIncrements[Game::Level::HARD])
                                                        };

    // statistics messages
    const QString c_HighscoresMessage                   {    "High-score: %1/%2"                                                                        };
    const QString c_WordPairsMessage                    {    "Word pairs: %1/%2"                                                                        };

    // status messages
    const QString c_DataLoadingMessage                  {    "Loading data. Please wait"                                                                };

    const QString c_DataAvailableMessage                {
                                                             "\n\t\tWelcome to SynAnt!\n\n\n"
                                                             "\nIt's simple!\n"
                                                             "\nTwo words are divided into equal pieces and mixed with each other.\n"
                                                             "\nYou just need to guess them.\n"
                                                             "\nThe words can either be synonyms or antonyms.\n\n\n"
                                                             "\nPress Play to enter the game or Help for more information."
                                                        };

    const QString c_NoValidEntriesLoadedMessage         {
                                                             "No valid entries could be loaded from database.\n\n"
                                                             "Please use the data entry page to add pairs.\n\n"
                                                             "Then return to intro page and press Play."
                                                        };

    const QString c_SelectOrDeleteWordPiecesMessage     {    "Please select a new word piece or delete one/more if necessary"                           };
    const QString c_DataEntryStartMessage               {    "Word pairs entry started"                                                                 };
    const QString c_DataEntryResumeMessage              {    "Data entry resumed. There are %1 entries pending to be saved!"                            };
    const QString c_DataEntryRequestMessage             {    "Please enter a new pair of words.\n\nChoose if they are synonyms or antonyms"             };
    const QString c_DataSaveInProgressMessage           {    "Data is currently being saved..."                                                         };
    const QString c_DataSuccessfullySavedMessage        {    "%1 word pairs have been successfully saved to database"                                   };
    const QString c_GameStartedMessage                  {    "Game started"                                                                             };
    const QString c_GamePausedMessage                   {    "Game paused"                                                                              };
    const QString c_GameResumedMessage                  {    "Game resumed"                                                                             };
    const QString c_GameStoppedMessage                  {    "Exiting game"                                                                             };

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

    const QString c_DataEntrySuccessMessage             {
                                                             "The new pair has been successfully added to validated pairs.\n\n"
                                                             "There are %1 entries pending to be saved.\n\n"
                                                             "Press Save to add the already validated pairs to game database "
                                                             "and make the available for playing."
                                                        };

    const QString c_PairAlreadyAddedMessage             {    "The pair has already been added. Please save added pairs to database or discard them"     };

    const QString c_DataEntryInvalidPairMessage         {
                                                             "The new words pair has not been correctly entered.\n\n"
                                                             "%1\n\n"
                                                             "Please correct and try again!"
                                                        };

    const QString c_DataEntryCacheResetRequestedMessage {    "Discarding unsaved entries"                                                               };
    const QString c_DataEntryCacheResetMessage          {    "The unsaved entries been discarded"                                                       };

    const QString c_ShowPairRequestedByUserMessage      {
                                                             "\nThe correct words are:\n\n"
                                                             "\t%1\n"
                                                             "\t%2\n"
                                                             "\nThe words are: \n\n\t"
                                                             "%3"
                                                             "\n\nA new pair of words is available below."
                                                        };

    const QString c_ScoresResetMessage                  {    "The statistics have been reset!\n"                                                        };

    const QString c_LevelChangedMessage                 {
                                                             "Level changed\n\n"
                                                             "A new pair of words is available below"
                                                        };

    // invalid pair entry error messages
    const QString c_WordHasLessThanMinimumCharacters    {    "At least one word has less than minimum required number of characters."                   };
    const QString c_PairHasLessThanMinimumCharacters    {    "The entered word pair has less than the minimum required number of characters."           };
    const QString c_PairHasMoreThanMaximumCharacters    {    "The entered word pair has more than the allowed maximum number of characters."            };
    const QString c_InvalidCharacters                   {    "At least one of the words contains invalid characters."                                   };
    const QString c_PairHasIdenticalWords               {    "The entered word pair has identical words."                                               };
    const QString c_PairAlreadyExists                   {    "The entered word pair already exists."                                                    };

    // fatal error messages
    const QString c_DatabaseDriverNotAvailable          {    "The SQLite driver is not available!"                                                      };
    const QString c_CannotOpenDatabase                  {    "The database cannot be opened!"                                                           };
    const QString c_CannotCreateTable                   {    "Cannot create database table!"                                                            };
    const QString c_TableIsInvalid                      {    "The database table is invalid!"                                                           };
    const QString c_CannotCreateFileMessage             {    "File not found. Attempt to create it failed!"                                             };
    const QString c_CannotOpenFileMessage               {    "File cannot be opened for reading!\n"                                                     };
    const QString c_CannotSaveDataMessage               {    "An error occured when saving the word pair(s)!"                                           };

    // window titles
    const QString c_IntroWindowTitle                    {    "SynAnt - Welcome"                                                                         };
    const QString c_HelpWindowTitle                     {    "SynAnt - Help"                                                                            };
    const QString c_MainWindowTitle                     {    "SynAnt - Let's Play!"                                                                     };
    const QString c_DataEntryWindowTitle                {    "SynAnt - Enter New Word Pairs"                                                            };
    const QString c_PromptSaveExitWindowTitle           {    "SynAnt - Save Data?"                                                                      };
    const QString c_FatalErrorWindowTitle               {    "Fatal Error!"                                                                             };

    // colors
    const QString c_WordFirstPieceTextColor             {    "blue"                                                                                     };
    const QString c_WordMiddlePieceTextColor            {    "black"                                                                                    };
    const QString c_WordLastPieceTextColor              {    "red"                                                                                      };

    // miscellaneous
    const QString c_FileNameMessage                     {    "\nFile: %1\n"                                                                             };
    const QString c_Synonyms                            {    "synonyms"                                                                                 };
    const QString c_Antonyms                            {    "antonyms"                                                                                 };

    static constexpr char c_SynonymsSeparator{'='};
    static constexpr char c_AntonymsSeparator{'!'};

}

#endif // GAMESTRINGS_H


