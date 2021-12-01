#include <QGuiApplication>
#include <QVector>

#include "gamepresenter.h"
#include "dataentrypresenter.h"
#include "gamestrings.h"
#include "dataentrystrings.h"
#include "gamecolors.h"
#include "timing.h"
#include "gamefacade.h"
#include "exceptions.h"
#include "gameproxy.h"

static const QMap<GamePresenter::Panes, QString> c_WindowTitles
{
    {GamePresenter::Panes::INTRO_PANE, GameStrings::Titles::c_IntroWindowTitle},
    {GamePresenter::Panes::HELP_PANE, GameStrings::Titles::c_HelpWindowTitle},
    {GamePresenter::Panes::MAIN_PANE, GameStrings::Titles::c_MainWindowTitle},
    {GamePresenter::Panes::DATA_ENTRY_PANE, GameStrings::Titles::c_DataEntryWindowTitle},
    {GamePresenter::Panes::PROMPT_SAVE_EXIT_PANE, GameStrings::Titles::c_PromptSaveExitWindowTitle},
    {GamePresenter::Panes::PROMPT_DISCARD_PANE, GameStrings::Titles::c_PromptDiscardWindowTitle},
    {GamePresenter::Panes::ERROR_PANE, GameStrings::Titles::c_FatalErrorWindowTitle}
};

static const QMap<Game::PieceTypes, QColor> c_WordPieceTextColors
{
    {Game::PieceTypes::BEGIN_PIECE, Colors::c_BeginPieceTextColor},
    {Game::PieceTypes::MIDDLE_PIECE, Colors::c_MiddlePieceTextColor},
    {Game::PieceTypes::END_PIECE, Colors::c_EndPieceTextColor}
};

static constexpr int c_PaneSwitchingDelay{350};
static constexpr int c_GameQuitDelay{200};

static_assert (static_cast<int>(GamePresenter::Levels::LEVEL_EASY) == static_cast<int>(Game::Levels::LEVEL_EASY) &&
               static_cast<int>(GamePresenter::Levels::LEVEL_MEDIUM) == static_cast<int>(Game::Levels::LEVEL_MEDIUM) &&
               static_cast<int>(GamePresenter::Levels::LEVEL_HARD) == static_cast<int>(Game::Levels::LEVEL_HARD) &&
               static_cast<int>(GamePresenter::Levels::LEVEL_NONE) == static_cast<int>(GamePresenter::Levels::LEVEL_NONE), "Incorrect mapping between GamePresenter::Levels and Game::Levels");

GamePresenter::GamePresenter(QObject *parent)
    : QObject(parent)
    , m_pDataEntryPresenter{new DataEntryPresenter{this}}
    , m_MainPaneInitialized {false}
    , m_IsDataEntryHelpMenuActive{false}
    , m_MainPaneStatisticsResetEnabled {false}
    , m_ClearMainPaneInputEnabled{false}
    , m_ErrorOccured {false}
    , m_QuitDeferred{false}
    , m_WindowTitle{GameStrings::Titles::c_IntroWindowTitle}
    , m_CurrentPane {Panes::INTRO_PANE}
    , m_StatusUpdatePane{Panes::INTRO_PANE}
    , m_PreviousPanesStack{}
    , m_FirstWordInputPiecesHoverIndex{-1}
    , m_SecondWordInputPiecesHoverIndex{-1}
    , m_pGameFacade{nullptr}
    , m_pGameProxy {new GameProxy{this}}
    , m_pStatusUpdateTimer {new QTimer{this}}
{
    m_pGameFacade = m_pGameProxy->getGameFacade();
    Q_ASSERT(m_pGameFacade);

    m_pStatusUpdateTimer->setSingleShot(true);

    auto connected{connect(m_pGameFacade, &GameFacade::statisticsChanged, this, &GamePresenter::_onStatisticsChanged)};
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::statusChanged, this, &GamePresenter::_onStatusChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::newMixedWordsAvailable, this, &GamePresenter::mixedWordsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::dataAvailableChanged, this, &GamePresenter::playEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::inputChanged, this, &GamePresenter::_onInputChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::piecesAddedToInputChanged, this, &GamePresenter::selectionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::completionChanged, this, &GamePresenter::submitMainPaneInputEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::languageChanged, this, &GamePresenter::languageChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::timeLimitEnabledChanged, this, &GamePresenter::timeLimitEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::remainingTimeRefreshed, this, &GamePresenter::remainingTimeChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::persistentIndexModeEnabledChanged, this, &GamePresenter::persistentModeEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::persistentPieceSelectionIndexChanged, this, &GamePresenter::pieceSelectionCursorPositionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::persistentPiecesRemovalIndexesChanged, this, &GamePresenter::piecesRemovalCursorPositionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::fetchingInProgressChanged, this, &GamePresenter::dataFetchingInProgressChanged);
    Q_ASSERT(connected);
    connected = connect(m_pStatusUpdateTimer, &QTimer::timeout, this, &GamePresenter::_updateMessage);
    Q_ASSERT(connected);
    connected = connect(qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter), &DataEntryPresenter::dataSaveInProgress, this, &GamePresenter::_onDataSaveInProgress);
    Q_ASSERT(connected);

    m_pGameFacade->init();
}

