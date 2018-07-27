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
      m_pWordMixer{wordMixer},
      m_pScoreItem{new ScoreItem{this}},
      m_IsInitialized{false}
{
    Q_ASSERT(m_pWordMixer);
    m_pWordMixer -> setParent(this);

    QHBoxLayout *statisticsLayout{new QHBoxLayout{}};
    m_pHighScores = new QLabel{};
    m_pHighScores -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_pHighScores -> setToolTip(GameStrings::c_HighscoresToolTip);
    m_pNrOfWordPairs = new QLabel{};
    m_pNrOfWordPairs -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_pNrOfWordPairs -> setToolTip(GameStrings::c_WordPairsToolTip);
    m_pResetButton = new QPushButton{GameStrings::c_ResetButtonLabel};
    m_pResetButton -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pResetButton -> setToolTip(GameStrings::c_ResetButtonToolTip);
    m_pResetButtonShortcut = new QShortcut{QKeySequence{GameStrings::c_ResetShortcut},this};
    // enable reset only if statistics are different from 0
    m_pResetButton -> setEnabled(false);
    m_pResetButtonShortcut ->setEnabled(false);
    statisticsLayout -> addWidget(m_pHighScores);
    statisticsLayout -> addWidget(m_pNrOfWordPairs);
    statisticsLayout -> addSpacing(30);
    statisticsLayout -> addWidget(m_pResetButton);

    QHBoxLayout *levelsStatusReqInputLayout{new QHBoxLayout{}};
    QLabel *requestInput{new QLabel{}};
    requestInput -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    requestInput -> setToolTip(GameStrings::c_GameInstructionsToolTip);
    QString requestInputText{GameStrings::c_InstructionsMessage};
    requestInput -> setText(requestInputText);
    QGroupBox *levelButtonsBox{new QGroupBox{}};
    levelButtonsBox -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    levelButtonsBox -> setToolTip(GameStrings::c_LevelButtonsToolTip);
    m_pLevelEasyButton = new QRadioButton{GameStrings::c_LevelEasyButtonLabel};
    QShortcut *levelEasyShortcut{new QShortcut{QKeySequence{GameStrings::c_LevelEasyButtonShortcut},this}};
    m_pLevelMediumButton = new QRadioButton{GameStrings::c_LevelMediumButtonLabel};
    QShortcut *levelMediumShortcut{new QShortcut{QKeySequence{GameStrings::c_LevelMediumButtonShortcut},this}};
    m_pLevelMediumButton -> setChecked(true);
    m_pLevelHardButton = new QRadioButton{GameStrings::c_LevelHardButtonLabel};
    QShortcut *levelHardShortcut{new QShortcut{QKeySequence{GameStrings::c_LevelHardButtonShortcut},this}};
    QVBoxLayout *levelButtonsLayout{new QVBoxLayout};
    levelButtonsLayout -> addWidget(m_pLevelEasyButton);
    levelButtonsLayout -> addWidget(m_pLevelMediumButton);
    levelButtonsLayout -> addWidget(m_pLevelHardButton);
    levelButtonsBox -> setLayout(levelButtonsLayout);
    m_pResultsErrors = new QLabel{};
    m_pResultsErrors -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pResultsErrors -> setToolTip(GameStrings::c_GameStatusToolTip);
    levelsStatusReqInputLayout -> addWidget(requestInput);
    levelsStatusReqInputLayout -> addWidget(levelButtonsBox);
    levelsStatusReqInputLayout -> addWidget(m_pResultsErrors);

    m_pMixedWordsLayout = new QHBoxLayout{};
    m_pMixedWordsLayout -> setSpacing(0);

    QHBoxLayout *wordsLayout{new QHBoxLayout{}};
    m_pFirstWordLineEdit = new QLineEdit{};
    m_pFirstWordLineEdit -> setToolTip(GameStrings::c_FirstWordToolTip);
    m_pSecondWordLineEdit = new QLineEdit{};
    m_pSecondWordLineEdit -> setToolTip(GameStrings::c_SecondWordToolTip);
    wordsLayout -> addWidget(m_pFirstWordLineEdit);
    wordsLayout -> addWidget(m_pSecondWordLineEdit);

    QHBoxLayout *buttonsLayout{new QHBoxLayout{}};
    buttonsLayout -> setSpacing(25);
    QPushButton *submitButton{new QPushButton{GameStrings::c_SubmitButtonLabel}};
    submitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    submitButton -> setToolTip(GameStrings::c_SubmitButtonToolTip);
    QShortcut *submitButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_SubmitButtonShortcut},this}};
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
    mainLayout -> addLayout(m_pMixedWordsLayout);
    mainLayout -> addLayout(wordsLayout);
    mainLayout -> addLayout(buttonsLayout);
    setLayout(mainLayout);
    setToolTip(GameStrings::c_MainWindowToolTip);
    setAttribute(Qt::WA_AlwaysShowToolTips);

    bool connected{connect(m_pScoreItem,&ScoreItem::statisticsUpdated,this,&MainGameWindow::onStatisticsUpdated)};
    Q_ASSERT(connected);
    connected = connect(this,&MainGameWindow::levelChanged,m_pScoreItem,&ScoreItem::setScoreIncrement);
    Q_ASSERT(connected);
    connected = connect(m_pResetButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonResetClicked);
    Q_ASSERT(connected);
    connected = connect(m_pResetButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonResetClicked);
    Q_ASSERT(connected);
    connected = connect(m_pLevelEasyButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonEasyToggled);
    Q_ASSERT(connected);
    connected = connect(levelEasyShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonEasyShortcutEntered);
    Q_ASSERT(connected);
    connected = connect(m_pLevelMediumButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonMediumToggled);
    Q_ASSERT(connected);
    connected = connect(levelMediumShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonMediumShortcutEntered);
    Q_ASSERT(connected);
    connected = connect(m_pLevelHardButton,&QRadioButton::toggled,this,&MainGameWindow::_onButtonHardToggled);
    Q_ASSERT(connected);
    connected = connect(levelHardShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonHardShortcutEntered);
    Q_ASSERT(connected);
    connected = connect(submitButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonSubmitClicked);
    Q_ASSERT(connected);
    connected = connect(submitButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonSubmitClicked);
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
    connected = connect(this,&MainGameWindow::levelChanged,m_pWordMixer,&WordMixer::setWordPieceSize);
    Q_ASSERT(connected);
}

