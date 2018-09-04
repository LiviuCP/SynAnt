#include <QGuiApplication>
#include <QDebug>

#include "gamepresenter.h"
#include "../Common/wordmixer.h"
#include "../Common/scoreitem.h"
#include "../Common/gamestrings.h"
#include "../Common/exceptions.h"

static constexpr int c_ToolTipDelay{1000};

GamePresenter::GamePresenter(QObject *parent)
    : QObject(parent)
    , m_IntroPaneVisible {true}
    , m_HelpPaneVisible {false}
    , m_MainPaneVisible {false}
    , m_MainPaneInitialized {false}
    , m_ResetEnabled {false}
    , m_ErrorOccured {false}
    , m_WindowTitle{GameStrings::c_IntroWindowTitle}
    , m_MainPaneStatusMessage {GameStrings::c_NoUserInputMessage}
    , m_CurrentPane {Pane::INTRO}
    , m_pWordMixer {nullptr}
    , m_pScoreItem {new ScoreItem{this}}
{
    m_pWordMixer = new WordMixer{QGuiApplication::applicationDirPath() + "/" + GameStrings::c_FileName, this};

    bool connected{connect(this,&GamePresenter::levelChanged,m_pWordMixer,&WordMixer::setWordPieceSize)};
    Q_ASSERT(connected);
    connected = connect(this,&GamePresenter::levelChanged,m_pScoreItem,&ScoreItem::setScoreIncrement);
    Q_ASSERT(connected);
    connected = connect(m_pScoreItem,&ScoreItem::statisticsUpdated,this,&GamePresenter::_onStatisticsUpdated);
    Q_ASSERT(connected);

    _onStatisticsUpdated();
}

void GamePresenter::switchToPane(Pane pane)
{
    Q_ASSERT(static_cast<int>(pane) >= 0 && static_cast<int>(pane) < static_cast<int>(Pane::Nr_Of_Panes));

    if (m_CurrentPane != pane)
    {
        try
        {
            switch(m_CurrentPane)
            {
            case Pane::INTRO:
                m_IntroPaneVisible = false;
                Q_EMIT introPaneVisibleChanged();
                break;
            case Pane::HELP:
                m_HelpPaneVisible = false;
                Q_EMIT helpPaneVisibleChanged();
                break;
            case Pane::MAIN:
                m_MainPaneVisible = false;
                Q_EMIT mainPaneVisibleChanged();
                break;
            default:
                Q_ASSERT(false);
            }

            switch(pane)
            {
            case Pane::INTRO:
                Q_ASSERT(false);
                break;
            case Pane::HELP:
                m_HelpPaneVisible = true;
                Q_EMIT helpPaneVisibleChanged();
                break;
            case Pane::MAIN:
                if (!m_MainPaneInitialized)
                {
                    _initMainPane();
                }
                m_MainPaneVisible = true;
                Q_EMIT mainPaneVisibleChanged();
                break;
            default:
                Q_ASSERT(false);
            }

            m_CurrentPane = pane;
            Q_EMIT windowTitleChanged();
        }
        catch (const GameException& exception)
        {
            _launchErrorPane(exception.getDescription());
        }
    }
}

void GamePresenter::handleResultsRequest()
{
    try {
        qDebug() << "=====================================================";
        qDebug() << "Words displayed as per user request! New words mixed";

        _updateStatusMessage(Game::StatusCodes::REQUESTED_BY_USER);
        m_pScoreItem -> updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);

        if (!m_ResetEnabled)
        {
            m_ResetEnabled = true;
            Q_EMIT resetEnabledChanged();
        }

        m_pWordMixer -> mixWords();
        Q_EMIT mixedWordsPiecesChanged();
    }
    catch (const GameException& exception)
    {
        _launchErrorPane(exception.getDescription());
    }
}

bool GamePresenter::handleSubmitRequest(const QString &firstWord, const QString &secondWord)
{
    try
    {
        bool clearTextFields{false};

        Game::StatusCodes statusCode{_checkWords(firstWord, secondWord)};

        _updateStatusMessage(statusCode);

        if (statusCode == Game::StatusCodes::SUCCESS)
        {
            qDebug() << "=====================================================";
            qDebug() << "Words guessed by user correctly! New words mixed";

            m_pScoreItem -> updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

            if (!m_ResetEnabled)
            {
                m_ResetEnabled = true;
                Q_EMIT resetEnabledChanged();
            }

            m_pWordMixer -> mixWords();
            Q_EMIT mixedWordsPiecesChanged();

            clearTextFields = true;
        }

        return clearTextFields;
    }
    catch(const GameException& exception)
    {
        _launchErrorPane(exception.getDescription());
    }
}