void GamePresenter::setCurrentPane(GamePresenter::Panes pane)
{
    if (m_CurrentPane != pane)
    {
        m_PreviousPanesStack.append(m_CurrentPane);
        _switchToPane(pane);

        if (pane == GamePresenter::Panes::DATA_ENTRY_PANE)
        {
            qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->startDataEntry();
        }
    }
}

void GamePresenter::goBack()
{
    Panes previousPane{m_PreviousPanesStack.last()};
    m_PreviousPanesStack.pop_back();

    if (m_QuitDeferred)
    {
        m_QuitDeferred = false;
        Q_EMIT quitGameDeferredChanged();
    }

    _switchToPane(previousPane);
}

void GamePresenter::promptForSavingAddedWordPairs()
{
    Q_ASSERT(m_CurrentPane == Panes::DATA_ENTRY_PANE || (m_CurrentPane == Panes::HELP_PANE && m_PreviousPanesStack.last() == Panes::DATA_ENTRY_PANE));

    m_PreviousPanesStack.append(m_CurrentPane);
    m_CurrentPane = Panes::PROMPT_SAVE_EXIT_PANE;

    Q_EMIT currentPaneChanged();
}

void GamePresenter::promptForDiscardingAddedWordPairs()
{
    Q_ASSERT(m_CurrentPane == Panes::DATA_ENTRY_PANE);

    m_PreviousPanesStack.append(m_CurrentPane);
    m_CurrentPane = Panes::PROMPT_DISCARD_PANE;

    Q_EMIT currentPaneChanged();
}

void GamePresenter::handleDisplayCorrectWordsPairRequest()
{
    m_pGameFacade->provideCorrectWordsPairToUser();
}

void GamePresenter::handleSubmitMainPaneInputRequest()
{
    m_pGameFacade->handleSubmitRequest();
}

void GamePresenter::handleMainPaneStatisticsResetRequest()
{
    m_pGameFacade -> resetGameStatistics();
}

void GamePresenter::setLevel(GamePresenter::Levels level)
{
    Q_ASSERT(static_cast<int>(level) >= 0 && static_cast<int>(level) < static_cast<int>(Game::Levels::LEVEL_NONE));
    m_pGameFacade->setGameLevel(static_cast<Game::Levels>(level));
}

void GamePresenter::handleLanguageChangeRequest(int newLanguageIndex, bool revertLanguageWhenDataUnavailable)
{
    Q_ASSERT(newLanguageIndex >= 0); // update it once the languages are better setup in backend (beyond facade)
    m_pGameFacade->setLanguage(newLanguageIndex, revertLanguageWhenDataUnavailable);
}

void GamePresenter::enableCursor()
{
    m_pGameFacade->enablePersistentMode();
}

void GamePresenter::disableCursor()
{
    m_pGameFacade->disablePersistentMode();
}

void GamePresenter::moveCursorToNextContainer()
{
    m_pGameFacade->goToNextPersistentModeContainer();
}

