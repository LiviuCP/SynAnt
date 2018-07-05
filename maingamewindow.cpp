#include <QHBoxLayout>
#include <QApplication>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QShortcut>
#include "maingamewindow.h"
#include "gamestrings.h"

MainGameWindow::MainGameWindow(QWidget *parent)
    : QWidget{parent},
      m_HintsWindow{nullptr},                                                                         // will be assigned a new value when connecting the main game window to hints window by calling updateHintsWindowPtr()
      m_MixedWords{},                                                                                 // initially empty, will be updated when the mixed words labels are created by calling getFirstTwoWords()
      m_WordMixer{nullptr},                                                                           // will be assigned a new value when connecting the main game window to the word mixer object by calling assignWordMixer()
      m_ScoreItem{new ScoreItem{this}},                                                               // prepare recording of statistics
      m_AlreadyAccessed{false}                                                                        // will be set true when getFirstTwoWords is called (user opens the main game window for the first time)
{
    QHBoxLayout *statisticsLayout{new QHBoxLayout{}};                                               // 1) create the top layout, which contains the score and number of word pairs labels and the button for reseting their content
    m_HighScores = new QLabel{};
    m_HighScores -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_HighScores -> setToolTip(GameStrings::c_HighscoresToolTip);
    m_NrOfWordPairs = new QLabel{};
    m_NrOfWordPairs -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_NrOfWordPairs -> setToolTip(GameStrings::c_WordPairsToolTip);
    QPushButton *resetButton{new QPushButton{GameStrings::c_ResetButtonLabel}};
    resetButton -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    resetButton -> setToolTip(GameStrings::c_ResetButtonToolTip);
    QShortcut *resetButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_ResetShortcut},this}}; // Reset button key combination (shortcut)
    statisticsLayout -> addWidget(m_HighScores);
    statisticsLayout -> addWidget(m_NrOfWordPairs);
    statisticsLayout -> addSpacing(30);
    statisticsLayout -> addWidget(resetButton);
                                                                                                    /* 2) create the next layout of the window, which contains:
                                                                                                       - the box with the message requesting input
                                                                                                       - the groupbox with the buttons for game level selection
                                                                                                       - the status (results/errors) message box
                                                                                                    */
    QHBoxLayout *levelsStatusReqInputLayout{new QHBoxLayout{}};
    QLabel *requestInput{new QLabel{}};
    requestInput -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    requestInput -> setToolTip(GameStrings::c_GameInstructionsToolTip);
    QString requestInputText{GameStrings::c_InstructionsMessage};
    requestInput -> setText(requestInputText);
    QGroupBox *levelButtonsBox{new QGroupBox{}};
    levelButtonsBox -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    levelButtonsBox -> setToolTip(GameStrings::c_LevelButtonsToolTip);
    m_LevelEasyButton = new QRadioButton{GameStrings::c_LevelEasyButtonLabel};
    QShortcut *levelEasyShortcut{new QShortcut{QKeySequence{GameStrings::c_LevelEasyButtonShortcut},this}};     // shortcut ALT+1 for accessing level Easy button from keyboard
    m_LevelMediumButton = new QRadioButton{GameStrings::c_LevelMediumButtonLabel};
    QShortcut *levelMediumShortcut{new QShortcut{QKeySequence{GameStrings::c_LevelMediumButtonShortcut},this}}; // shortcut ALT+2 for accessing level Medium button from keyboard
    m_LevelMediumButton -> setChecked(true);                                                        // medium level (2 characters per word piece) is the default game level
    m_LevelHardButton = new QRadioButton{GameStrings::c_LevelHardButtonLabel};
    QShortcut *levelHardShortcut{new QShortcut{QKeySequence{GameStrings::c_LevelHardButtonShortcut},this}};     // shortcut ALT+3 for accessing the level Hard button from keyboard
    QVBoxLayout *levelButtonsLayout{new QVBoxLayout};
    levelButtonsLayout -> addWidget(m_LevelEasyButton);
    levelButtonsLayout -> addWidget(m_LevelMediumButton);
    levelButtonsLayout -> addWidget(m_LevelHardButton);
    levelButtonsBox -> setLayout(levelButtonsLayout);
    m_ResultsErrors = new QLabel{};
    m_ResultsErrors -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_ResultsErrors -> setToolTip(GameStrings::c_GameStatusToolTip);
    levelsStatusReqInputLayout -> addWidget(requestInput);
    levelsStatusReqInputLayout -> addWidget(levelButtonsBox);
    levelsStatusReqInputLayout -> addWidget(m_ResultsErrors);
                                                                                                    // 3) create the row with the labels containing pieces of the 2 words (mixed into one word)
    m_MixedWordsLayout = new QHBoxLayout{};
    m_MixedWordsLayout -> setSpacing(0);
                                                                                                    // 4) create the row with the 2 boxes used for entering the requested words
    QHBoxLayout *wordsLayout{new QHBoxLayout{}};
    m_FirstWord = new QLineEdit{};
    m_FirstWord -> setToolTip(GameStrings::c_FirstWordToolTip);
    m_SecondWord = new QLineEdit{};
    m_SecondWord -> setToolTip(GameStrings::c_SecondWordToolTip);
    wordsLayout -> addWidget(m_FirstWord);
    wordsLayout -> addWidget(m_SecondWord);
                                                                                                    // 5) create the row with the buttons
    QHBoxLayout *buttonsLayout{new QHBoxLayout{}};
    buttonsLayout -> setSpacing(25);
    QPushButton *submitButton{new QPushButton{GameStrings::c_SubmitButtonLabel}};                    // button used by user for acknowledging the input
    submitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    submitButton -> setToolTip(GameStrings::c_SubmitButtonToolTip);
    QShortcut *submitButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_SubmitButtonShortcut},this}}; // Submit button primary key combination (shortcut)
    QShortcut *submitButtonAltShortcut{new QShortcut{QKeySequence{GameStrings::c_SubmitButtonAltShortcut},this}};  // Submit button alternative key combination (shortcut)
    QPushButton *hintsButton{new QPushButton{GameStrings::c_HelpButtonLabel}};                       // button used for accessing the game instructions (hints window opens)
    hintsButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    hintsButton -> setToolTip(GameStrings::c_HelpButtonToolTip);
    QShortcut *hintsButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_HelpButtonShortcut},this}}; // Hints button key combination (shortcut)
    QPushButton *showResultsButton{new QPushButton{GameStrings::c_ResultsButtonLabel}};              // button used for asking the program to show the correct results
    showResultsButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    showResultsButton -> setToolTip(GameStrings::c_ResultsButtonToolTip);
    QShortcut *showResultsButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_ResultsButtonShortcut},this}}; // Show results button key combination (shortcut)
    QPushButton *quitButton{new QPushButton{GameStrings::c_QuitButtonLabel}};                        // button used for quitting the application (window closes)
    quitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    quitButton -> setToolTip(GameStrings::c_QuitButtonToolTip);
    QShortcut *quitButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_QuitButtonShortcut},this}}; // Quit button key combination (shortcut)
    buttonsLayout -> addWidget(submitButton);
    buttonsLayout -> addWidget(hintsButton);
    buttonsLayout -> addWidget(showResultsButton);
    buttonsLayout -> addWidget(quitButton);
                                                                                                    // 6) add everything to the main layout and the main layout to the window
    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(statisticsLayout,1);
    mainLayout -> addLayout(levelsStatusReqInputLayout);
    mainLayout -> addLayout(m_MixedWordsLayout);
    mainLayout -> addLayout(wordsLayout);
    mainLayout -> addLayout(buttonsLayout);
    setLayout(mainLayout);
    setToolTip(GameStrings::c_MainWindowToolTip);
    setAttribute(Qt::WA_AlwaysShowToolTips);                                                        /* ensure tooltips are shown when the window is hidden and then shown again
                                                                                                     without transitioning to another window (e.g. by slotButtonSubmitClicked() )
                                                                                                    */

                                                                                                    // 7) connect the button signals to their slots
    connect(m_ScoreItem,&ScoreItem::statisticsUpdated,this,&MainGameWindow::onStatisticsUpdated);
    connect(this,&MainGameWindow::levelChanged,m_ScoreItem, &ScoreItem::setScoreIncrement);
    connect(resetButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonResetClicked);
    connect(resetButtonShortcut,&QShortcut::activated,this,
                                                       &MainGameWindow::_onButtonResetClicked);
    connect(m_LevelEasyButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonEasyToggled);
    connect(levelEasyShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonEasyShortcut);
    connect(m_LevelMediumButton,&QRadioButton::toggled,this,
                                                        &MainGameWindow::_onButtonMediumToggled);
    connect(levelMediumShortcut,&QShortcut::activated,this,
                                                       &MainGameWindow::_onButtonMediumShortcut);
    connect(m_LevelHardButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonHardToggled);
    connect(levelHardShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonHardShortcut);
    connect(submitButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonSubmitClicked);
    connect(submitButtonShortcut,&QShortcut::activated,this,
                                                        &MainGameWindow::_onButtonSubmitClicked);
    connect(submitButtonAltShortcut,&QShortcut::activated,this,
                                                    &MainGameWindow::_onButtonSubmitClicked);
    connect(hintsButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonHintsClicked);
    connect(hintsButtonShortcut,&QShortcut::activated,this,
                                                        &MainGameWindow::_onButtonHintsClicked);
    connect(showResultsButton,&QPushButton::clicked,this,
                                                    &MainGameWindow::_onButtonResultsClicked);
    connect(showResultsButtonShortcut,&QShortcut::activated,this,
                                                        &MainGameWindow::_onButtonResultsClicked);
    connect(quitButton,&QPushButton::clicked,qApp,&QApplication::quit);
    connect(quitButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
}
                                                                                                    // this function links the main game window to hints window by assigning the pointer to the HintsWindow object
