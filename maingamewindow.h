                                                                                                   /*

                                                                                                      This class implements the main game window

                                                                                                      This window can be accessed from 2 possible locations:
                                                                                                      - from the introduction window, by clicking the Play button
                                                                                                      - from the hints (help) window, by clicking the Ok button

                                                                                                      The role of this window is to play the actual game:
                                                                                                      - enter the correct words
                                                                                                      - click the Submit button

                                                                                                      From this window the user can execute following actions:
                                                                                                      1) Jump to the hints (help) window by clicking the Hints button
                                                                                                      2) Play the game as mentioned above
                                                                                                      3) Quit the game by clicking the Quit button

                                                                                                   */

#ifndef MAINGAMEWINDOW_H
#define MAINGAMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVector>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include "hintswindow.h"
#include "wordmixer.h"

class HintsWindow;                                                                                  // forward declaration necessary (MGW contains pointer to HW and viceversa)
class MainGameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainGameWindow(QWidget *parent = 0);
    void updateHintsWinPtr(HintsWindow *hw);                                                        // links the main game window to hints window by assigning the pointer to the HintsWindow object
    void assignWordMixer(WordMixer *wm);                                                            // links the main game window to the word mixer object
    bool windowAlreadyAccessed() const;                                                             // used by the slot of the hints window Ok button to determine if the main game window has already been accessed once by user
    void getFirstTwoWords();                                                                        // called when main game window is accessed for the first time (when clicking Play from introduction window or Ok from HintsWindow, if HW was opened from intro window)

private slots:
    void _onButtonResetClicked();                                                                   // executes when button Reset is clicked
    void _onButtonEasyToggled(bool checked);                                                        // executes when level is changed to "easy" by clicking the designated radio button
    void _onButtonEasyShortcut();                                                                   // executes when the shortcut for the "easy" button (ALT+1) is entered
    void _onButtonMediumToggled(bool checked);                                                      // executes when level is changed to "medium" by clicking the designated radio button
    void _onButtonMediumShortcut();                                                                 // executes when the shortcut for the "medium" button (ALT+2) is entered
    void _onButtonHardToggled(bool checked);                                                        // executes when level is changed to "hard" by clicking the designated radio button
    void _onButtonHardShortcut();                                                                   // executes when the shortcut for the "hard" button (ALT+3) is entered
    void _onButtonSubmitClicked();                                                                  // executes when button Submit is clicked
    void _onButtonHintsClicked();                                                                   // executes when button Hints is clicked
    void _onButtonResultsClicked();                                                                 // executes when button Show results is clicked

private:
    HintsWindow* m_HintsWindow;                                                                     // for opening the hints window when the user presses the Hints button
    QVector<QLabel*> m_MixedWords;                                                                  // labels which contain the text pieces of the 2 words (mixed into a single word)
    QHBoxLayout* m_MixedWordsLayout;                                                                // groups label which contain the text pieces of the word pair
    QLabel* m_ResultsErrors;                                                                        // used for displaying results/errors in the top-right area of the main window
    QLineEdit* m_FirstWord;                                                                         // used for getting the first word entered by user
    QLineEdit* m_SecondWord;                                                                        // used for getting the second word entered by user
    QLabel* m_HighScores;                                                                           // used for storing the current scores (obtained/maximum possible)
    QLabel* m_NrOfWordPairs;                                                                        // used for storing the current number of word pairs (guessed/total number including the ones not guessed)
    QRadioButton* m_LevelEasyButton;                                                                // 3 radio buttons for selecting between difficulty levels Easy, Medium, Hard
    QRadioButton* m_LevelMediumButton;
    QRadioButton* m_LevelHardButton;
    WordMixer* m_WordMixer;                                                                         // for accessing the WordMixer object in order to get 2 mixed words, check user input, retrieve the correct solution when user pushes the Show results button
    bool m_AlreadyAccessed;                                                                         // default false, set true after user enters main game window for the first time
    enum StatisticsUpdate {                                                                         // used for determining how the scores and number of pairs get updated
        FULL_UPDATE,                                                                                // all scores (obtained/total) and number of pairs (guessed/total) get updated (bool value is false)
        PARTIAL_UPDATE                                                                              // only the total score and total number of pairs get updated (bool value is true)
    };

    void _removeMixedWordsLabels();                                                                 // removes labels of the current mixed words from the layout; de-allocates them from memory
    void _createMixedWordsLabels();                                                                 // re-allocates labels for the new mixed words, word pieces written into labels
    void _addMixedWordsLabels();                                                                    // adds the newly allocated labels to their layout
    void _switchToLevel(const int level);                                                           // initiates difficulty level change according to user's request
};

#endif // MAINGAMEWINDOW_H
