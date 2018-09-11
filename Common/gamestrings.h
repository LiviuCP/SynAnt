/*

  String literals used by the game window classes etc.

*/

#ifndef GAMESTRINGS_H
#define GAMESTRINGS_H

#include <QString>

namespace GameStrings {

    //file name
    const QString c_FileName                            {    "data"                                                                                     };
    const QString c_NoFile                              {    ""                                                                                         };

    // status and instructions messages
    const QString c_IntroWindowWelcomeMessage           {
                                                             "\n\t\tWelcome to SynAnt!\n\n\n"
                                                             "\nIt's simple!\n"
                                                             "\nTwo words are divided into equal pieces and mixed with each other.\n"
                                                             "\nYou just need to guess them.\n"
                                                             "\nThe words can either be synonyms or antonyms.\n\n\n"
                                                             "\nPress Play to enter the game or Help for more information.\n"
                                                        };

    const QString c_HelpWindowMessage                   {
                                                             "\n\n 1) The words are either in Romanian or English language.\n"
                                                             "\n 2) Minimum word size is 5 characters. \n"
                                                             "\n 3) Each word is divided in equal parts, "
                                                             "except the last part, which might have a smaller number of characters.\n"
                                                             "\n 4) The parts from both words are mixed with each other.\n"
                                                             "\n 5) The first (beginning) part of each word is marked blue.\n"
                                                             "\n 6) The last (end) part of each word is marked red.\n"
                                                             "\n 7) The words can either be synonyms or antonyms.\n"
                                                             "\n 8) Please enter only small letter characters. No numbers, spaces, "
                                                             "capital letters or any other characters are allowed.\n"
                                                             "\n 9) You need to enter both words to win the round.\n"
                                                             "\n10) After entering the 2 words, press Submit for validating input.\n"
                                                             "\n11) If the input is not correct, the user will be asked "
                                                             "to re-enter the words.\n"
                                                             "\n12) If the 2 words have been correctly determined, "
                                                             "the program will display 2 new mixed words for guessing.\n"
                                                             "\n13) 2 new words will also be presented for guessing "
                                                             "if the Show results button is pressed.\n"
                                                             "\n14) The status box is located on the right side of the radio buttons. "
                                                             "It displays if the words have been correctly entered or not.\n"
                                                             "\n15) The status box also displays the right solution "
                                                             "when the Show results button is pressed.\n"
                                                             "\n16) The scores and number of word pairs are displayed in the upper part "
                                                             "of the window.\n"
                                                             "\n17) The format of the score and number of word pairs is: "
                                                             "available/total.\n"
                                                             "\n18) The available score and number of pairs are increased only when "
                                                             "a correct guess is made.\n"
                                                             "\n19) Press the Reset button to set the scores and number of pairs to 0.\n"
                                                             "\n20) Choose the desired difficulty level by using the radio buttons.\n"
                                                             "\n21) The difficulty level determines the size of the parts in which "
                                                             "the words are divided.\n"
                                                             "\n22) It also determines the obtained score for correctly guessing "
                                                             "a pair of words.\n"
                                                             "\n23) The obtained score per pair is: 1 point (easy), 2 points (medium), "
                                                             "4 points (hard).\n"
                                                             "\n24) When changing the difficulty level, the score and number of word "
                                                             "pairs remain unchanged.\n"
                                                             "\n25) However, when the level is changed, a new pair of words "
                                                             "(divided according to the new level) will be presented to the user.\n"
                                                             "\n26) Each button has an appropriate key combination (shortcut).\n"
                                                             "\n27) For all buttons (except Show results) the key combination is "
                                                             "ALT+[first character of the button name].\n"
                                                             "\n28) For example the key combination for Quit is ALT+Q.\n"
                                                             "\n29) For the Show results button the key combination is ALT+B.\n"
                                                             "\n30) For buttons Play, Ok and Submit the alternative key combination is "
                                                             "ENTER.\n"
                                                             "\n31) Hit ALT+1/2/3 for setting the difficulty levels to "
                                                             "easy/medium/hard.\n"
                                                             "\n\n\n\n\t\t\t\tPress Ok to proceed (or return) to game.\n"
                                                        };