void MainGameWindow::updateHintsWinPtr(HintsWindow *hw)
{
    m_HintsWindow = hw;
}
                                                                                                    // this function links the main game window to the word mixer object
void MainGameWindow::assignWordMixer(WordMixer *wm)
{
    m_WordMixer = wm;
    if (m_WordMixer) {
        connect(this,&MainGameWindow::levelChanged,m_WordMixer, &WordMixer::setWordPieceSize);
    }
}
                                                                                                    // this function is used by the slot of the hints window Ok button to determine if the main game window has already been accessed once by user
bool MainGameWindow::windowAlreadyAccessed() const
{
    return m_AlreadyAccessed;
}
                                                                                                    /* this function requests the first pair of words from the WordMixer object; the function is called in 2 scenarios:
                                                                                                       Scenario 1: from introduction window, button Play is clicked
                                                                                                       Scenario 2: from introducton window user clicks the Hints button; then he pushes Ok in the hints window
                                                                                                    */
void MainGameWindow::getFirstTwoWords()
{
    m_AlreadyAccessed = true;                                                                         // set variable true to signal that the main game window has already been accessed once and the first 2 words were sent to the user
    m_WordMixer -> mixWords();                                                                        // Step 1: a pair of words is fetched from file and mixed (this is solely the task of the class WordMixer)
    _createMixedWordsLabels();                                                                       // Step 2: the main game window takes over and creates the labels for the mixed words pieces
    _addMixedWordsLabels();                                                                          // Step 3: the created label array is added to the layout (mixedWordsLayout)
    m_ScoreItem -> resetStatistics();
    m_ResultsErrors -> setText(m_WordMixer -> getStatusMessage());                                      // Step 4: update the box with results/errors mentioning that the user hasn't submitted any input yet
}

