pragma Singleton

import QtQuick 2.7

QtObject {
    id: gameStrings

    // titles, messages etc.
    readonly property string fatalErrorWindowTitle:                                 "Fatal Error!"
    readonly property string fatalErrorWindowObjectName:                            "errorDialog"

    readonly property string promptSaveBeforeExitMessage:                           "\n\n\n\n\tDo you want to save the added word pairs before exiting?\n"
    readonly property string promptDiscardMessage:                                  "\n\n\n\n\tDo you really want to discard the added word pairs?\n"

    // labels
    readonly property string playButtonLabel:                                       "Play"
    readonly property string helpButtonLabel:                                       "Help"
    readonly property string quitButtonLabel:                                       "Quit"
    readonly property string submitButtonLabel:                                     "Submit"
    readonly property string clearMainPaneInputButtonLabel:                         "Clear input"
    readonly property string showPairButtonLabel:                                   "Show pair"
    readonly property string mainPaneStatisticsResetButtonLabel:                    "Reset"
    readonly property string okButtonLabel:                                         "Ok"
    readonly property string levelEasyButtonLabel:                                  "Easy"
    readonly property string levelMediumButtonLabel:                                "Medium"
    readonly property string levelHardButtonLabel:                                  "Hard"
    readonly property string dataEntryButtonLabel:                                  "Enter new pairs"
    readonly property string dataEntryClearButtonLabel:                             "Clear"
    readonly property string synonymsSelectionButtonLabel:                          "Synonyms"
    readonly property string antonymsSelectionButtonLabel:                          "Antonyms"
    readonly property string addPairButtonLabel:                                    "Add pair"
    readonly property string backButtonLabel:                                       "Back"
    readonly property string discardButtonLabel:                                    "Discard"
    readonly property string saveButtonLabel:                                       "Save"
    readonly property string promptSaveYesButtonLabel:                              "Yes"
    readonly property string promptSaveNoButtonLabel:                               "No"
    readonly property string promptSaveCancelButtonLabel:                           "Cancel"
    readonly property string promptDiscardYesButtonLabel:                           "Yes"
    readonly property string promptDiscardCancelButtonLabel:                        "Cancel"
    readonly property string closeButtonLabel:                                      "Close"

    // shortcuts
    readonly property string playButtonShortcut:                                    "ALT+P"
    readonly property string helpButtonShortcut:                                    "ALT+H"
    readonly property string quitButtonShortcut:                                    "ALT+Q"
    readonly property string submitButtonShortcut:                                  "ALT+S"
    readonly property string clearMainPaneInputButtonShortcut:                      "ALT+Z"
    readonly property string clearMainPaneFirstInputWordShortcut:                   "ALT+4"
    readonly property string clearMainPaneSecondInputWordShortcut:                  "ALT+5"
    readonly property string showPairButtonShortcut:                                "ALT+B"
    readonly property string mainPaneResetButtonShortcut:                           "ALT+R"
    readonly property string okButtonShortcut:                                      "ALT+O"
    readonly property string levelEasyButtonShortcut:                               "ALT+1"
    readonly property string levelMediumButtonShortcut:                             "ALT+2"
    readonly property string levelHardButtonShortcut:                               "ALT+3"
    readonly property string dataEntryButtonShortcut:                               "ALT+E"
    readonly property string dataEntryClearButtonShortcut:                          "ALT+X"
    readonly property string addPairButtonShortcut:                                 "ALT+A"
    readonly property string backButtonShortcut:                                    "ALT+K"
    readonly property string saveButtonShortcut:                                    "CTRL+S"
    readonly property string discardButtonShortcut:                                 "CTRL+D"
    readonly property string promptSaveYesButtonShortcut:                           "ALT+Y"
    readonly property string promptSaveNoButtonShortcut:                            "ALT+N"
    readonly property string promptSaveCancelButtonShortcut:                        "ALT+W"
    readonly property string promptDiscardYesButtonShortcut:                        "CTRL+Y"
    readonly property string promptDiscardCancelButtonShortcut:                     "CTRL+W"
    readonly property string closeButtonShortcut:                                   "ALT+C"

    // tooltips
    readonly property string introPaneToolTip:                                      "Welcome to SynAnt"
    readonly property string mainPaneToolTip:                                       "This is where the action happens!"
    readonly property string dataEntryPaneToolTip:                                  "Please feel free to add new pairs to the game."
    readonly property string promptPaneToolTip:                                     "Please choose one of the below options."
    readonly property string errorPaneToolTip:                                      "A fatal error occurred. Corrective actions are required."

    readonly property string highscoresToolTip:                                     "Displays the game scores:
                                                                                    \n1) Current score obtained by user
                                                                                    \n2) Maximum possible score if all word pairs had been guessed."

    readonly property string wordPairsToolTip:                                      "Displays the number of word pairs:
                                                                                    \n1) Current number of pairs the user has guessed so far
                                                                                    \n2) Total number of pairs presented to user so far."
    readonly property string mainPaneStatusBoxToolTip:                              "Displays the game status (e.g. if 2 correct words were entered)."
    readonly property string selectWordPieceToolTip:                                "Single click to add the word piece to the first word input.
                                                                                    \nDouble click to add the word piece to the second word input."
    readonly property string wordPieceAlreadySelectedToolTip:                       "Already added word piece to input!"
    readonly property string mainPaneFirstWordInputToolTip:                         "Click to remove the selected chunk of pieces from the first word input."
    readonly property string mainPaneSecondWordInputToolTip:                        "Click to remove the selected chunk of pieces from the second word input."
    readonly property string playButtonToolTip:                                     "Click or hit ALT+P to play."
    readonly property string quitButtonToolTip:                                     "Click or hit ALT+Q to exit application.
                                                                                    \nThe application will exit only after completing all load/save operations."
    readonly property string okButtonToolTip:                                       "Click or hit ALT+O to return to previous page."
    readonly property string submitButtonToolTip:                                   "Click or hit ALT+S after entering the 2 words."
    readonly property string clearMainPaneInputButtonToolTip:                       "Click or hit ALT+Z to clear the user input."
    readonly property string showPairButtonToolTip:                                 "Click or hit ALT+B to have the correct words displayed."
    readonly property string mainPaneResetButtonToolTip:                            "Click or hit ALT+R for resetting the scores and number of pairs."
    readonly property string levelButtonsToolTip:                                   "Choose the desired difficulty level.
                                                                                    \nClick radio button or hit: \n1) ALT+1 for Easy \n2) ALT+2 for Medium \n3) ALT+3 for Hard"
    readonly property string dataEntryButtonToolTip:                                "Click or hit ALT+E to go to the new word pairs entry page"
    readonly property string dataEntryClearButtonToolTip:                           "Click or hit ALT+X to clear the data entry text fields."
    readonly property string addPairButtonToolTip:                                  "Click or hit ALT+A to add the new word pair"
    readonly property string backButtonToolTip:                                     "Click or hit ALT+K to go to previous page"
    readonly property string saveButtonToolTip:                                     "Click or hit CTRL+S to save the added entries to database"
    readonly property string discardButtonToolTip:                                  "Click or hit CTRL+D to discard the added entries"
    readonly property string promptSaveYesButtonToolTip:                            "Click or hit ALT+Y to save and exit the application"
    readonly property string promptSaveNoButtonToolTip:                             "Click or hit ALT+N to exit the application without saving"
    readonly property string promptSaveCancelButtonToolTip:                         "Click or hit ALT+W to return to data entry dialog"
    readonly property string promptDiscardYesButtonToolTip:                         "Click or hit CTRL+Y to discard the added entries"
    readonly property string promptDiscardCancelButtonToolTip:                      "Click or hit CTRL+W to cancel discarding the added entries"
    readonly property string closeButtonToolTip:                                    "A fatal error occurred. Click button or hit ALT+C to exit the game."
    readonly property string dataEntryFirstWordToolTip:                             "Enter the first word from the pair to be added to database."
    readonly property string dataEntrySecondWordToolTip:                            "Enter the second word from the pair to be added to database."
}