    const QString c_InstructionsMessage                 {
                                                             "\nPlease enter the 2 words.\n"
                                                             "\nHit Submit when ready.\n"
                                                             "\nUse radio buttons to change difficulty level.\n"
                                                             "\nPress Reset to reset the scores and number of pairs.\n"
                                                        };

    const QString c_SuccessMessage                      {
                                                             "Congrats! You entered the correct words."
                                                             "\n\nThe two words are:\n\n"
                                                             "\t%1\n"
                                                             "\t%2\n"
                                                             "\nThe words are: \n\n\t"
                                                             "%3"
                                                             "\n\nNext pair of words is available below."
                                                        };

    const QString c_RequestedByUserMessage              {
                                                             "\nThe correct words are:\n\n"
                                                             "\t%1\n"
                                                             "\t%2\n"
                                                             "\nThe words are: \n\n\t"
                                                             "%3"
                                                             "\n\nNext pair of words is available below."
                                                        };

    const QString c_MissingWordsMessage                 {    "At least one of the words hasn't been entered.\n\nPlease try again!"                      };
    const QString c_IncorrectWordsMessage               {    "You didn't enter the correct words.\n\nPlease try again!"                                 };
    const QString c_NoUserInputMessage                  {    "No user input so far."                                                                    };
    const QString c_ScoresResetMessage                  {    "\nScores reset.\n\nLevel unchanged.\n\nNo user input so far.\n"                           };
    const QString c_LevelChangedMessage                 {    "Level changed.\n\nNo user input so far.\n"                                                };
    const QString c_HighscoresMessage                   {    "High-score: %1/%2"                                                                        };
    const QString c_WordPairsMessage                    {    "Word pairs: %1/%2"                                                                        };

    // exception messages
    const QString c_FileNotFoundMessage                 {    "File not found!\n"                                                                        };
    const QString c_CannotOpenFileMessage               {    "File cannot be opened!\n"                                                                 };
    const QString c_EmptyFileMessage                    {    "Empty file!\n"                                                                            };

    const QString c_EmptyRowMessage                     {    "An empty row has been read from file!\n"                                                  };
    const QString c_MultipleSeparatorsMessage           {    "Multiple separators!\n"                                                                   };
    const QString c_NoSeparatorMessage                  {    "No separator found!\n"                                                                    };
    const QString c_IllegalCharactersMessage            {    "%1 word contains illegal characters!\n"                                                   };
    const QString c_LessThanMinCharsMessage             {    "%1 word has less than the minimum required number of characters!\n"                       };

    const QString c_FileNameMessage                     {    "\nFile: %1\n"                                                                             };
    const QString c_RowNumberMessage                    {    "\nRow number: %1\n"                                                                       };

    // button names
    const QString c_PlayButtonLabel                     {    "Play"                                                                                     };
    const QString c_HelpButtonLabel                     {    "Help"                                                                                     };
    const QString c_QuitButtonLabel                     {    "Quit"                                                                                     };
    const QString c_OkButtonLabel                       {    "Ok"                                                                                       };
    const QString c_SubmitButtonLabel                   {    "Submit"                                                                                   };
    const QString c_ResultsButtonLabel                  {    "Show results"                                                                             };
    const QString c_LevelEasyButtonLabel                {    "Easy"                                                                                     };
    const QString c_LevelMediumButtonLabel              {    "Medium"                                                                                   };
    const QString c_LevelHardButtonLabel                {    "Hard"                                                                                     };
    const QString c_ResetButtonLabel                    {    "Reset"                                                                                    };
    const QString c_FatalErrorQuitButtonLabel           {    "Close"                                                                                    };

    // shortcuts
    const QString c_PlayButtonShortcut                  {    "ALT+P"                                                                                    };
    const QString c_HelpButtonShortcut                  {    "ALT+H"                                                                                    };
    const QString c_QuitButtonShortcut                  {    "ALT+Q"                                                                                    };
    const QString c_OkButtonShortcut                    {    "ALT+O"                                                                                    };
    const QString c_SubmitButtonShortcut                {    "ALT+S"                                                                                    };
    const QString c_ResultsButtonShortcut               {    "ALT+B"                                                                                    };
    const QString c_LevelEasyButtonShortcut             {    "ALT+1"                                                                                    };
    const QString c_LevelMediumButtonShortcut           {    "ALT+2"                                                                                    };
    const QString c_LevelHardButtonShortcut             {    "ALT+3"                                                                                    };
    const QString c_ResetShortcut                       {    "ALT+R"                                                                                    };
    const QString c_FatalErrorQuitButtonShortcut        {    "ALT+C"                                                                                    };