void MainGameWindow::onStatisticsUpdated()
{
    QVector<int> scoresPairs{m_ScoreItem -> getStatistics()};
    m_HighScores -> setText("High-score: " + QString::number(scoresPairs[0]) + "/" + QString::number(scoresPairs[1]));
    m_NrOfWordPairs -> setText("Word pairs: " + QString::number(scoresPairs[2]) + "/" + QString::number(scoresPairs[3]));
}
                                                                                                    // slot handles the button Reset click
void MainGameWindow::_onButtonResetClicked()
{
    hide();
    m_ScoreItem -> resetStatistics();                                                                 // all scores (obtained/total available) and number of pairs (guessed/total presented to user) are reset to 0
    m_ResultsErrors -> setText(m_WordMixer -> getStatusMessage());                                      // results/errors box updated with reset message
    show();
}

                                                                                                    // slot handles changing the level to Easy when the designated radio button is checked (previously unchecked)
void MainGameWindow::_onButtonEasyToggled(bool checked)
{
    if (checked)
    {
        _switchToLevel(Game::Level::EASY);
    }
}
                                                                                                    // slot handles changing the level to Easy when shortcut ALT+1 is entered
void MainGameWindow::_onButtonEasyShortcut()
{
    if (!(m_LevelEasyButton -> isChecked()))                                                          // if current level is NOT Easy, check the radio button
    {
        hide();
        m_LevelEasyButton -> setChecked(true);                                                        // this triggers the toggled() signal and the button slot executes as if the button was clicked
        show();
    }
}
                                                                                                    // slot handles changing the level to Medium when the designated radio button is checked (previously unchecked)
