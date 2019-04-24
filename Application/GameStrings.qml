pragma Singleton

import QtQuick 2.7

QtObject {
    id: gameStrings

    readonly property string introPaneMessage:                  "\n\t\tWelcome to SynAnt!\n\n
                                                                 \nIt's simple!
                                                                 \nTwo words are divided into equal pieces and mixed with each other.
                                                                 \nYou just need to guess them.
                                                                 \nThe words can either be synonyms or antonyms.\n\n
                                                                 \nPress Play to enter the game or Help for more information."

    readonly property string mainPaneInstructionsMessage:       "\nPlease enter the 2 words.
                                                                 \nHit Submit when ready.
                                                                 \nUse radio buttons to change difficulty level.
                                                                 \nPress Reset to reset the scores and number of pairs."

    readonly property string playButtonLabel:                   "Play"
    readonly property string helpButtonLabel:                   "Help"
    readonly property string quitButtonLabel:                   "Quit"
    readonly property string submitButtonLabel:                 "Submit"
    readonly property string clearInputButtonLabel:             "Clear input"
    readonly property string resultsButtonLabel:                "Show results"
    readonly property string resetButtonLabel:                  "Reset"
    readonly property string okButtonLabel:                     "Ok"
    readonly property string levelEasyButtonLabel:              "Easy"
    readonly property string levelMediumButtonLabel:            "Medium"
    readonly property string levelHardButtonLabel:              "Hard"
    readonly property string closeButtonLabel:                  "Close"

    readonly property string playButtonShortcut:                "ALT+P"
    readonly property string helpButtonShortcut:                "ALT+H"
    readonly property string quitButtonShortcut:                "ALT+Q"
    readonly property string submitButtonShortcut:              "ALT+S"
    readonly property string clearInputButtonShortcut:          "ALT+Z"
    readonly property string clearFirstInputWordShortcut:       "ALT+4"
    readonly property string clearSecondInputWordShortcut:      "ALT+5"
    readonly property string resultsButtonShortcut:             "ALT+B"
    readonly property string resetButtonShortcut:               "ALT+R"
    readonly property string okButtonShortcut:                  "ALT+O"
    readonly property string levelEasyButtonShortcut:           "ALT+1"
    readonly property string levelMediumButtonShortcut:         "ALT+2"
    readonly property string levelHardButtonShortcut:           "ALT+3"
    readonly property string closeButtonShortcut:               "ALT+C"

    readonly property string introPaneToolTip:                  "Welcome to SynAnt"
    readonly property string helpPaneToolTip:                   "Please read the detailed game instructions."
    readonly property string mainPaneToolTip:                   "This is where the action happens!"
    readonly property string errorPaneToolTip:                  "A fatal error occurred. Corrective actions are required."

    readonly property string highscoresToolTip:                 "Displays the game scores:
                                                                \n1) Current score obtained by user
                                                                \n2) Maximum possible score if all word pairs had been guessed."

    readonly property string wordPairsToolTip:                  "Displays the number of word pairs:
                                                                 \n1) Current number of pairs the user has guessed so far
                                                                 \n2) Total number of pairs presented to user so far."
    readonly property string gameInstructionsToolTip:           "Displays the game instructions."
    readonly property string gameStatusToolTip:                 "Displays the game status (e.g. if 2 correct words were entered)."
    readonly property string selectWordPieceToolTip:            "Single click to add the word piece to the first word input. \nDouble click to add the word piece to the second word input."
    readonly property string wordPieceAlreadySelectedToolTip:   "Already added word piece to input!"
    readonly property string firstWordInputToolTip:             "Click to remove the selected chunk of pieces from the first word input."
    readonly property string secondWordInputToolTip:            "Click to remove the selected chunk of pieces from the second word input."
    readonly property string playButtonToolTip:                 "Click or hit ALT+P to play."
    readonly property string helpButtonToolTip:                 "Click or hit ALT+H for accessing the detailed game instructions."
    readonly property string quitButtonToolTip:                 "Click or hit ALT+Q to exit application."
    readonly property string okButtonToolTip:                   "Click or hit ALT+O to return to game."
    readonly property string submitButtonToolTip:               "Click or hit ALT+S after entering the 2 words."
    readonly property string clearInputButtonToolTip:           "Click or hit ALT+Z to clear the user input."
    readonly property string resultsButtonToolTip:              "Click or hit ALT+B to have the correct words displayed."
    readonly property string resetButtonToolTip:                "Click or hit ALT+R for resetting the scores and number of pairs."
    readonly property string levelButtonsToolTip:               "Choose the desired difficulty level.
                                                                 \nClick radio button or hit: \n1) ALT+1 for Easy \n2) ALT+2 for Medium \n3) ALT+3 for Hard"

    readonly property string closeButtonToolTip:                "A fatal error occurred. Click button or hit ALT+C to exit the game."

}