    // tooltips
    const QString c_HighscoresToolTip                   {
                                                             "Displays the game scores:\n"
                                                             "\t1) Current score obtained by user\n"
                                                             "\t2) Maximum possible score if all word pairs had been guessed."
                                                        };

    const QString c_WordPairsToolTip                    {
                                                             "Displays the number of word pairs: \n"
                                                             "\t1) Current number of pairs the user has guessed so far\n"
                                                             "\t2) Total number of pairs presented to user so far."
                                                        };

    const QString c_LevelButtonsToolTip                 {
                                                             "Choose the desired difficulty level. \n"
                                                             "Click radio button or hit: \n"
                                                             "\t1) ALT+1 for Easy\n"
                                                             "\t2) ALT+2 for Medium\n"
                                                             "\t3) ALT+3 for Hard\n"
                                                        };

    const QString c_PlayButtonToolTip                   {    "Click or hit ALT+P to play."                                                              };
    const QString c_HelpButtonToolTip                   {    "Click or hit ALT+H for accessing the detailed game instructions."                         };
    const QString c_QuitButtonToolTip                   {    "Click or hit ALT+Q to exit application."                                                  };
    const QString c_IntroWindowToolTip                  {    "Welcome to SynAnt"                                                                        };
    const QString c_OkButtonToolTip                     {    "Click or hit ALT+O to return to game."                                                    };
    const QString c_HelpWindowToolTip                   {    "Please read the detailed game instructions."                                              };
    const QString c_SubmitButtonToolTip                 {    "Click or hit ALT+S after entering the 2 words."                                           };
    const QString c_ResultsButtonToolTip                {    "Click or hit ALT+B to have the correct words displayed."                                  };
    const QString c_ResetButtonToolTip                  {    "Click or hit ALT+R for resetting the scores and number of pairs."                         };
    const QString c_GameInstructionsToolTip             {    "Displays the game instructions."                                                          };
    const QString c_GameStatusToolTip                   {    "Displays the game status (e.g. if 2 correct words were entered)."                         };
    const QString c_FirstWordToolTip                    {    "Enter the first word."                                                                    };
    const QString c_SecondWordToolTip                   {    "Enter the second word."                                                                   };
    const QString c_MainWindowToolTip                   {    "This is where the action happens!"                                                        };
    const QString c_FatalErrorQuitButtonToolTip         {    "Click or hit ALT+C to exit the game."                                                     };
    const QString c_FatalErrorWindowToolTip             {    "A fatal error occurred. Corrective actions are required."                                 };

    // window titles
    const QString c_IntroWindowTitle                    {    "SynAnt - Welcome"                                                                         };
    const QString c_HelpWindowTitle                     {    "SynAnt - Help"                                                                            };
    const QString c_MainWindowTitle                     {    "SynAnt - Let's Play!"                                                                     };
    const QString c_FatalErrorWindowTitle               {    "Fatal Error!"                                                                             };

    // colors
    const QString c_BackgroundColor                     {    "grey"                                                                                     };
    const QString c_BorderColor                         {    "white"                                                                                    };
    const QString c_TextColor                           {    "white"                                                                                    };
    const QString c_WordFirstPieceColor                 {    "blue"                                                                                     };
    const QString c_WordLastPieceColor                  {    "red"                                                                                      };
    const QString c_WordPieceSelectedColor              {    "black"                                                                                    };
    const QString c_PushButtonColor                     {    "darkgrey"                                                                                 };

    // miscellaneous
    const QString c_FirstWordLowerCase                  {    "first"                                                                                    };
    const QString c_FirstWordCamelCase                  {    "First"                                                                                    };
    const QString c_SecondWordLowerCase                 {    "second"                                                                                   };
    const QString c_SecondWordCamelCase                 {    "Second"                                                                                   };
    const QString c_Synonyms                            {    "synonyms"                                                                                 };
    const QString c_Antonyms                            {    "antonyms"                                                                                 };

}

#endif // GAMESTRINGS_H


