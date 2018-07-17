#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QShortcut>

#include "appmanager.h"
#include "maingamewindow.h"
#include "wordmixer.h"
#include "scoreitem.h"
#include "gamestrings.h"


MainGameWindow::MainGameWindow(WordMixer *wordMixer, QWidget *parent)
    : QWidget{parent},
      m_MixedWords{},
      m_WordMixer{wordMixer},
      m_ScoreItem{new ScoreItem{this}},
      m_IsInitialized{false}
{
    Q_ASSERT(m_WordMixer);
    m_WordMixer -> setParent(this);

    QHBoxLayout *statisticsLayout{new QHBoxLayout{}};
    m_HighScores = new QLabel{};
    m_HighScores -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_HighScores -> setToolTip(GameStrings::c_HighscoresToolTip);
    m_NrOfWordPairs = new QLabel{};
    m_NrOfWordPairs -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_NrOfWordPairs -> setToolTip(GameStrings::c_WordPairsToolTip);
    m_ResetButton = new QPushButton{GameStrings::c_ResetButtonLabel};
    m_ResetButton -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_ResetButton -> setToolTip(GameStrings::c_ResetButtonToolTip);
    m_ResetButtonShortcut = new QShortcut{QKeySequence{GameStrings::c_ResetShortcut},this};
    // enable reset only if statistics are different from 0
    m_ResetButton -> setEnabled(false);
    m_ResetButtonShortcut ->setEnabled(false);
    statisticsLayout -> addWidget(m_HighScores);
    statisticsLayout -> addWidget(m_NrOfWordPairs);
    statisticsLayout -> addSpacing(30);
    statisticsLayout -> addWidget(m_ResetButton);

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

    bool connected{connect(m_ScoreItem,&ScoreItem::statisticsUpdated,this,&MainGameWindow::onStatisticsUpdated)};
    Q_ASSERT(connected);
    connected = connect(this,&MainGameWindow::levelChanged,m_ScoreItem,&ScoreItem::setScoreIncrement);
    Q_ASSERT(connected);
    connected = connect(m_ResetButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonResetClicked);
    Q_ASSERT(connected);
    connected = connect(m_ResetButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonResetClicked);
    Q_ASSERT(connected);
    connected = connect(m_LevelEasyButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonEasyToggled);
    Q_ASSERT(connected);
    connected = connect(levelEasyShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonEasyShortcutEntered);
    Q_ASSERT(connected);
    connected = connect(m_LevelMediumButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonMediumToggled);
    Q_ASSERT(connected);
    connected = connect(levelMediumShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonMediumShortcutEntered);
    Q_ASSERT(connected);
    connected = connect(m_LevelHardButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonHardToggled);
    Q_ASSERT(connected);
    connected = connect(levelHardShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonHardShortcutEntered);
    Q_ASSERT(connected);
    connected = connect(submitButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonSubmitClicked);
    Q_ASSERT(connected);
    connected = connect(submitButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonSubmitClicked);
    Q_ASSERT(connected);
    connected = connect(submitButtonAltShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonSubmitClicked);
    Q_ASSERT(connected);
    connected = connect(hintsButton,&QPushButton::clicked,this,&MainGameWindow::switchedMaintoHints);
    Q_ASSERT(connected);
    connected = connect(hintsButtonShortcut,&QShortcut::activated,this,&MainGameWindow::switchedMaintoHints);
    Q_ASSERT(connected);
    connected = connect(showResultsButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonResultsClicked);
    Q_ASSERT(connected);
    connected = connect(showResultsButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonResultsClicked);
    Q_ASSERT(connected);
    connected = connect(quitButton, &QPushButton::clicked,qApp,&QApplication::quit);
    Q_ASSERT(connected);
    connected = connect(quitButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    Q_ASSERT(connected);
    connected = connect(this,&MainGameWindow::levelChanged,m_WordMixer,&WordMixer::setWordPieceSize);
    Q_ASSERT(connected);
}

bool MainGameWindow::isInitialized() const
{
    return m_IsInitialized;
}

void MainGameWindow::init()
{
    m_IsInitialized = true;
    m_WordMixer -> mixWords();
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    m_ScoreItem -> resetStatistics();
    _updateStatusMessage(Game::StatusCodes::NO_USER_INPUT);
}

void MainGameWindow::onStatisticsUpdated()
{
    QVector<int> scoresPairs{m_ScoreItem -> getStatistics()};
    m_HighScores -> setText("High-score: " + QString::number(scoresPairs[0]) + "/" + QString::number(scoresPairs[1]));
    m_NrOfWordPairs -> setText("Word pairs: " + QString::number(scoresPairs[2]) + "/" + QString::number(scoresPairs[3]));
}

void MainGameWindow::_onButtonResetClicked()
{
    m_ScoreItem -> resetStatistics();
    _updateStatusMessage(Game::StatusCodes::STATISTICS_RESET);
    m_ResetButton -> setEnabled(false);
    m_ResetButtonShortcut -> setEnabled(false);
}

void MainGameWindow::_onButtonEasyToggled(bool checked)
{
    if (checked)
    {
        _switchToLevel(Game::Level::EASY);
    }
}

void MainGameWindow::_onButtonEasyShortcutEntered()
{
    if (!(m_LevelEasyButton -> isChecked()))
    {
        m_LevelEasyButton -> setChecked(true);
    }
}

void MainGameWindow::_onButtonMediumToggled(bool checked)
{
    if (checked)
    {
        _switchToLevel(Game::Level::MEDIUM);
    }
}

void MainGameWindow::_onButtonMediumShortcutEntered()
{
    if (!(m_LevelMediumButton -> isChecked()))
    {
        m_LevelMediumButton -> setChecked(true);
    }
}

void MainGameWindow::_onButtonHardToggled(bool checked)
{
    if (checked)
    {
        _switchToLevel(Game::Level::HARD);
    }
}

void MainGameWindow::_onButtonHardShortcutEntered()
{
    if (!(m_LevelHardButton -> isChecked()))
    {
        m_LevelHardButton -> setChecked(true);
    }
}

void MainGameWindow::_onButtonSubmitClicked()
{
    QString firstInputWord{m_FirstWord -> text()};
    QString secondInputword{m_SecondWord -> text()};
    m_FirstWord -> clear();
    m_SecondWord -> clear();
    m_FirstWord -> setFocus();

    Game::StatusCodes statusCode {m_WordMixer->checkWords(firstInputWord, secondInputword)};
    // update the status message before retrieving any new words
    _updateStatusMessage(statusCode);
    if (statusCode == Game::StatusCodes::SUCCESS)
    {
        m_WordMixer -> mixWords();
        _removeMixedWordsLabels();
        _createMixedWordsLabels();
        _addMixedWordsLabels();
        m_ScoreItem -> updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

        if (!m_ResetButton->isEnabled())
        {
            m_ResetButton->setEnabled(true);
        }

        if (!m_ResetButtonShortcut->isEnabled())
        {
            m_ResetButtonShortcut->setEnabled(true);
        }
    }
}

void MainGameWindow::_onButtonResultsClicked()
{
    m_FirstWord -> clear();
    m_SecondWord -> clear();
    m_FirstWord -> setFocus();
    _updateStatusMessage(Game::StatusCodes::REQUESTED_BY_USER);
    m_WordMixer -> mixWords();
    _removeMixedWordsLabels();
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    m_ScoreItem -> updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);

    if (!m_ResetButton->isEnabled())
    {
        m_ResetButton->setEnabled(true);
    }

    if (!m_ResetButtonShortcut->isEnabled())
    {
        m_ResetButtonShortcut->setEnabled(true);
    }
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
    m_FirstWord -> clear();
    m_SecondWord -> clear();
    m_FirstWord -> setFocus();
    Q_EMIT levelChanged(level);
    m_WordMixer -> mixWords();
    _removeMixedWordsLabels();
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    _updateStatusMessage(Game::StatusCodes::LEVEL_CHANGED);
}

