#include <QGuiApplication>
#include <QVector>

#include "gamepresenter.h"
#include "../SystemFunctionality/Management/gamefacade.h"
#include "../SystemFunctionality/Utilities/gamestrings.h"
#include "../SystemFunctionality/Utilities/exceptions.h"
#include "../SystemFunctionality/Proxies/gameproxy.h"

static const QMap<GamePresenter::Pane, QString> c_WindowTitles
{
    {GamePresenter::Pane::INTRO, GameStrings::c_IntroWindowTitle},
    {GamePresenter::Pane::HELP, GameStrings::c_HelpWindowTitle},
    {GamePresenter::Pane::MAIN, GameStrings::c_MainWindowTitle},
    {GamePresenter::Pane::ERROR, GameStrings::c_FatalErrorWindowTitle}
};

static const QMap<Game::PieceTypes, QColor> c_WordPieceTextColors
{
    {Game::PieceTypes::BEGIN_PIECE, GameStrings::c_WordFirstPieceTextColor},
    {Game::PieceTypes::MIDDLE_PIECE, GameStrings::c_WordMiddlePieceTextColor},
    {Game::PieceTypes::END_PIECE, GameStrings::c_WordLastPieceTextColor}
};

GamePresenter::GamePresenter(QObject *parent)
    : QObject(parent)
    , m_IntroPaneVisible {true}
    , m_HelpPaneVisible {false}
    , m_MainPaneVisible {false}
    , m_MainPaneInitialized {false}
    , m_StatisticsResetEnabled {false}
    , m_ClearInputEnabled{false}
    , m_ErrorOccured {false}
    , m_WindowTitle{GameStrings::c_IntroWindowTitle}
    , m_CurrentPane {Pane::INTRO}
    , m_PreviousPane{Pane::INTRO}
    , m_FirstWordInputHoverIndex{-1}
    , m_SecondWordInputHoverIndex{-1}
    , m_pGameFacade{nullptr}
    , m_pGameProxy {new GameProxy{this}}
    , m_pStatusUpdateTimer {new QTimer{this}}
{
    m_pGameFacade = m_pGameProxy->getFacade();
    Q_ASSERT(m_pGameFacade);

    bool connected{connect(m_pGameFacade, &GameFacade::statisticsChanged, this, &GamePresenter::_onStatisticsChanged)};
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::statusChanged, this, &GamePresenter::_onStatusChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::mixedWordsChanged, this, &GamePresenter::mixedWordsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::dataAvailableChanged, this, &GamePresenter::playEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::inputChanged, this, &GamePresenter::_onInputChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::selectionChanged, this, &GamePresenter::selectionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::completionChanged, this, &GamePresenter::submitEnabledChanged);
    Q_ASSERT(connected);

    m_pGameFacade->init();
}

