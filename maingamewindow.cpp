#include <QHBoxLayout>
#include <QApplication>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QShortcut>
#include "maingamewindow.h"
#include "gamestrings.h"

MainGameWindow::MainGameWindow(QWidget *parent)
    : QWidget{parent},
      m_HintsWindow{nullptr},
      m_MixedWords{},
      m_WordMixer{nullptr},
      m_ScoreItem{new ScoreItem{this}},
      m_AlreadyAccessed{false}
{
    QHBoxLayout *statisticsLayout{new QHBoxLayout{}};
    m_HighScores = new QLabel{};
    m_HighScores -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_HighScores -> setToolTip(GameStrings::c_HighscoresToolTip);
    m_NrOfWordPairs = new QLabel{};
    m_NrOfWordPairs -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_NrOfWordPairs -> setToolTip(GameStrings::c_WordPairsToolTip);
    QPushButton *resetButton{new QPushButton{GameStrings::c_ResetButtonLabel}};
    resetButton -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    resetButton -> setToolTip(GameStrings::c_ResetButtonToolTip);
    QShortcut *resetButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_ResetShortcut},this}};
    statisticsLayout -> addWidget(m_HighScores);
    statisticsLayout -> addWidget(m_NrOfWordPairs);
    statisticsLayout -> addSpacing(30);
    statisticsLayout -> addWidget(resetButton);

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
    QShortcut *levelEasyShortcut{new QShortcut{QKeySequence{GameStrings::c_LevelEasyButtonShortcut},this}};
    m_LevelMediumButton = new QRadioButton{GameStrings::c_LevelMediumButtonLabel};
    QShortcut *levelMediumShortcut{new QShortcut{QKeySequence{GameStrings::c_LevelMediumButtonShortcut},this}};
    m_LevelMediumButton -> setChecked(true);
    m_LevelHardButton = new QRadioButton{GameStrings::c_LevelHardButtonLabel};
    QShortcut *levelHardShortcut{new QShortcut{QKeySequence{GameStrings::c_LevelHardButtonShortcut},this}};
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

    m_MixedWordsLayout = new QHBoxLayout{};
    m_MixedWordsLayout -> setSpacing(0);

    QHBoxLayout *wordsLayout{new QHBoxLayout{}};
    m_FirstWord = new QLineEdit{};
    m_FirstWord -> setToolTip(GameStrings::c_FirstWordToolTip);
    m_SecondWord = new QLineEdit{};
    m_SecondWord -> setToolTip(GameStrings::c_SecondWordToolTip);
    wordsLayout -> addWidget(m_FirstWord);
    wordsLayout -> addWidget(m_SecondWord);

    QHBoxLayout *buttonsLayout{new QHBoxLayout{}};
    buttonsLayout -> setSpacing(25);
    QPushButton *submitButton{new QPushButton{GameStrings::c_SubmitButtonLabel}};
    submitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    submitButton -> setToolTip(GameStrings::c_SubmitButtonToolTip);
    QShortcut *submitButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_SubmitButtonShortcut},this}};
    QShortcut *submitButtonAltShortcut{new QShortcut{QKeySequence{GameStrings::c_SubmitButtonAltShortcut},this}};
    QPushButton *hintsButton{new QPushButton{GameStrings::c_HelpButtonLabel}};
    hintsButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    hintsButton -> setToolTip(GameStrings::c_HelpButtonToolTip);
    QShortcut *hintsButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_HelpButtonShortcut},this}};
    QPushButton *showResultsButton{new QPushButton{GameStrings::c_ResultsButtonLabel}};
    showResultsButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    showResultsButton -> setToolTip(GameStrings::c_ResultsButtonToolTip);
    QShortcut *showResultsButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_ResultsButtonShortcut},this}};
    QPushButton *quitButton{new QPushButton{GameStrings::c_QuitButtonLabel}};
    quitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    quitButton -> setToolTip(GameStrings::c_QuitButtonToolTip);
    QShortcut *quitButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_QuitButtonShortcut},this}};
    buttonsLayout -> addWidget(submitButton);
    buttonsLayout -> addWidget(hintsButton);
    buttonsLayout -> addWidget(showResultsButton);
    buttonsLayout -> addWidget(quitButton);

    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(statisticsLayout,1);
    mainLayout -> addLayout(levelsStatusReqInputLayout);
    mainLayout -> addLayout(m_MixedWordsLayout);
    mainLayout -> addLayout(wordsLayout);
    mainLayout -> addLayout(buttonsLayout);
    setLayout(mainLayout);
    setToolTip(GameStrings::c_MainWindowToolTip);
    setAttribute(Qt::WA_AlwaysShowToolTips);

    connect(m_ScoreItem,&ScoreItem::statisticsUpdated,this,&MainGameWindow::onStatisticsUpdated);
    connect(this,&MainGameWindow::levelChanged,m_ScoreItem,&ScoreItem::setScoreIncrement);
    connect(resetButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonResetClicked);
    connect(resetButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonResetClicked);
    connect(m_LevelEasyButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonEasyToggled);
    connect(levelEasyShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonEasyShortcut);
    connect(m_LevelMediumButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonMediumToggled);
    connect(levelMediumShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonMediumShortcut);
    connect(m_LevelHardButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonHardToggled);
    connect(levelHardShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonHardShortcut);
    connect(submitButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonSubmitClicked);
    connect(submitButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonSubmitClicked);
    connect(submitButtonAltShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonSubmitClicked);
    connect(hintsButton,&QPushButton::clicked, this,&MainGameWindow::_onButtonHintsClicked);
    connect(hintsButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonHintsClicked);
    connect(showResultsButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonResultsClicked);
    connect(showResultsButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonResultsClicked);
    connect(quitButton, &QPushButton::clicked,qApp,&QApplication::quit);
    connect(quitButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
}

