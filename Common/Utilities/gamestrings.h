/*

  String literals used by the game window classes etc.

*/

#ifndef GAMESTRINGS_H
#define GAMESTRINGS_H

#include "game.h"

#include <QString>

static const QString c_HelpWindowMessageString          {
                                                             "\n\n 1) The words can be in any language depending on the data file content.\n"
                                                             "\n 2) Minimum word size is %1 characters. \n"
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
                                                             "to the appropriate word in the input area below.\n"
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
                                                             "%2 point/s (easy), %3 points (medium), %4 points (hard).\n"
                                                             "\n28) When changing the difficulty level, the score and number of word "
                                                             "pairs remain unchanged.\n"
                                                             "\n29) However, when the level is changed, a new pair of words "
                                                             "(divided according to the new level) will be presented to the user.\n"
                                                             "\n30) Each button has an appropriate key combination (shortcut).\n"
                                                             "\n31) For all push buttons (except Show results and Clear input) the key combination is "
                                                             "ALT+[first character of the button name]. "
                                                             "For example the key combination for Quit is ALT+Q.\n"
                                                             "\n32) For the Show results button the key combination is ALT+B. "
                                                             "For Clear input the shortcut ALT+Z could be used.\n"
                                                             "\n33) Hit ALT+1/2/3 for setting the difficulty levels to "
                                                             "easy/medium/hard.\n\n"
                                                             "\nPress Ok to return to previous page.\n"
                                                        };

namespace GameStrings {

    //file name
    const QString c_FileName                            {    "data"                                                                                     };
    const QString c_NoFile                              {    ""                                                                                         };

    // general window messages
    const QString c_HelpWindowMessage                   {
                                                             c_HelpWindowMessageString
                                                                 .arg(Game::c_MinWordSize)
                                                                 .arg(Game::c_ScoreIncrements[Game::Level::EASY])
                                                                 .arg(Game::c_ScoreIncrements[Game::Level::MEDIUM])
                                                                 .arg(Game::c_ScoreIncrements[Game::Level::HARD])
                                                        };

    // statistics messages
    const QString c_HighscoresMessage                   {    "High-score: %1/%2"                                                                        };
    const QString c_WordPairsMessage                    {    "Word pairs: %1/%2"                                                                        };

    // status messages
    const QString c_DefaultStatusMessage                {    "Please select a new word piece or delete one/more if necessary"                           };
    const QString c_GameStartedMessage                  {    "Game started"                                                                             };
    const QString c_GameResumedMessage                  {    "Game resumed"                                                                             };

    const QString c_PieceNotAddedMessage                {    "The selected wordpiece cannot be added to the input of the required word!"                };
    const QString c_PieceSuccessfullyAddedMessage       {    "The selected wordpiece has been successfully added to user input"                         };
    const QString c_PiecesRemovedMessage                {    "One or more wordpieces has been removed from input"                                       };
    const QString c_AllPiecesAddedMessage               {    "All pieces selected. Hit Submit or correct the input if needed"                           };
    const QString c_AllPiecesRemovedMessage             {    "All pieces have been removed from user input"                                             };

    const QString c_SuccessMessage                      {
                                                             "Congrats! You entered the correct words."
                                                             "\n\nThe two words are:\n\n"
                                                             "\t%1\n"
                                                             "\t%2\n"
                                                             "\nThe words are: \n\n\t"
                                                             "%3"
                                                             "\n\nA new pair of words is available below."
                                                        };

    const QString c_IncorrectWordsMessage               {    "You didn't enter the correct words.\n\nPlease correct input and try again!"               };

    const QString c_RequestedByUserMessage              {
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

    // exception messages
    const QString c_FileNotFoundMessage                 {    "File not found!\n"                                                                        };
    const QString c_CannotOpenFileMessage               {    "File cannot be opened!\n"                                                                 };
    const QString c_EmptyFileMessage                    {    "Empty file!\n"                                                                            };

    const QString c_EmptyRowMessage                     {    "An empty row has been read from file!\n"                                                  };
    const QString c_MultipleSeparatorsMessage           {    "Multiple separators!\n"                                                                   };
    const QString c_NoSeparatorMessage                  {    "No separator found!\n"                                                                    };
    const QString c_IllegalCharactersMessage            {    "%1 word contains illegal characters!\n"                                                   };
    const QString c_SameWordsInPairMessage              {    "The word pair contains identical words!\n"                                                };
    const QString c_LessThanMinWordCharsMessage         {    "%1 word has less than the minimum required number of characters!\n"                       };
    const QString c_LessThanMinPairCharsMessage         {    "The words pair has less than the minimum required total number of characters!\n"          };
    const QString c_GreaterThanMaxPairCharsMessage      {    "The words pair has more than the maximum allowed total number of characters!\n"           };

    const QString c_FileNameMessage                     {    "\nFile: %1\n"                                                                             };
    const QString c_RowNumberMessage                    {    "\nRow number: %1\n"                                                                       };

    // window titles
    const QString c_IntroWindowTitle                    {    "SynAnt - Welcome"                                                                         };
    const QString c_HelpWindowTitle                     {    "SynAnt - Help"                                                                            };
    const QString c_MainWindowTitle                     {    "SynAnt - Let's Play!"                                                                     };
    const QString c_FatalErrorWindowTitle               {    "Fatal Error!"                                                                             };

    // colors
    const QString c_WordFirstPieceTextColor             {    "blue"                                                                                     };
    const QString c_WordMiddlePieceTextColor            {    "black"                                                                                    };
    const QString c_WordLastPieceTextColor              {    "red"                                                                                      };

    // miscellaneous
    const QString c_FirstWordLowerCase                  {    "first"                                                                                    };
    const QString c_FirstWordCamelCase                  {    "First"                                                                                    };
    const QString c_SecondWordLowerCase                 {    "second"                                                                                   };
    const QString c_SecondWordCamelCase                 {    "Second"                                                                                   };
    const QString c_Synonyms                            {    "synonyms"                                                                                 };
    const QString c_Antonyms                            {    "antonyms"                                                                                 };

}

#endif // GAMESTRINGS_H