void GamePresenter::switchToPane(Pane pane)
{
    Q_ASSERT(static_cast<int>(pane) >= 0 && static_cast<int>(pane) < static_cast<int>(Pane::Nr_Of_Panes));
    Q_ASSERT(!(m_CurrentPane == Pane::MAIN && pane == Pane::INTRO));

    if (m_CurrentPane != pane)
    {
        try
        {
            switch(m_CurrentPane)
            {
            case Pane::INTRO:
                m_IntroPaneVisible = false;
                break;
            case Pane::HELP:
                m_HelpPaneVisible = false;
                break;
            case Pane::MAIN:
                m_MainPaneVisible = false;
                break;
            default:
                Q_ASSERT(false);
            }

            switch(pane)
            {
            case Pane::INTRO:
                m_IntroPaneVisible = true;
                break;
            case Pane::HELP:
                m_HelpPaneVisible = true;
                break;
            case Pane::MAIN:
                if (!m_MainPaneInitialized)
                {
                    m_MainPaneInitialized = true;
                    m_pGameFacade -> startGame();
                }
                else
                {
                    m_pGameFacade -> resumeGame();
                }

                m_MainPaneVisible = true;
                break;
            default:
                Q_ASSERT(false);
            }

            m_PreviousPane = m_CurrentPane;
            m_CurrentPane = pane;

            Q_EMIT currentPaneChanged();
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
    try
    {
        m_pGameFacade->provideResultsToUser();
    }
    catch (const GameException& exception)
    {
        _launchErrorPane(exception.getDescription());
    }
}

void GamePresenter::handleSubmitRequest()
{
    try
    {
        m_pGameFacade->handleSubmitRequest();
    }
    catch(const GameException& exception)
    {
        _launchErrorPane(exception.getDescription());
    }
}

void GamePresenter::handleResetRequest()
{
    m_pGameFacade -> resetStatistics();
}

void GamePresenter::switchToLevel(int level)
{
    try
    {
        Q_ASSERT(level >= 0 && level < static_cast<int>(Game::Level::NrOfLevels));
        m_pGameFacade->setLevel(static_cast<Game::Level>(level));
    }
    catch (const GameException& exception)
    {
        _launchErrorPane(exception.getDescription());
    }
}

void GamePresenter::selectWordPieceForFirstInputWord(int wordPieceIndex)
{
    m_pGameFacade->addWordPieceToInputWord(Game::InputWordNumber::ONE, wordPieceIndex);
}

void GamePresenter::selectWordPieceForSecondInputWord(int wordPieceIndex)
{
    m_pGameFacade->addWordPieceToInputWord(Game::InputWordNumber::TWO, wordPieceIndex);
}

void GamePresenter::removeWordPiecesFromFirstInputWord(int inputRangeStart)
{
    m_pGameFacade->removeWordPiecesFromInputWord(Game::InputWordNumber::ONE, inputRangeStart);
}

void GamePresenter::removeWordPiecesFromSecondInputWord(int inputRangeStart)
{
    m_pGameFacade->removeWordPiecesFromInputWord(Game::InputWordNumber::TWO, inputRangeStart);
}

void GamePresenter::clearInput()
{
    m_pGameFacade->clearInput();
}

void GamePresenter::clearFirstInputWord()
{
    if (!m_pGameFacade->getFirstWordInputIndexes().empty())
    {
        m_pGameFacade->removeWordPiecesFromInputWord(Game::InputWordNumber::ONE, 0);
    }
}

void GamePresenter::clearSecondInputWord()
{
    if (!m_pGameFacade->getSecondWordInputIndexes().empty())
    {
        m_pGameFacade->removeWordPiecesFromInputWord(Game::InputWordNumber::TWO, 0);
    }
}

void GamePresenter::updateFirstWordInputHoverIndex(int index)
{
    m_FirstWordInputHoverIndex = index;
    Q_EMIT hoverChanged();
}

void GamePresenter::updateSecondWordInputHoverIndex(int index)
{
    m_SecondWordInputHoverIndex = index;
    Q_EMIT hoverChanged();
}

void GamePresenter::clearWordInputHoverIndexes()
{
    m_FirstWordInputHoverIndex = -1;
    m_SecondWordInputHoverIndex = -1;

    Q_EMIT hoverChanged();
}

GamePresenter::Pane GamePresenter::getPreviousPane() const
{
    return m_PreviousPane;
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

bool GamePresenter::isPlayEnabled() const
{
    return m_pGameFacade->isDataAvailable();
}

bool GamePresenter::getResetEnabled() const
{
    return m_StatisticsResetEnabled;
}

bool GamePresenter::getClearInputEnabled() const
{
    return m_ClearInputEnabled;
}

bool GamePresenter::getSubmitEnabled() const
{
    return m_pGameFacade->isInputComplete();
}

bool GamePresenter::getErrorOccured() const
{
    return m_ErrorOccured;
}

QList<QVariant> GamePresenter::getMixedWordsPiecesContent() const
{
    QList<QVariant> mixedWordsPiecesContent;

    for (auto pieceContent : m_pGameFacade->getMixedWordsPiecesContent())
    {
        mixedWordsPiecesContent.append(pieceContent);
    }

    return mixedWordsPiecesContent;
}

QList<QVariant> GamePresenter::getMixedWordsPiecesTextColors() const
{
    QList<QVariant> mixedWordsPiecesTextColors;

    for (auto pieceType : m_pGameFacade->getMixedWordsPiecesTypes())
    {
        mixedWordsPiecesTextColors.append(c_WordPieceTextColors[pieceType]);
    }

    return mixedWordsPiecesTextColors;
}

QList<QVariant> GamePresenter::getMixedWordsPiecesSelections() const
{
    QList<QVariant> mixedWordsPiecesSelections;

    for (auto isPieceSelected : m_pGameFacade->getAreMixedWordsPiecesSelected())
    {
        mixedWordsPiecesSelections.append(isPieceSelected);
    }

    return mixedWordsPiecesSelections;
}

QList<QVariant> GamePresenter::getFirstWordInputPiecesContent() const
{
    QList<QVariant> firstWordInputPiecesContent;

    for (auto index : m_pGameFacade->getFirstWordInputIndexes())
    {
        firstWordInputPiecesContent.append(m_pGameFacade->getMixedWordsPiecesContent().at(index));
    }

    return firstWordInputPiecesContent;
}

QList<QVariant> GamePresenter::getFirstWordInputPiecesTextColors() const
{
    QList<QVariant> firstWordInputPiecesTextColors;

    for (auto index : m_pGameFacade->getFirstWordInputIndexes())
    {
        firstWordInputPiecesTextColors.append(c_WordPieceTextColors[m_pGameFacade->getMixedWordsPiecesTypes().at(index)]);
    }

    return firstWordInputPiecesTextColors;
}

int GamePresenter::getFirstWordInputHoverIndex() const
{
    return m_FirstWordInputHoverIndex;
}

bool GamePresenter::getIsFirstWordInputHovered() const
{
    return (m_FirstWordInputHoverIndex != -1);
}

QList<QVariant> GamePresenter::getSecondWordInputPiecesContent() const
{
    QList<QVariant> secondWordInputPiecesContent;

    for (auto index : m_pGameFacade->getSecondWordInputIndexes())
    {
        secondWordInputPiecesContent.append(m_pGameFacade->getMixedWordsPiecesContent().at(index));
    }

    return secondWordInputPiecesContent;
}

QList<QVariant> GamePresenter::getSecondWordInputPiecesTextColors() const
{
    QList<QVariant> secondWordInputPiecesTextColors;

    for (auto index : m_pGameFacade->getSecondWordInputIndexes())
    {
        secondWordInputPiecesTextColors.append(c_WordPieceTextColors[m_pGameFacade->getMixedWordsPiecesTypes().at(index)]);
    }

    return secondWordInputPiecesTextColors;
}

bool GamePresenter::getIsSecondWordInputHovered() const
{
    return (m_SecondWordInputHoverIndex != -1);
}

int GamePresenter::getSecondWordInputHoverIndex() const
{
    return m_SecondWordInputHoverIndex;
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

int GamePresenter::getToolTipDelay() const
{
    return Game::c_ToolTipDelay;
}

int GamePresenter::getToolTipTimeout() const
{
    return Game::c_ToolTipTimeout;
}

QString GamePresenter::getWindowTitle() const
{
    return c_WindowTitles[m_CurrentPane];
}

QString GamePresenter::getIntroPaneMessage() const
{
    return m_IntroPaneMessage;
}

QString GamePresenter::getHelpPaneMessage() const
{
    return GameStrings::c_HelpWindowMessage;
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

GamePresenter::~GamePresenter()
{
    m_pGameProxy->releaseResources();
}

void GamePresenter::_onInputChanged()
{
    clearWordInputHoverIndexes();

    if ((m_pGameFacade->getFirstWordInputIndexes().size() != 0 || m_pGameFacade->getSecondWordInputIndexes().size() != 0) && !m_ClearInputEnabled)
    {
        m_ClearInputEnabled = true;
        Q_EMIT clearInputEnabledChanged();
    }
    else if ((m_pGameFacade->getFirstWordInputIndexes().size() == 0 && m_pGameFacade->getSecondWordInputIndexes().size() == 0) && m_ClearInputEnabled)
    {
        m_ClearInputEnabled = false;
        Q_EMIT clearInputEnabledChanged();
    }

    Q_EMIT inputChanged();
}

void GamePresenter::_onStatisticsChanged()
{
    int obtainedScore{m_pGameFacade->getObtainedScore()};
    int totalAvailableScore{m_pGameFacade->getTotalAvailableScore()};
    int guessedWordPairs{m_pGameFacade->getGuessedWordPairs()};
    int totalWordPairs{m_pGameFacade->getTotalWordPairs()};

    bool emptyStatistics{obtainedScore == 0 && totalAvailableScore == 0 && guessedWordPairs == 0 && totalWordPairs == 0};

    if (!m_StatisticsResetEnabled && !emptyStatistics)
    {
        m_StatisticsResetEnabled = true;
        Q_EMIT resetEnabledChanged();
    }
    else if (m_StatisticsResetEnabled && emptyStatistics)
    {
        m_StatisticsResetEnabled = false;
        Q_EMIT resetEnabledChanged();
    }

    m_MainPaneScoreMessage = GameStrings::c_HighscoresMessage.arg(obtainedScore)
                                                             .arg(totalAvailableScore);
    m_MainPaneWordPairsMessage = GameStrings::c_WordPairsMessage.arg(guessedWordPairs)
                                                                .arg(totalWordPairs);

    Q_EMIT mainPaneStatisticsMessagesChanged();
}

void GamePresenter::_onStatusChanged(Game::StatusCodes statusCode)
{
    if (m_pStatusUpdateTimer->isActive())
    {
        m_pStatusUpdateTimer->stop();
    }

    switch (statusCode)
    {
    case Game::StatusCodes::LOADING_DATA:
        _updateStatusMessage(GameStrings::c_DataLoadingMessage, Pane::INTRO);
        break;
    case Game::StatusCodes::DATA_LOAD_COMPLETE:
        _updateStatusMessage(GameStrings::c_DataReadyMessage, Pane::INTRO);
        break;
    case Game::StatusCodes::GAME_STARTED:
        _updateStatusMessage(GameStrings::c_GameStartedMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::GAME_RESUMED_COMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_GameResumedMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_AllPiecesAddedMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::GAME_RESUMED_INCOMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_GameResumedMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::SUCCESS:
        {
            QString message{GameStrings::c_SuccessMessage.arg(m_pGameFacade->getFirstReferenceWord())
                                                         .arg(m_pGameFacade->getSecondReferenceWord())
                                                         .arg(m_pGameFacade->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms)};
            _updateStatusMessage(message, Pane::MAIN);
        }
        QTimer::singleShot(Game::c_ExtStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::INCORRECT_WORDS:
        _updateStatusMessage(GameStrings::c_IncorrectWordsMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_AllPiecesAddedMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::REQUESTED_BY_USER:
        {
            QString message = GameStrings::c_RequestedByUserMessage.arg(m_pGameFacade->getFirstReferenceWord())
                                                                   .arg(m_pGameFacade->getSecondReferenceWord())
                                                                   .arg(m_pGameFacade->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
            _updateStatusMessage(message, Pane::MAIN);
        }
        QTimer::singleShot(Game::c_ExtStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::STATISTICS_RESET_COMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_ScoresResetMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_AllPiecesAddedMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::STATISTICS_RESET_INCOMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_ScoresResetMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::LEVEL_CHANGED:
        _updateStatusMessage(GameStrings::c_LevelChangedMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::PIECE_NOT_ADDED:
        _updateStatusMessage(GameStrings::c_PieceNotAddedMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::PIECE_ADDED_COMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_PieceSuccessfullyAddedMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_AllPiecesAddedMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::PIECE_ADDED_INCOMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_PieceSuccessfullyAddedMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::PIECES_REMOVED:
        _updateStatusMessage(GameStrings::c_PiecesRemovedMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    case Game::StatusCodes::USER_INPUT_CLEARED:
        _updateStatusMessage(GameStrings::c_AllPiecesRemovedMessage, Pane::MAIN);
        QTimer::singleShot(Game::c_StdStatusUpdateTimeout, m_pStatusUpdateTimer, [this](){_updateStatusMessage(GameStrings::c_DefaultStatusMessage, Pane::MAIN);});
        break;
    default:
        Q_ASSERT(false);
    }
}

void GamePresenter::_updateStatusMessage(const QString &message, GamePresenter::Pane pane)
{
    switch(pane)
    {
    case Pane::INTRO:
        m_IntroPaneMessage = message;
        Q_EMIT introPaneMessageChanged();
        break;
    case Pane::HELP:
        break;
    case Pane::MAIN:
        m_MainPaneStatusMessage = message;
        Q_EMIT mainPaneStatusMessageChanged();
        break;
    case Pane::ERROR:
        break;
    default:
        Q_ASSERT(static_cast<int>(m_CurrentPane) >= 0 && static_cast<int>(m_CurrentPane) < static_cast<int>(Pane::Nr_Of_Panes));
    }
}

void GamePresenter::_launchErrorPane(const QString& errorMessage)
{
    switch (m_CurrentPane) {
    case Pane::INTRO:
        m_IntroPaneVisible = false;
        Q_EMIT currentPaneChanged();
        break;
    case Pane::HELP:
        m_HelpPaneVisible = false;
        Q_EMIT currentPaneChanged();
        break;
    case Pane::MAIN:
        m_MainPaneVisible = false;
        Q_EMIT currentPaneChanged();
        break;
    default:
        Q_ASSERT(static_cast<int>(m_CurrentPane) >= 0 && static_cast<int>(m_CurrentPane) < static_cast<int>(Pane::Nr_Of_Panes));
    }

    m_CurrentPane = Pane::ERROR;
    m_ErrorMessage = errorMessage;
    m_ErrorOccured = true;

    Q_EMIT errorMessageChanged();
    Q_EMIT errorOccuredChanged();
    Q_EMIT windowTitleChanged();
}