void MainGameWindow::_onButtonMediumToggled(bool checked)
{
    if (checked)
    {
        _switchToLevel(Game::Level::MEDIUM);
    }
}
                                                                                                    // slot handles changing the level to Medium when shortcut ALT+2 is entered
void MainGameWindow::_onButtonMediumShortcut()
{
    if (!(m_LevelMediumButton -> isChecked()))                                                        // if current level is NOT Medium, check the radio button
    {
        hide();
        m_LevelMediumButton -> setChecked(true);                                                      // this triggers the toggled() signal and the button slot executes as if the button was clicked
        show();
    }
}
                                                                                                    // slot handles changing the level to Hard when the designated radio button is checked (previously unchecked)
void MainGameWindow::_onButtonHardToggled(bool checked)
{
    if (checked)
    {
        _switchToLevel(Game::Level::HARD);
    }
}
                                                                                                    // slot handles changing the level to Hard when shortcut ALT+3 is entered
void MainGameWindow::_onButtonHardShortcut()
{
    if (!(m_LevelHardButton -> isChecked()))                                                          // if current level is NOT Hard, check the radio button
    {
        hide();
        m_LevelHardButton -> setChecked(true);                                                        // this triggers the toggled() signal and the button slot executes as if the button was clicked
        show();
    }
}
                                                                                                    // slot handles the button Submit click
void MainGameWindow::_onButtonSubmitClicked()
{
                                                                                                    // step 1: hide main game window, clear the input buffers, set first input box as active
    hide();
    QString firstInputWord{m_FirstWord -> text()};                                                    // stores the first word entered by user
    QString secondInputword{m_SecondWord -> text()};                                                  // stores the second word entered by user
    m_FirstWord -> clear();                                                                           // any user input is erased as new words will be presented for guessing
    m_SecondWord -> clear();
    m_FirstWord -> setFocus();
                                                                                                    /* step 2: check if the user entered the correct words
                                                                                                       - if the user input is correct, a success message is created and 2 new words are read and mixed
                                                                                                       - otherwise a error message is created and the mixed words remain unchanged (user should re-enter the words)
                                                                                                    */
    if (m_WordMixer -> checkWords(firstInputWord,secondInputword))
    {
        m_WordMixer -> mixWords();                                                                    // then request a new pair of words
        _removeMixedWordsLabels();                                                                   // update mixed words labels with the new words pair
        _createMixedWordsLabels();
        _addMixedWordsLabels();
        m_ScoreItem -> updateStatistics(FULL_UPDATE);
    }
    m_ResultsErrors -> setText(m_WordMixer -> getStatusMessage());                                      // step 3: update errors/results section with the status message (can be a success or error message depending on previous check)
    show();                                                                                         // step 4: show the updated main game window
}
                                                                                                    // slot handles the button Hints click
void MainGameWindow::_onButtonHintsClicked()
{
    hide();
    m_HintsWindow -> show();
}
                                                                                                    // slot handles the button Show results click