void GamePresenter::moveCursorLeft()
{
    m_pGameFacade->decreasePersistentIndex();
}

void GamePresenter::moveCursorRight()
{
    m_pGameFacade->increasePersistentIndex();
}

void GamePresenter::executeFirstCursorAction()
{
    m_pGameFacade->executeFirstPersistentModeAction();
}

void GamePresenter::executeSecondCursorAction()
{
    m_pGameFacade->executeSecondPersistentModeAction();
}

void GamePresenter::setTimeLimitEnabled(bool enabled)
{
    if (enabled)
    {
        m_pGameFacade->enableTimeLimit();
    }
    else
    {
        m_pGameFacade->disableTimeLimit();
    }
}

void GamePresenter::selectPieceForFirstInputWord(int wordPieceIndex)
{
    m_pGameFacade->addPieceToInputWord(Game::InputWordNumber::ONE, wordPieceIndex);
}

void GamePresenter::selectPieceForSecondInputWord(int wordPieceIndex)
{
    m_pGameFacade->addPieceToInputWord(Game::InputWordNumber::TWO, wordPieceIndex);
}

void GamePresenter::removePiecesFromFirstInputWord(int inputRangeStart)
{
    m_pGameFacade->removePiecesFromInputWord(Game::InputWordNumber::ONE, inputRangeStart);
}

void GamePresenter::removePiecesFromSecondInputWord(int inputRangeStart)
{
    m_pGameFacade->removePiecesFromInputWord(Game::InputWordNumber::TWO, inputRangeStart);
}

void GamePresenter::clearMainPaneInput()
{
    m_pGameFacade->clearInput();
}

void GamePresenter::clearMainPaneFirstInputWord()
{
    if (!m_pGameFacade->getFirstWordInputIndexes().empty())
    {
        m_pGameFacade->clearInputWord(Game::InputWordNumber::ONE);
    }
}

void GamePresenter::clearMainPaneSecondInputWord()
{
    if (!m_pGameFacade->getSecondWordInputIndexes().empty())
    {
        m_pGameFacade->clearInputWord(Game::InputWordNumber::TWO);
    }
}

void GamePresenter::updateFirstWordInputHoverIndex(int index)
{
    m_FirstWordInputPiecesHoverIndex = index;
    Q_EMIT hoverChanged();
}

void GamePresenter::updateSecondWordInputHoverIndex(int index)
{
    m_SecondWordInputPiecesHoverIndex = index;
    Q_EMIT hoverChanged();
}

void GamePresenter::clearWordInputHoverIndexes()
{
    m_FirstWordInputPiecesHoverIndex = -1;
    m_SecondWordInputPiecesHoverIndex = -1;

    Q_EMIT hoverChanged();
}

void GamePresenter::quit()
{
    // involve facade only if the user accessed the main pane
    if (m_MainPaneInitialized)
    {
        m_pGameFacade->quitGame();
    }
    else
    {
        QGuiApplication::quit();
    }
}

void GamePresenter::handleSaveAndQuit()
{
    if (m_QuitDeferred)
    {
        _switchToPane(Panes::DATA_ENTRY_PANE);
        qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->handleSaveAddedWordPairsRequest();
    }
}

QObject *GamePresenter::getDataEntryPresenter() const
{
    return m_pDataEntryPresenter;
}

GamePresenter::Panes GamePresenter::getCurrentPane() const
{
    return m_CurrentPane;
}

bool GamePresenter::isLanguageSelectionEnabled() const
{
    return true; // to be updated
}

bool GamePresenter::isPlayEnabled() const
{
    return m_pGameFacade->isDataAvailable();
}

bool GamePresenter::getMainPaneStatisticsResetEnabled() const
{
    return m_MainPaneStatisticsResetEnabled;
}

bool GamePresenter::isPersistentModeEnabled() const
{
    return m_pGameFacade->isPersistentModeEnabled();
}

bool GamePresenter::isTimeLimitEnabled() const
{
    return m_pGameFacade->isTimeLimitEnabled();
}