bool MainGameWindow::isInitialized() const
{
    return m_IsInitialized;
}

void MainGameWindow::init()
{
    m_IsInitialized = true;
    m_pWordMixer -> mixWords();
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    m_pScoreItem -> resetStatistics();
    _updateStatusMessage(Game::StatusCodes::NO_USER_INPUT);
}

void MainGameWindow::onStatisticsUpdated()
{
    QVector<int> scoresPairs{m_pScoreItem -> getStatistics()};
    m_pHighScores -> setText("High-score: " + QString::number(scoresPairs[0]) + "/" + QString::number(scoresPairs[1]));
    m_pNrOfWordPairs -> setText("Word pairs: " + QString::number(scoresPairs[2]) + "/" + QString::number(scoresPairs[3]));
}

void MainGameWindow::_onButtonResetClicked()
{
    m_pScoreItem -> resetStatistics();
    _updateStatusMessage(Game::StatusCodes::STATISTICS_RESET);
    m_pResetButton -> setEnabled(false);
    m_pResetButtonShortcut -> setEnabled(false);
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
    if (!(m_pLevelEasyButton -> isChecked()))
    {
        m_pLevelEasyButton -> setChecked(true);
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
    if (!(m_pLevelMediumButton -> isChecked()))
    {
        m_pLevelMediumButton -> setChecked(true);
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
    if (!(m_pLevelHardButton -> isChecked()))
    {
        m_pLevelHardButton -> setChecked(true);
    }
}

void MainGameWindow::_onButtonSubmitClicked()
{
    QString firstInputWord{m_pFirstWordLineEdit -> text()};
    QString secondInputword{m_pSecondWordLineEdit -> text()};
    m_pFirstWordLineEdit -> clear();
    m_pSecondWordLineEdit -> clear();
    m_pFirstWordLineEdit -> setFocus();

    Game::StatusCodes statusCode {_checkWords(firstInputWord, secondInputword)};
    // update the status message before retrieving any new words
    _updateStatusMessage(statusCode);
    if (statusCode == Game::StatusCodes::SUCCESS)
    {
        m_pWordMixer -> mixWords();
        _removeMixedWordsLabels();
        _createMixedWordsLabels();
        _addMixedWordsLabels();
        m_pScoreItem -> updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

        if (!m_pResetButton->isEnabled())
        {
            m_pResetButton->setEnabled(true);
        }

        if (!m_pResetButtonShortcut->isEnabled())
        {
            m_pResetButtonShortcut->setEnabled(true);
        }
    }
}

void MainGameWindow::_onButtonResultsClicked()
{
    m_pFirstWordLineEdit -> clear();
    m_pSecondWordLineEdit -> clear();
    m_pFirstWordLineEdit -> setFocus();
    _updateStatusMessage(Game::StatusCodes::REQUESTED_BY_USER);
    m_pWordMixer -> mixWords();
    _removeMixedWordsLabels();
    _createMixedWordsLabels();
    _addMixedWordsLabels();
    m_pScoreItem -> updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);

    if (!m_pResetButton->isEnabled())
    {
        m_pResetButton->setEnabled(true);
    }

    if (!m_pResetButtonShortcut->isEnabled())
    {
        m_pResetButtonShortcut->setEnabled(true);
    }
}

Game::StatusCodes MainGameWindow::_checkWords(const QString &firstWord, const QString &secondWord)
{
    Game::StatusCodes statusCode;

    const QString firstWordRef{m_pWordMixer->getFirstWord()};
    const QString secondWordRef{m_pWordMixer->getSecondWord()};

    if (firstWord.isEmpty() || secondWord.isEmpty())
    {
        statusCode = Game::StatusCodes::MISSING_WORDS;
    }

    else if (((firstWord == firstWordRef) && (secondWord == secondWordRef)) || ((firstWord == secondWordRef) && (secondWord == firstWordRef)))
    {
        statusCode = Game::StatusCodes::SUCCESS;
    }
    else
    {
        statusCode = Game::StatusCodes::INCORRECT_WORDS;
    }
    return statusCode;
}

void MainGameWindow::_removeMixedWordsLabels()
{
    for(auto currentWordPiece : m_MixedWords)
    {
        m_pMixedWordsLayout -> removeWidget(currentWordPiece);
        delete currentWordPiece;
        currentWordPiece = nullptr;
    }
}

void MainGameWindow::_createMixedWordsLabels()
{
    int newNumberOfPieces{(m_pWordMixer->getMixedWordsStringArray()).size()};
    m_MixedWords.resize(newNumberOfPieces);
    for (int wordPieceIndex{0}; wordPieceIndex < newNumberOfPieces; wordPieceIndex++)
    {
        m_MixedWords[wordPieceIndex] = new QLabel{};
        m_MixedWords[wordPieceIndex] -> setText((m_pWordMixer -> getMixedWordsStringArray()).at(wordPieceIndex));
        m_MixedWords[wordPieceIndex] -> setStyleSheet(GameStrings::c_WordEachPieceStyle);
    }

    m_MixedWords[m_pWordMixer -> getFirstWordBeginIndex()]  -> setStyleSheet(GameStrings::c_WordBeginPieceStyle);

    m_MixedWords[m_pWordMixer -> getSecondWordBeginIndex()] -> setStyleSheet(GameStrings::c_WordBeginPieceStyle);

    m_MixedWords[m_pWordMixer -> getFirstWordEndIndex()]    -> setStyleSheet(GameStrings::c_WordEndPieceStyle);

    m_MixedWords[m_pWordMixer -> getSecondWordEndIndex()]   -> setStyleSheet(GameStrings::c_WordEndPieceStyle);

}

void MainGameWindow::_addMixedWordsLabels()
{
    for (auto currentWordPiece : m_MixedWords)
    {
        m_pMixedWordsLayout -> addWidget(currentWordPiece);
    }
}

void MainGameWindow::_switchToLevel(Game::Level level)
{
    m_pFirstWordLineEdit -> clear();
    m_pSecondWordLineEdit -> clear();
    m_pFirstWordLineEdit -> setFocus();
    Q_EMIT levelChanged(level);
    m_pWordMixer -> mixWords();
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
        statusMessage += "\t" + m_pWordMixer->getFirstWord() + "\n";
        statusMessage += "\t" + m_pWordMixer->getSecondWord() + "\n";
        statusMessage += "\nThe words are: \n\n\t";
        statusMessage += m_pWordMixer->areSynonyms() ? "synonyms" : "antonyms";
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
        statusMessage += "\t" + m_pWordMixer->getFirstWord() + "\n";
        statusMessage += "\t" + m_pWordMixer->getSecondWord() + "\n";
        statusMessage += "\nThe words are: \n\n\t";
        statusMessage += m_pWordMixer->areSynonyms() ? "synonyms" : "antonyms";
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

    m_pResultsErrors -> setText(statusMessage);
}