void MainGameWindow::_onButtonResultsClicked()
{
    hide();                                                                                         // step 1: hide main game window (will be shown again once updated)
    m_FirstWord -> clear();                                                                           // any user input is erased as new words will be presented for guessing
    m_SecondWord -> clear();
    m_FirstWord -> setFocus();
    m_WordMixer -> retrieveResults();                                                                 // step 2: results message created, 2 new words are read and mixed
    m_ScoreItem -> updateStatistics(PARTIAL_UPDATE);                                                  // step 3: update the total score and the total number of pairs only (user gets no points and the guessed number of pairs stays unchanged)
    m_WordMixer -> mixWords();                                                                        // step 4: fetch a new pair of mixed words
    _removeMixedWordsLabels();                                                                       // step 5: update mixed words label array (remove old labels, create the ones for the new words, add them to layout
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    m_ResultsErrors -> setText(m_WordMixer -> getStatusMessage());                                      // step 7: update the results/errors section with the created results message
    show();                                                                                         // step 8: show the updated main game window
}

                                                                                                    // removes the labels containing the pieces of the 2 mixed words from the mixed words layout; de-allocates them from memory
void MainGameWindow::_removeMixedWordsLabels()
{
    for(auto currentWordPiece : m_MixedWords)
    {
        m_MixedWordsLayout -> removeWidget(currentWordPiece);
        delete currentWordPiece;
        currentWordPiece = nullptr;
    }
}
                                                                                                    // this function allocates new labels for the pieces of the new mixed words pair; writes each word piece in its label
void MainGameWindow::_createMixedWordsLabels()
{
    int newNumberOfPieces{(m_WordMixer->getMixedWordsStringArray()).size()};                          // total number of pieces of the "new" words pair (their labels to be created)
    m_MixedWords.resize(newNumberOfPieces);
    for (int wordPieceIndex{0}; wordPieceIndex < newNumberOfPieces; wordPieceIndex++)               // used "classic" for (without auto currentWordPiece) as the function is looping through 2 vectors (the string vector and the label pointer vector)
    {
        m_MixedWords[wordPieceIndex] = new QLabel{};
        m_MixedWords[wordPieceIndex] ->                                                               // retrieve the string from each mixed words array position
                 setText((m_WordMixer -> getMixedWordsStringArray()).at(wordPieceIndex));             // and write it into the newly created label (with the same position index)
        m_MixedWords[wordPieceIndex] -> setStyleSheet(GameStrings::c_WordEachPieceStyle);
    }
                                                                                                    // for the first piece of each word set text color green
    m_MixedWords[m_WordMixer -> getFirstWordBeginIndex()]
                                        -> setStyleSheet(GameStrings::c_WordBeginPieceStyle);
    m_MixedWords[m_WordMixer -> getSecondWordBeginIndex()]
                                        -> setStyleSheet(GameStrings::c_WordBeginPieceStyle);
                                                                                                    // for the last piece of each word set text color red
    m_MixedWords[m_WordMixer -> getFirstWordEndIndex()]
                                        -> setStyleSheet(GameStrings::c_WordEndPieceStyle);
    m_MixedWords[m_WordMixer -> getSecondWordEndIndex()]
                                        -> setStyleSheet(GameStrings::c_WordEndPieceStyle);
}
                                                                                                    // this function adds the newly created labels to the mixed words layout
void MainGameWindow::_addMixedWordsLabels()
{
    for (auto currentWordPiece : m_MixedWords)
    {
        m_MixedWordsLayout -> addWidget(currentWordPiece);
    }
}
                                                                                                    /* this function initiates changing the difficulty level according to user's request
                                                                                                       when the level is changed no modification is done to the statistics (score, number of word pairs)
                                                                                                       this means the current pair of words at the time when user chooses another level
                                                                                                       is not taken into account
                                                                                                    */
void MainGameWindow::_switchToLevel(Game::Level level)
{
    hide();
    m_FirstWord -> clear();                                                                           // any user input is erased as new words will be presented for guessing
    m_SecondWord -> clear();
    m_FirstWord -> setFocus();
    Q_EMIT levelChanged(level);                                                                       // Step 1: change the level parameters to the values coresponding to the chosen new level
    m_WordMixer -> mixWords();                                                                        // Step 2: get a new pair of words
    _removeMixedWordsLabels();                                                                       // Step 3: update the labels of the mixed words pair in the main game window
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    m_ResultsErrors -> setText(m_WordMixer -> getStatusMessage());                                      // Step 4: update the errors/results label to reflect that the level has changed
    show();
}