bool GamePresenter::getClearMainPaneInputEnabled() const
{
    return m_ClearMainPaneInputEnabled;
}

bool GamePresenter::getSubmitMainPaneInputEnabled() const
{
    return m_pGameFacade->isInputComplete();
}

bool GamePresenter::getDataFetchingInProgress() const
{
    return m_pGameFacade->isDataFetchingInProgress();
}

bool GamePresenter::getErrorOccured() const
{
    return m_ErrorOccured;
}

bool GamePresenter::getQuitGameDeferred() const
{
    return m_QuitDeferred;
}

void GamePresenter::setQuitGameDeferred(bool deferred)
{
    if (m_QuitDeferred != deferred)
    {
        m_QuitDeferred = deferred;
        Q_EMIT quitGameDeferredChanged();
    }
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

int GamePresenter::getFirstWordInputPiecesHoverIndex() const
{
    return m_FirstWordInputPiecesHoverIndex;
}

bool GamePresenter::getAreFirstWordInputPiecesHovered() const
{
    return (m_FirstWordInputPiecesHoverIndex != -1);
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

bool GamePresenter::getAreSecondWordInputPiecesHovered() const
{
    return (m_SecondWordInputPiecesHoverIndex != -1);
}

int GamePresenter::getSecondWordInputPiecesHoverIndex() const
{
    return m_SecondWordInputPiecesHoverIndex;
}

int GamePresenter::getPieceSelectionCursorPosition() const
{
    return m_pGameFacade->getPersistentPieceSelectionIndex();
}

int GamePresenter::getPiecesRemovalFirstWordCursorPosition() const
{
    return m_pGameFacade->getFirstPersistentPiecesRemovalIndex();
}

int GamePresenter::getPiecesRemovalSecondWordCursorPosition() const
{
    return m_pGameFacade->getSecondPersistentPiecesRemovalIndex();
}

int GamePresenter::getLanguageIndex() const
{
    return m_pGameFacade->getCurrentLanguageIndex();
}

QStringList GamePresenter::getRemainingTime() const
{
    QPair<QString, QString> currentTimeMinSec{m_pGameFacade->getRemainingTime()};
    return QStringList{} << currentTimeMinSec.first << currentTimeMinSec.second;
}

QString GamePresenter::getWindowTitle() const
{
    return (m_CurrentPane == Panes::HELP_PANE ? (c_WindowTitles[m_CurrentPane].arg(m_IsDataEntryHelpMenuActive ? DataEntryStrings::Descriptors::c_DataEntryDescriptor
                                                                                                         : GameStrings::Descriptors::c_GameDescriptor))
                                        : c_WindowTitles[m_CurrentPane]);
}

QString GamePresenter::getIntroPaneMessage() const
{
    return m_IntroPaneMessage;
}

QString GamePresenter::getHelpPaneMessage() const
{
    return (m_IsDataEntryHelpMenuActive ? DataEntryStrings::Messages::c_DataEntryHelpMessage : GameStrings::Messages::c_GameHelpMessage);
}

QString GamePresenter::getHelpPaneToolTip() const
{
    return (GameStrings::Messages::c_GameHelpPaneToolTip.arg((m_IsDataEntryHelpMenuActive ? DataEntryStrings::Descriptors::c_DataEntryDescriptor
                                                                                   : GameStrings::Descriptors::c_GameDescriptor).toLower()));
}

QString GamePresenter::getHelpButtonToolTip() const
{
    return (GameStrings::Messages::c_GameHelpButtonToolTip.arg((m_CurrentPane == Panes::DATA_ENTRY_PANE ? DataEntryStrings::Descriptors::c_DataEntryDescriptor
                                                                                           : GameStrings::Descriptors::c_GameDescriptor).toLower()));
}

QString GamePresenter::getMainPaneStatusMessage() const
{
    return m_MainPaneStatusMessage;
}

QString GamePresenter::getObtainedScore() const
{
    return m_pGameFacade->getObtainedScore();
}

QString GamePresenter::getTotalAvailableScore() const
{
    return m_pGameFacade->getTotalAvailableScore();
}

QString GamePresenter::getGuessedWordPairs() const
{
    return m_pGameFacade->getGuessedWordPairs();
}

QString GamePresenter::getTotalWordPairs() const
{
    return m_pGameFacade->getTotalWordPairs();
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

    if ((m_pGameFacade->getFirstWordInputIndexes().size() != 0 || m_pGameFacade->getSecondWordInputIndexes().size() != 0) && !m_ClearMainPaneInputEnabled)
    {
        m_ClearMainPaneInputEnabled = true;
        Q_EMIT clearMainPaneInputEnabledChanged();
    }
    else if ((m_pGameFacade->getFirstWordInputIndexes().size() == 0 && m_pGameFacade->getSecondWordInputIndexes().size() == 0) && m_ClearMainPaneInputEnabled)
    {
        m_ClearMainPaneInputEnabled = false;
        Q_EMIT clearMainPaneInputEnabledChanged();
    }

    Q_EMIT inputChanged();
}

void GamePresenter::_onStatisticsChanged()
{
    bool canReset{m_pGameFacade->canResetGameStatistics()};

    if (!m_MainPaneStatisticsResetEnabled && canReset)
    {
        m_MainPaneStatisticsResetEnabled = true;
        Q_EMIT mainPaneStatisticsResetEnabledChanged();
    }
    else if (m_MainPaneStatisticsResetEnabled && !canReset)
    {
        m_MainPaneStatisticsResetEnabled = false;
        Q_EMIT mainPaneStatisticsResetEnabledChanged();
    }

    Q_EMIT mainPaneStatisticsChanged();
}

void GamePresenter::_onDataSaveInProgress()
{
    m_pGameFacade->handleDataSavingOperationInProgress();
}

void GamePresenter::_onStatusChanged()
{
    GameFacade::StatusCodes statusCode{m_pGameFacade->getStatusCode()};

    // once main pane is accessed exclude handling the status codes that are only applicable for the intro pane (from status message display point of view) ...
    if (!m_MainPaneInitialized || (statusCode != GameFacade::StatusCodes::DATA_SUCCESSFULLY_SAVED &&
                                   statusCode != GameFacade::StatusCodes::ADDITIONAL_DATA_SAVE_IN_PROGRESS) ||
                                  (m_QuitDeferred && statusCode == GameFacade::StatusCodes::DATA_SUCCESSFULLY_SAVED)) // ...except this status when user quits and data save is pending
    {
        if (m_pStatusUpdateTimer->isActive())
        {
            m_pStatusUpdateTimer->stop();
        }

        switch (statusCode)
        {
        case GameFacade::StatusCodes::NO_LANGUAGE_SET:
            _updateStatusMessage(GameStrings::Messages::c_WelcomeMessage, Panes::INTRO_PANE, Timing::c_NoDelay);
            break;
        case GameFacade::StatusCodes::FETCHING_DATA:
            if (m_CurrentPane == Panes::INTRO_PANE || m_CurrentPane == Panes::MAIN_PANE)
            {
                _updateStatusMessage(GameStrings::Messages::c_FetchingDataMessage, m_CurrentPane, Timing::c_NoDelay);
            }
            break;
        case GameFacade::StatusCodes::DATA_FETCHING_COMPLETE:
            if (m_CurrentPane == Panes::INTRO_PANE)
            {
                _updateStatusMessage(GameStrings::Messages::c_LanguageChangedMessage, Panes::INTRO_PANE, Timing::c_NoDelay);
                _updateStatusMessage(GameStrings::Messages::c_PleasePlayOrEnterDataMessage, Panes::INTRO_PANE, Timing::c_ShortStatusUpdateDelay);
            }
            else if (m_CurrentPane == Panes::MAIN_PANE)
            {
                _updateStatusMessage(GameStrings::Messages::c_LanguageChangedMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
                _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            }
            break;
        case GameFacade::StatusCodes::NO_DATA_ENTRIES_FETCHED:
            if (m_CurrentPane == Panes::INTRO_PANE)
            {
                _updateStatusMessage(GameStrings::Messages::c_LanguageChangedMessage, Panes::INTRO_PANE, Timing::c_NoDelay);
                _updateStatusMessage(GameStrings::Messages::c_NoValidEntriesFetchedMessage, Panes::INTRO_PANE, Timing::c_ShortStatusUpdateDelay);
            }
            else if (m_CurrentPane == Panes::MAIN_PANE)
            {
                _updateStatusMessage(GameStrings::Messages::c_CannotChangeLanguageMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
                _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            }
            break;
        case GameFacade::StatusCodes::DATA_FETCHING_ERROR:
            _launchErrorPane(GameStrings::Error::c_CannotFetchDataMessage);
            break;
        case GameFacade::StatusCodes::DATA_GOT_AVAILABLE:
            if (m_QuitDeferred)
            {
                QTimer::singleShot(c_GameQuitDelay, this, [this](){quit();});
            }
            else
            {
                _updateStatusMessage(GameStrings::Messages::c_PleasePlayOrEnterDataMessage, Panes::INTRO_PANE, Timing::c_NoDelay);
            }
            break;
        case GameFacade::StatusCodes::DATA_STILL_UNAVAILABLE:
            _updateStatusMessage(GameStrings::Messages::c_NoValidEntriesSavedForGameLanguage, Panes::INTRO_PANE, Timing::c_NoDelay);
            break;
        case GameFacade::StatusCodes::GAME_STARTED:
            _updateStatusMessage(GameStrings::Messages::c_GameStartedMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::GAME_PAUSED:
            _updateStatusMessage(GameStrings::Messages::c_GamePausedMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            break;
        case GameFacade::StatusCodes::GAME_RESUMED_COMPLETE_INPUT:
            _updateStatusMessage(GameStrings::Messages::c_GameResumedMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_AllPiecesAddedMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::GAME_RESUMED_INCOMPLETE_INPUT:
            _updateStatusMessage(GameStrings::Messages::c_GameResumedMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::GAME_STOPPED:
            if (m_CurrentPane == Panes::MAIN_PANE)
            {
                _updateStatusMessage(GameStrings::Messages::c_GameStoppedMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
                QTimer::singleShot(c_GameQuitDelay, this, [](){QGuiApplication::quit();});
            }
            else
            {
                QGuiApplication::quit();
            }
            break;
        case GameFacade::StatusCodes::NEW_DATA_SAVE_IN_PROGRESS:
            _updateStatusMessage(GameStrings::Messages::c_NewDataSaveInProgressMessage, Panes::INTRO_PANE, Timing::c_NoDelay);
            break;
        case GameFacade::StatusCodes::ADDITIONAL_DATA_SAVE_IN_PROGRESS:
            _updateStatusMessage(GameStrings::Messages::c_AdditionalDataSaveInProgressMessage, Panes::INTRO_PANE, Timing::c_NoDelay);
            break;
        case GameFacade::StatusCodes::DATA_SUCCESSFULLY_SAVED:
            _updateStatusMessage(GameStrings::Messages::c_AdditionalDataAvailableMessage, Panes::INTRO_PANE, Timing::c_NoDelay);
            if (m_QuitDeferred)
            {
                QTimer::singleShot(c_GameQuitDelay, this, [this](){quit();});
            }
            else
            {
                _updateStatusMessage(GameStrings::Messages::c_PleasePlayOrEnterDataMessage, Panes::INTRO_PANE, Timing::c_ShortStatusUpdateDelay);
            }
            break;
        case GameFacade::StatusCodes::DATA_ENTRY_SAVING_ERROR:
            _launchErrorPane(GameStrings::Error::c_CannotSaveDataMessage);
            break;
        case GameFacade::StatusCodes::PIECE_NOT_ADDED:
            _updateStatusMessage(GameStrings::Messages::c_PieceNotAddedToInputMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::PIECE_ADDED_COMPLETE_INPUT:
            _updateStatusMessage(GameStrings::Messages::c_PieceAddedToInputMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_AllPiecesAddedMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::PIECE_ADDED_INCOMPLETE_INPUT:
            _updateStatusMessage(GameStrings::Messages::c_PieceAddedToInputMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::PIECES_REMOVED:
            _updateStatusMessage(GameStrings::Messages::c_PiecesRemovedFromInputMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::USER_INPUT_CLEARED:
            _updateStatusMessage(GameStrings::Messages::c_AllPiecesRemovedMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::CORRECT_USER_INPUT:
        {
            QString message{GameStrings::Messages::c_CorrectUserInputMessage.arg(m_pGameFacade->getFirstReferenceWord())
                        .arg(m_pGameFacade->getSecondReferenceWord())
                        .arg(m_pGameFacade->areWordsFromCurrentPairSynonyms() ? GameStrings::Descriptors::c_SynonymsDescriptor : GameStrings::Descriptors::c_AntonymsDescriptor)};
            _updateStatusMessage(message, Panes::MAIN_PANE, Timing::c_NoDelay);
        }
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_LongStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::INCORRECT_USER_INPUT:
            _updateStatusMessage(GameStrings::Messages::c_IncorrectUserInputMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_AllPiecesAddedMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::SOLUTION_REQUESTED_BY_USER:
        {
            QString message = GameStrings::Messages::c_ShowPairRequestedByUserMessage.arg(m_pGameFacade->getFirstReferenceWord())
                    .arg(m_pGameFacade->getSecondReferenceWord())
                    .arg(m_pGameFacade->areWordsFromCurrentPairSynonyms() ? GameStrings::Descriptors::c_SynonymsDescriptor : GameStrings::Descriptors::c_AntonymsDescriptor);
            _updateStatusMessage(message, Panes::MAIN_PANE, Timing::c_NoDelay);
        }
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_LongStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::STATISTICS_RESET_COMPLETE_INPUT:
            _updateStatusMessage(GameStrings::Messages::c_StatisticsResetMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_AllPiecesAddedMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::STATISTICS_RESET_INCOMPLETE_INPUT:
            _updateStatusMessage(GameStrings::Messages::c_StatisticsResetMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::LEVEL_CHANGED:
            _updateStatusMessage(GameStrings::Messages::c_LevelChangedMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::PERSISTENT_MODE_ENTERED:
            _updateStatusMessage(GameStrings::Messages::c_CursorModeEnabledMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::PERSISTENT_MODE_EXITED:
            _updateStatusMessage(GameStrings::Messages::c_CursorModeDisabledMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::PERSISTENT_INDEX_REQUIRED:
            _updateStatusMessage(GameStrings::Messages::c_ClickSelectOrRemoveDisabledMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::TIME_LIMIT_ENABLED:
            _updateStatusMessage(GameStrings::Messages::c_TimeLimitEnabledMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::TIME_LIMIT_DISABLED:
            _updateStatusMessage(GameStrings::Messages::c_TimeLimitDisabledMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        case GameFacade::StatusCodes::TIME_LIMIT_REACHED:
            _updateStatusMessage(GameStrings::Messages::c_TimeLimitReachedMessage, Panes::MAIN_PANE, Timing::c_NoDelay);
            _updateStatusMessage(GameStrings::Messages::c_SelectOrDeleteWordPiecesMessage, Panes::MAIN_PANE, Timing::c_ShortStatusUpdateDelay);
            break;
        default:
            Q_ASSERT(false);
        }
    }
}

void GamePresenter::_switchToPane(Panes pane)
{
    Q_ASSERT(static_cast<int>(pane) >= 0 && static_cast<int>(pane) < static_cast<int>(Panes::NrOfPanes));
    Q_ASSERT(!(m_CurrentPane == Panes::MAIN_PANE && pane == Panes::INTRO_PANE));

    bool delayedSwitchingRequested{false};
    bool isGamePausingRequired{false};

    auto triggerPaneSwitching = [this]()
    {
        Q_EMIT currentPaneChanged();
    };

    if (m_CurrentPane != pane)
    {
        // execute certain actions related to the current pane (that don't depend on future pane) here; also exclude certain panes (like error)
        switch(m_CurrentPane)
        {
        case Panes::INTRO_PANE:
            break;
        case Panes::HELP_PANE:
            break;
        case Panes::MAIN_PANE:
            isGamePausingRequired = true;
            break;
        case Panes::DATA_ENTRY_PANE:
            break;
        case Panes::PROMPT_DISCARD_PANE:
            break;
        case Panes::PROMPT_SAVE_EXIT_PANE:
            break;
        default:
            Q_ASSERT(false);
        }

        switch(pane)
        {
        case Panes::INTRO_PANE:
            if (m_CurrentPane == Panes::DATA_ENTRY_PANE)
            {
                qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->stopDataEntry();
                delayedSwitchingRequested = true;
            }
            break;
        case Panes::HELP_PANE:
            if (isGamePausingRequired)
            {
                m_pGameFacade->pauseGame();
                delayedSwitchingRequested = true;
            }
            _setDataEntryHelpMenuActive(m_CurrentPane == Panes::DATA_ENTRY_PANE);
            break;
        case Panes::MAIN_PANE:
            if (!m_MainPaneInitialized)
            {
                m_MainPaneInitialized = true;
                m_pGameFacade -> startGame();
            }
            else
            {
                if (m_CurrentPane == Panes::DATA_ENTRY_PANE)
                {
                    qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->stopDataEntry();
                    delayedSwitchingRequested = true;
                }
                m_pGameFacade -> resumeGame();
            }
            break;
        case Panes::DATA_ENTRY_PANE:
            if (isGamePausingRequired)
            {
                m_pGameFacade->pauseGame();
                delayedSwitchingRequested = true;
            }
            else if (m_CurrentPane == Panes::HELP_PANE || m_CurrentPane == Panes::PROMPT_SAVE_EXIT_PANE)
            {
                qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->resumeDataEntry();
            }
            break;
        default:
            Q_ASSERT(false);
        }

        m_CurrentPane = pane;

        if (delayedSwitchingRequested)
        {
            QTimer::singleShot(c_PaneSwitchingDelay, this, [triggerPaneSwitching](){triggerPaneSwitching();});
        }
        else
        {
            triggerPaneSwitching();
        }
    }
}

bool GamePresenter::_canResetGameStatistics() const
{
    return m_pGameFacade->canResetGameStatistics();
}

void GamePresenter::_setDataEntryHelpMenuActive(bool active)
{
    if (m_IsDataEntryHelpMenuActive != active)
    {
        m_IsDataEntryHelpMenuActive = active;
        Q_EMIT helpPaneContentChanged();
    }
}

void GamePresenter::_updateStatusMessage(const QString& message, GamePresenter::Panes pane, int delay)
{
    m_CurrentStatusMessage = message;
    m_StatusUpdatePane = pane;

    if (delay > 0)
    {
        m_pStatusUpdateTimer->start(delay);
    }
    else
    {
        _updateMessage();
    }
}

void GamePresenter::_updateMessage()
{
    switch(m_StatusUpdatePane)
    {
    case Panes::INTRO_PANE:
        m_IntroPaneMessage = m_CurrentStatusMessage;
        Q_EMIT introPaneMessageChanged();
        break;
    case Panes::HELP_PANE:
        break;
    case Panes::MAIN_PANE:
        m_MainPaneStatusMessage = m_CurrentStatusMessage;
        Q_EMIT mainPaneStatusMessageChanged();
        break;
    case Panes::DATA_ENTRY_PANE:
        // data entry pane status to be updated by the data entry presenter only
        Q_ASSERT(false);
    case Panes::ERROR_PANE:
        break;
    default:
        Q_ASSERT(static_cast<int>(m_StatusUpdatePane) >= 0 && static_cast<int>(m_StatusUpdatePane) < static_cast<int>(Panes::NrOfPanes));
    }
}

void GamePresenter::_launchErrorPane(const QString& errorMessage)
{
    m_CurrentPane = Panes::ERROR_PANE;
    m_ErrorMessage = errorMessage;
    m_ErrorOccured = true;

    Q_EMIT currentPaneChanged();
    Q_EMIT errorMessageChanged();
    Q_EMIT errorOccuredChanged();
}