void MainGameWindow::updateHintsWinPtr(HintsWindow *hw)
{
    m_HintsWindow = hw;
}

void MainGameWindow::assignWordMixer(WordMixer *wm)
{
    m_WordMixer = wm;
    if (m_WordMixer) {
        connect(this,&MainGameWindow::levelChanged,m_WordMixer, &WordMixer::setWordPieceSize);
    }
}

bool MainGameWindow::windowAlreadyAccessed() const
{
    return m_AlreadyAccessed;
}

void MainGameWindow::getFirstTwoWords()
{
    m_AlreadyAccessed = true;
    m_WordMixer -> mixWords();
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    m_ScoreItem -> resetStatistics();
    m_ResultsErrors -> setText(m_WordMixer -> getStatusMessage());
}

void MainGameWindow::onStatisticsUpdated()
{
    QVector<int> scoresPairs{m_ScoreItem -> getStatistics()};
    m_HighScores -> setText("High-score: " + QString::number(scoresPairs[0]) + "/" + QString::number(scoresPairs[1]));
    m_NrOfWordPairs -> setText("Word pairs: " + QString::number(scoresPairs[2]) + "/" + QString::number(scoresPairs[3]));
}

void MainGameWindow::_onButtonResetClicked()
{
    hide();
    m_ScoreItem -> resetStatistics();
    m_ResultsErrors -> setText(GameStrings::c_ScoresResetMessage);
    show();
}

void MainGameWindow::_onButtonEasyToggled(bool checked)
{
    if (checked)
    {
        _switchToLevel(Game::Level::EASY);
    }
}

void MainGameWindow::_onButtonEasyShortcut()
{
    if (!(m_LevelEasyButton -> isChecked()))
    {
        hide();
        m_LevelEasyButton -> setChecked(true);
        show();
    }
}

void MainGameWindow::_onButtonMediumToggled(bool checked)
{
    if (checked)
    {
        _switchToLevel(Game::Level::MEDIUM);
    }
}

void MainGameWindow::_onButtonMediumShortcut()
{
    if (!(m_LevelMediumButton -> isChecked()))
    {
        hide();
        m_LevelMediumButton -> setChecked(true);
        show();
    }
}