void GamePresenter::handleResetRequest()
{
    if (m_ResetEnabled)
    {
        qDebug() << "=====================================================";
        qDebug() << "Statistics reset! Same mixed words to be guessed by user";

        m_pScoreItem -> resetStatistics();
        _updateStatusMessage(Game::StatusCodes::STATISTICS_RESET);
        m_ResetEnabled = false;
        Q_EMIT resetEnabledChanged();
    }
}

void GamePresenter::switchToLevel(int level)
{
    try
    {
        Q_ASSERT(level >= 0 && level < static_cast<int>(Game::Level::NrOfLevels));

        qDebug() << "=====================================================";
        qDebug() << "Level changed! New words mixed";

        _setLevel(static_cast<Game::Level>(level));
    }
    catch (const GameException& exception)
    {
        _launchErrorPane(exception.getDescription());
    }
}

int GamePresenter::getLevelEasy() const
{
    return static_cast<int>(Game::Level::EASY);
}

int GamePresenter::getLevelMedium() const
{
    return static_cast<int>(Game::Level::MEDIUM);
}

int GamePresenter::getLevelHard() const
{
    return static_cast<int>(Game::Level::HARD);
}

QString GamePresenter::getPlayButtonLabel() const
{
    return GameStrings::c_PlayButtonLabel;
}

QString GamePresenter::getHelpButtonLabel() const
{
    return GameStrings::c_HelpButtonLabel;
}

QString GamePresenter::getQuitButtonLabel() const
{
    return GameStrings::c_QuitButtonLabel;
}

QString GamePresenter::getSubmitButtonLabel() const
{
    return GameStrings::c_SubmitButtonLabel;
}

QString GamePresenter::getResultsButtonLabel() const
{
    return GameStrings::c_ResultsButtonLabel;
}

QString GamePresenter::getResetButtonLabel() const
{
    return GameStrings::c_ResetButtonLabel;
}

QString GamePresenter::getOkButtonLabel() const
{
    return GameStrings::c_OkButtonLabel;
}

QString GamePresenter::getLevelEasyButtonLabel() const
{
    return GameStrings::c_LevelEasyButtonLabel;
}

QString GamePresenter::getLevelMediumButtonLabel() const
{
    return GameStrings::c_LevelMediumButtonLabel;
}

QString GamePresenter::getLevelHardButtonLabel() const
{
    return GameStrings::c_LevelHardButtonLabel;
}

QString GamePresenter::getCloseButtonLabel() const
{
    return GameStrings::c_FatalErrorQuitButtonLabel;
}

bool GamePresenter::getIntroPaneVisible() const
{
    return m_IntroPaneVisible;
}

bool GamePresenter::getHelpPaneVisible() const
{
    return m_HelpPaneVisible;
}

bool GamePresenter::getMainPaneVisible() const
{
    return m_MainPaneVisible;
}

bool GamePresenter::getResetEnabled() const
{
    return m_ResetEnabled;
}

bool GamePresenter::getErrorOccured() const
{
    return m_ErrorOccured;
}

QString GamePresenter::getWindowTitle() const
{
    QString windowTitle;
    switch(m_CurrentPane)
    {
    case Pane::INTRO:
        windowTitle = GameStrings::c_IntroWindowTitle;
        break;
    case Pane::HELP:
        windowTitle = GameStrings::c_HelpWindowTitle;
        break;
    case Pane::MAIN:
        windowTitle = GameStrings::c_MainWindowTitle;
        break;
    //reserved for future use
    default:
        ;
    }
    return windowTitle;
}

QString GamePresenter::getIntroPaneMessage() const
{
    return GameStrings::c_IntroWindowWelcomeMessage;
}

QString GamePresenter::getHelpPaneMessage() const
{
    return GameStrings::c_HelpWindowMessage;
}

QString GamePresenter::getMainPaneInstructionsMessage() const
{
    return GameStrings::c_InstructionsMessage;
}

QString GamePresenter::getMainPaneStatusMessage() const
{
    return m_MainPaneStatusMessage;
}

QString GamePresenter::getMainPaneScoreMessage() const
{
    return m_MainPaneScoreMessage;
}

