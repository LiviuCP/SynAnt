#include "maingamewindow.h"

#include "../OtherResources/styles.h"
#include "../OtherResources/selectablelabel.h"

#include "../../Common/wordmixer.h"
#include "../../Common/scoreitem.h"
#include "../../Common/gamestrings.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QShortcut>

MainGameWindow::MainGameWindow(WordMixer *wordMixer, QWidget *parent)
    : QWidget{parent},
      m_MixedWordsLabels{},
      m_pWordMixer{wordMixer},
      m_pScoreItem{new ScoreItem{this}},
      m_IsInitialized{false},
      m_IsSubmitEnabled{false},
      m_IsResetEnabled{false}
{
    Q_ASSERT(m_pWordMixer);
    m_pWordMixer -> setParent(this);

    QHBoxLayout *statisticsLayout{new QHBoxLayout{}};
    m_pHighScoresLabel = new QLabel{};
    m_pHighScoresLabel -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_pHighScoresLabel -> setToolTip(GameStrings::c_HighscoresToolTip);
    m_pNrOfWordPairsLabel = new QLabel{};
    m_pNrOfWordPairsLabel -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_pNrOfWordPairsLabel -> setToolTip(GameStrings::c_WordPairsToolTip);
    m_pResetButton = new QPushButton{GameStrings::c_ResetButtonLabel};
    m_pResetButton -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pResetButton -> setToolTip(GameStrings::c_ResetButtonToolTip);
    m_pResetButtonShortcut = new QShortcut{QKeySequence{GameStrings::c_ResetShortcut},this};
    // enable reset only if statistics are different from 0
    m_pResetButton -> setEnabled(m_IsResetEnabled);
    m_pResetButtonShortcut ->setEnabled(m_IsResetEnabled);
    statisticsLayout -> addWidget(m_pHighScoresLabel);
    statisticsLayout -> addWidget(m_pNrOfWordPairsLabel);
    statisticsLayout -> addSpacing(width()/25);
    statisticsLayout -> addWidget(m_pResetButton);

    QHBoxLayout *levelsStatusReqInputLayout{new QHBoxLayout{}};
    QLabel *requestInput{new QLabel{}};
    requestInput -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    requestInput -> setMinimumWidth(width()/1.8);
    requestInput -> setWordWrap(true);
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
    m_pResultsErrorsLabel = new QLabel{};
    m_pResultsErrorsLabel -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pResultsErrorsLabel -> setMinimumSize(width()/3, height()/1.6);
    m_pResultsErrorsLabel -> setWordWrap(true);
    m_pResultsErrorsLabel -> setToolTip(GameStrings::c_GameStatusToolTip);
    levelsStatusReqInputLayout -> addWidget(requestInput);
    levelsStatusReqInputLayout -> addWidget(levelButtonsBox);
    levelsStatusReqInputLayout -> addWidget(m_pResultsErrorsLabel);

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
    m_pSubmitButton = new QPushButton{GameStrings::c_SubmitButtonLabel};
    m_pSubmitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_pSubmitButton -> setToolTip(GameStrings::c_SubmitButtonToolTip);
    m_pSubmitButtonShortcut = new QShortcut{QKeySequence{GameStrings::c_SubmitButtonShortcut},this};
    // enable the submit button/shortcut only if user entered input in both linedit fields
    m_pSubmitButton -> setEnabled(m_IsSubmitEnabled);
    m_pSubmitButtonShortcut -> setEnabled(m_IsSubmitEnabled);
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
    buttonsLayout -> addWidget(m_pSubmitButton);
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
    setWindowTitle(GameStrings::c_MainWindowTitle);

    m_pFirstWordLineEdit -> setFocus();

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
    connected = connect(m_pSubmitButton,&QPushButton::clicked,this,&MainGameWindow::_onButtonSubmitClicked);
    Q_ASSERT(connected);
    connected = connect(m_pSubmitButtonShortcut,&QShortcut::activated,this,&MainGameWindow::_onButtonSubmitClicked);
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
    connected = connect(this,&MainGameWindow::keyReleased,this,&MainGameWindow::_onKeyReleased);
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
    m_pHighScoresLabel -> setText(GameStrings::c_HighscoresMessage.arg(m_pScoreItem->getObtainedScore())
                                                             .arg(m_pScoreItem->getTotalAvailableScore()));
    m_pNrOfWordPairsLabel -> setText(GameStrings::c_WordPairsMessage.arg(m_pScoreItem->getGuessedWordPairs())
                                                               .arg(m_pScoreItem->getTotalWordPairs()));
}

void MainGameWindow::keyReleaseEvent(QKeyEvent *keyEvent)
{
    (void) keyEvent;
    emit keyReleased();
}

void MainGameWindow::_onButtonResetClicked()
{
    m_pScoreItem -> resetStatistics();
    _updateStatusMessage(Game::StatusCodes::STATISTICS_RESET);
    _setResetEnabled(false);
    m_pFirstWordLineEdit->setFocus();
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

        _setResetEnabled(true);

        m_pFirstWordLineEdit -> clear();
        m_pSecondWordLineEdit -> clear();

        _setSubmitEnabled(false);
    }
    m_pFirstWordLineEdit -> setFocus();
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

    _setResetEnabled(true);

    _setSubmitEnabled(false);
}