void MainGameWindow::_updateStatusMessage(Game::StatusCodes statusCode)
{
    QString statusMessage{};

    switch(statusCode)
    {
    case Game::StatusCodes::SUCCESS:
        statusMessage = GameStrings::c_SuccessMessage;
        statusMessage += "\n\nThe two words are:\n\n";
        statusMessage += "\t" + m_WordMixer->getFirstWord() + "\n";
        statusMessage += "\t" + m_WordMixer->getSecondWord() + "\n";
        statusMessage += "\nThe words are: \n\n\t";
        statusMessage += m_WordMixer->areSynonyms() ? "synonyms" : "antonyms";
        statusMessage += "\n\nNext pair of words is available below.";
        break;
    case Game::StatusCodes::MISSING_WORDS:
        statusMessage = GameStrings::c_MissingWordsMessage;
        break;
    case Game::StatusCodes::INCORRECT_WORDS:
        statusMessage = GameStrings::c_IncorrectWordsMessage;
        break;
    case Game::StatusCodes::REQUESTED_BY_USER:
        statusMessage = "The correct words are: \n\n";
        statusMessage += "\t" + m_WordMixer->getFirstWord() + "\n";
        statusMessage += "\t" + m_WordMixer->getSecondWord() + "\n";
        statusMessage += "\nThe words are: \n\n\t";
        statusMessage += m_WordMixer->areSynonyms() ? "synonyms" : "antonyms";
        statusMessage += "\n\nNext pair of words is available below.";
        break;
    case Game::StatusCodes::NO_USER_INPUT:
        statusMessage = GameStrings::c_NoUserInputMessage;
        break;
    case Game::StatusCodes::STATISTICS_RESET:
        statusMessage = GameStrings::c_ScoresResetMessage;
        break;
    case Game::StatusCodes::LEVEL_CHANGED:
        statusMessage = GameStrings::c_LevelChangedMessage;
    // reserved for future use
    default:
        ;
    }

    m_ResultsErrors -> setText(statusMessage);
}