QString GamePresenter::getMainPaneWordPairsMessage() const
{
    return m_MainPaneWordPairsMessage;
}

QString GamePresenter::getErrorMessage() const
{
    return m_ErrorMessage;
}

int GamePresenter::getToolTipDelay() const
{
    return c_ToolTipDelay;
}

QString GamePresenter::getIntroPaneToolTip() const
{
    return GameStrings::c_IntroWindowToolTip;
}

QString GamePresenter::getHelpPaneToolTip() const
{
    return GameStrings::c_HelpWindowToolTip;
}

QString GamePresenter::getMainPaneToolTip() const
{
    return GameStrings::c_MainWindowToolTip;
}

QString GamePresenter::getErrorPaneToolTip() const
{
    return GameStrings::c_FatalErrorWindowToolTip;
}

QString GamePresenter::getHighscoresToolTip() const
{
    return GameStrings::c_HighscoresToolTip;
}

QString GamePresenter::getWordPairsToolTip() const
{
    return GameStrings::c_WordPairsToolTip;
}

QString GamePresenter::getGameInstructionsToolTip() const
{
    return GameStrings::c_GameInstructionsToolTip;
}

QString GamePresenter::getGameStatusToolTip() const
{
    return GameStrings::c_GameStatusToolTip;
}

QString GamePresenter::getFirstWordToolTip() const
{
    return GameStrings::c_FirstWordToolTip;
}

QString GamePresenter::getSecondWordToolTip() const
{
    return GameStrings::c_SecondWordToolTip;
}

QString GamePresenter::getPlayButtonToolTip() const
{
    return GameStrings::c_PlayButtonToolTip;
}

QString GamePresenter::getHelpButtonToolTip() const
{
    return GameStrings::c_HelpButtonToolTip;
}

QString GamePresenter::getQuitButtonToolTip() const
{
    return GameStrings::c_QuitButtonToolTip;
}

QString GamePresenter::getOkButtonToolTip() const
{
    return GameStrings::c_OkButtonToolTip;
}

QString GamePresenter::getSubmitButtonToolTip() const
{
    return GameStrings::c_SubmitButtonToolTip;
}

QString GamePresenter::getResultsButtonToolTip() const
{
    return GameStrings::c_ResultsButtonToolTip;
}

QString GamePresenter::getResetButtonToolTip() const
{
    return GameStrings::c_ResetButtonToolTip;
}

QString GamePresenter::getLevelButtonsToolTip() const
{
    return GameStrings::c_LevelButtonsToolTip;
}

QString GamePresenter::getCloseButtonToolTip() const
{
    return GameStrings::c_FatalErrorQuitButtonToolTip;
}

QString GamePresenter::getPlayButtonShortcut() const
{
    return GameStrings::c_PlayButtonShortcut;
}

QString GamePresenter::getHelpButtonShortcut() const
{
    return GameStrings::c_HelpButtonShortcut;
}

QString GamePresenter::getQuitButtonShortcut() const
{
    return GameStrings::c_QuitButtonShortcut;
}

QString GamePresenter::getOkButtonShortcut() const
{
    return GameStrings::c_OkButtonShortcut;
}

QString GamePresenter::getSubmitButtonShortcut() const
{
    return GameStrings::c_SubmitButtonShortcut;
}

QString GamePresenter::getResultsButtonShortcut() const
{
    return GameStrings::c_ResultsButtonShortcut;
}

QString GamePresenter::getResetButtonShortcut() const
{
    return GameStrings::c_ResetShortcut;
}

QString GamePresenter::getLevelEasyButtonShortcut() const
{
    return GameStrings::c_LevelEasyButtonShortcut;
}

QString GamePresenter::getLevelMediumButtonShortcut() const
{
    return GameStrings::c_LevelMediumButtonShortcut;
}

QString GamePresenter::getLevelHardButtonShortcut() const
{
    return GameStrings::c_LevelHardButtonShortcut;
}

QString GamePresenter::getCloseButtonShortcut() const
{
    return GameStrings::c_FatalErrorQuitButtonShortcut;
}

QList<QVariant> GamePresenter::getMixedWordsPieces() const
{
    QList<QVariant> mixedWordsPieces;
    QVector<QString> mixedWordsStringArray{m_pWordMixer->getMixedWordsStringArray()};
    for (auto piece : mixedWordsStringArray)
    {
        mixedWordsPieces.append(piece);
    }
    return mixedWordsPieces;
}