void MainGameWindow::_onKeyReleased()
{
    if ((m_pFirstWordLineEdit->text()).size() != 0 && (m_pSecondWordLineEdit->text()).size() != 0)
    {
        _setSubmitEnabled(true);
    }
    else
    {
        _setSubmitEnabled(false);
    }
}

Game::StatusCodes MainGameWindow::_checkWords(const QString &firstWord, const QString &secondWord)
{
    Game::StatusCodes statusCode;

    const QString firstWordRef{m_pWordMixer->getFirstWord()};
    const QString secondWordRef{m_pWordMixer->getSecondWord()};

    if (((firstWord == firstWordRef) && (secondWord == secondWordRef)) || ((firstWord == secondWordRef) && (secondWord == firstWordRef)))
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
    for(auto currentWordPiece : m_MixedWordsLabels)
    {
        m_pMixedWordsLayout -> removeWidget(currentWordPiece);
        delete currentWordPiece;
        currentWordPiece = nullptr;
    }
}

void MainGameWindow::_createMixedWordsLabels()
{
    int newNumberOfPieces{(m_pWordMixer->getMixedWordsStringArray()).size()};
    m_MixedWordsLabels.resize(newNumberOfPieces);
    for (int wordPieceIndex{0}; wordPieceIndex < newNumberOfPieces; wordPieceIndex++)
    {
        m_MixedWordsLabels[wordPieceIndex] = new SelectableLabel{};
        m_MixedWordsLabels[wordPieceIndex] -> setText((m_pWordMixer -> getMixedWordsStringArray()).at(wordPieceIndex));
    }

    m_MixedWordsLabels[m_pWordMixer -> getFirstWordFirstPieceIndex() ] -> setLabelNotSelectedStyleSheet(Styles::c_WordFirstPieceNotSelectedStyle);
    m_MixedWordsLabels[m_pWordMixer -> getSecondWordFirstPieceIndex()] -> setLabelNotSelectedStyleSheet(Styles::c_WordFirstPieceNotSelectedStyle);
    m_MixedWordsLabels[m_pWordMixer -> getFirstWordLastPieceIndex()  ] -> setLabelNotSelectedStyleSheet(Styles::c_WordLastPieceNotSelectedStyle);
    m_MixedWordsLabels[m_pWordMixer -> getSecondWordLastPieceIndex() ] -> setLabelNotSelectedStyleSheet(Styles::c_WordLastPieceNotSelectedStyle);

    m_MixedWordsLabels[m_pWordMixer -> getFirstWordFirstPieceIndex() ] -> setLabelSelectedStyleSheet(Styles::c_WordFirstPieceSelectedStyle);
    m_MixedWordsLabels[m_pWordMixer -> getSecondWordFirstPieceIndex()] -> setLabelSelectedStyleSheet(Styles::c_WordFirstPieceSelectedStyle);
    m_MixedWordsLabels[m_pWordMixer -> getFirstWordLastPieceIndex()  ] -> setLabelSelectedStyleSheet(Styles::c_WordLastPieceSelectedStyle);
    m_MixedWordsLabels[m_pWordMixer -> getSecondWordLastPieceIndex() ] -> setLabelSelectedStyleSheet(Styles::c_WordLastPieceSelectedStyle);
}

void MainGameWindow::_addMixedWordsLabels()
{
    for (auto currentWordPiece : m_MixedWordsLabels)
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
    _setSubmitEnabled(false);
}

void MainGameWindow::_updateStatusMessage(Game::StatusCodes statusCode)
{
    QString statusMessage{};

    switch(statusCode)
    {
    case Game::StatusCodes::SUCCESS:
        statusMessage = GameStrings::c_SuccessMessage.arg(m_pWordMixer->getFirstWord())
                                                     .arg(m_pWordMixer->getSecondWord())
                                                     .arg(m_pWordMixer->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
        break;
    case Game::StatusCodes::INCORRECT_WORDS:
        statusMessage = GameStrings::c_IncorrectWordsMessage;
        break;
    case Game::StatusCodes::REQUESTED_BY_USER:
        statusMessage = GameStrings::c_RequestedByUserMessage.arg(m_pWordMixer->getFirstWord())
                                                             .arg(m_pWordMixer->getSecondWord())
                                                             .arg(m_pWordMixer->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
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

    m_pResultsErrorsLabel -> setText(statusMessage);
}

void MainGameWindow::_setSubmitEnabled(bool enabled)
{
    if (m_IsSubmitEnabled != enabled)
    {
        m_pSubmitButton->setEnabled(enabled);
        m_pSubmitButtonShortcut->setEnabled(enabled);
        m_IsSubmitEnabled = enabled;
    }
}

void MainGameWindow::_setResetEnabled(bool enabled)
{
    if (m_IsResetEnabled != enabled)
    {
        m_pResetButton->setEnabled(enabled);
        m_pResetButtonShortcut->setEnabled(enabled);
        m_IsResetEnabled = enabled;
    }
}