void MainGameWindow::_onButtonHardToggled(bool checked)
{
    if (checked)
    {
        _switchToLevel(Game::Level::HARD);
    }
}

void MainGameWindow::_onButtonHardShortcut()
{
    if (!(m_LevelHardButton -> isChecked()))
    {
        hide();
        m_LevelHardButton -> setChecked(true);
        show();
    }
}

void MainGameWindow::_onButtonSubmitClicked()
{
    hide();
    QString firstInputWord{m_FirstWord -> text()};
    QString secondInputword{m_SecondWord -> text()};
    m_FirstWord -> clear();
    m_SecondWord -> clear();
    m_FirstWord -> setFocus();

    if (m_WordMixer -> checkWords(firstInputWord,secondInputword))
    {
        m_WordMixer -> mixWords();
        _removeMixedWordsLabels();
        _createMixedWordsLabels();
        _addMixedWordsLabels();
        m_ScoreItem -> updateStatistics(FULL_UPDATE);
    }
    m_ResultsErrors -> setText(m_WordMixer -> getStatusMessage());
    show();
}

void MainGameWindow::_onButtonHintsClicked()
{
    hide();
    m_HintsWindow -> show();
}

void MainGameWindow::_onButtonResultsClicked()
{
    hide();
    m_FirstWord -> clear();
    m_SecondWord -> clear();
    m_FirstWord -> setFocus();
    m_WordMixer -> retrieveResults();
    m_ScoreItem -> updateStatistics(PARTIAL_UPDATE);
    m_WordMixer -> mixWords();
    _removeMixedWordsLabels();
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    m_ResultsErrors -> setText(m_WordMixer -> getStatusMessage());
    show();
}

void MainGameWindow::_removeMixedWordsLabels()
{
    for(auto currentWordPiece : m_MixedWords)
    {
        m_MixedWordsLayout -> removeWidget(currentWordPiece);
        delete currentWordPiece;
        currentWordPiece = nullptr;
    }
}

void MainGameWindow::_createMixedWordsLabels()
{
    int newNumberOfPieces{(m_WordMixer->getMixedWordsStringArray()).size()};
    m_MixedWords.resize(newNumberOfPieces);
    for (int wordPieceIndex{0}; wordPieceIndex < newNumberOfPieces; wordPieceIndex++)
    {
        m_MixedWords[wordPieceIndex] = new QLabel{};
        m_MixedWords[wordPieceIndex] -> setText((m_WordMixer -> getMixedWordsStringArray()).at(wordPieceIndex));
        m_MixedWords[wordPieceIndex] -> setStyleSheet(GameStrings::c_WordEachPieceStyle);
    }

    m_MixedWords[m_WordMixer -> getFirstWordBeginIndex()]  -> setStyleSheet(GameStrings::c_WordBeginPieceStyle);

    m_MixedWords[m_WordMixer -> getSecondWordBeginIndex()] -> setStyleSheet(GameStrings::c_WordBeginPieceStyle);

    m_MixedWords[m_WordMixer -> getFirstWordEndIndex()]    -> setStyleSheet(GameStrings::c_WordEndPieceStyle);

    m_MixedWords[m_WordMixer -> getSecondWordEndIndex()]   -> setStyleSheet(GameStrings::c_WordEndPieceStyle);

}

void MainGameWindow::_addMixedWordsLabels()
{
    for (auto currentWordPiece : m_MixedWords)
    {
        m_MixedWordsLayout -> addWidget(currentWordPiece);
    }
}

void MainGameWindow::_switchToLevel(Game::Level level)
{
    hide();
    m_FirstWord -> clear();
    m_SecondWord -> clear();
    m_FirstWord -> setFocus();
    Q_EMIT levelChanged(level);
    m_WordMixer -> mixWords();
    _removeMixedWordsLabels();
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    m_ResultsErrors -> setText(GameStrings::c_LevelChangedMessage);
    show();
}