int GamePresenter::getFirstWordFirstPieceIndex() const
{
    return m_pWordMixer->getFirstWordFirstPieceIndex();
}

int GamePresenter::getFirstWordLastPieceIndex() const
{
    return m_pWordMixer->getFirstWordLastPieceIndex();
}

int GamePresenter::getSecondWordFirstPieceIndex() const
{
    return m_pWordMixer->getSecondWordFirstPieceIndex();
}

int GamePresenter::getSecondWordLastPieceIndex() const
{
    return m_pWordMixer->getSecondWordLastPieceIndex();
}

void GamePresenter::_onStatisticsUpdated()
{
    m_MainPaneScoreMessage = GameStrings::c_HighscoresMessage.arg(m_pScoreItem->getObtainedScore())
                                                             .arg(m_pScoreItem->getTotalAvailableScore());
    m_MainPaneWordPairsMessage = GameStrings::c_WordPairsMessage.arg(m_pScoreItem->getGuessedWordPairs())
                                                                .arg(m_pScoreItem->getTotalWordPairs());

    Q_EMIT mainPaneScoreMessageChanged();
    Q_EMIT mainPaneWordPairsMessageChanged();
}

Game::StatusCodes GamePresenter::_checkWords(const QString &firstWord, const QString &secondWord)
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

void GamePresenter::_initMainPane()
{

    qDebug() << "=====================================================";
    qDebug() << "Main window initialized! First 2 words mixed";

    m_MainPaneInitialized = true;
    m_pWordMixer -> mixWords();
    Q_EMIT mixedWordsPiecesChanged();
}

void GamePresenter::_updateStatusMessage(Game::StatusCodes statusCode)
{
    switch (statusCode)
    {
    case Game::StatusCodes::SUCCESS:
        m_MainPaneStatusMessage = GameStrings::c_SuccessMessage.arg(m_pWordMixer->getFirstWord())
                                                               .arg(m_pWordMixer->getSecondWord())
                                                               .arg(m_pWordMixer->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
        break;
    case Game::StatusCodes::MISSING_WORDS:
        m_MainPaneStatusMessage = GameStrings::c_MissingWordsMessage;
        break;
    case Game::StatusCodes::INCORRECT_WORDS:
        m_MainPaneStatusMessage = GameStrings::c_IncorrectWordsMessage;
        break;
    case Game::StatusCodes::REQUESTED_BY_USER:
        m_MainPaneStatusMessage = GameStrings::c_RequestedByUserMessage.arg(m_pWordMixer->getFirstWord())
                                                                       .arg(m_pWordMixer->getSecondWord())
                                                                       .arg(m_pWordMixer->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
        break;
    case Game::StatusCodes::STATISTICS_RESET:
        m_MainPaneStatusMessage = GameStrings::c_ScoresResetMessage;
        break;
    case Game::StatusCodes::LEVEL_CHANGED:
        m_MainPaneStatusMessage = GameStrings::c_LevelChangedMessage;
    //reserved for future use
    default:
        ;
    }

    Q_EMIT mainPaneStatusMessageChanged();
}

void GamePresenter::_setLevel(Game::Level level)
{
    _updateStatusMessage(Game::StatusCodes::LEVEL_CHANGED);
    Q_EMIT levelChanged(level);
    m_pWordMixer -> mixWords();
    Q_EMIT mixedWordsPiecesChanged();
}

void GamePresenter::_launchErrorPane(const QString& errorMessage)
{
    qDebug() << "Error message: " << errorMessage;

    switch (m_CurrentPane) {
    case Pane::INTRO:
        m_IntroPaneVisible = false;
        Q_EMIT introPaneVisibleChanged();
        break;
    case Pane::HELP:
        m_HelpPaneVisible = false;
        Q_EMIT helpPaneVisibleChanged();
        break;
    case Pane::MAIN:
        m_MainPaneVisible = false;
        Q_EMIT mainPaneVisibleChanged();
        break;
    default:
        Q_ASSERT(static_cast<int>(m_CurrentPane) >= 0 && static_cast<int>(m_CurrentPane) < static_cast<int>(Pane::Nr_Of_Panes));
    }

    m_CurrentPane = Pane::ERROR;
    m_ErrorMessage = errorMessage;
    m_ErrorOccured = true;

    Q_EMIT errorMessageChanged();
    Q_EMIT errorOccuredChanged();
}
