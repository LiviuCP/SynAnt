#include <QGuiApplication>
#include <QVector>

#include "gamepresenter.h"
#include "dataentrypresenter.h"
#include "../SystemFunctionality/Management/gamefacade.h"
#include "../SystemFunctionality/Utilities/exceptions.h"
#include "../SystemFunctionality/ManagementProxies/gameproxy.h"

static const QMap<GamePresenter::Pane, QString> c_WindowTitles
{
    {GamePresenter::Pane::INTRO, Game::Titles::c_IntroWindowTitle},
    {GamePresenter::Pane::HELP, Game::Titles::c_HelpWindowTitle},
    {GamePresenter::Pane::MAIN, Game::Titles::c_MainWindowTitle},
    {GamePresenter::Pane::DATA_ENTRY, Game::Titles::c_DataEntryWindowTitle},
    {GamePresenter::Pane::PROMPT_SAVE_EXIT, Game::Titles::c_PromptSaveExitWindowTitle},
    {GamePresenter::Pane::PROMPT_DISCARD, Game::Titles::c_PromptDiscardWindowTitle},
    {GamePresenter::Pane::ERROR, Game::Titles::c_FatalErrorWindowTitle}
};

static const QMap<Game::PieceTypes, QColor> c_WordPieceTextColors
{
    {Game::PieceTypes::BEGIN_PIECE, Game::Colors::c_BeginPieceTextColor},
    {Game::PieceTypes::MIDDLE_PIECE, Game::Colors::c_MiddlePieceTextColor},
    {Game::PieceTypes::END_PIECE, Game::Colors::c_EndPieceTextColor}
};

GamePresenter::GamePresenter(QObject *parent)
    : QObject(parent)
    , m_pDataEntryPresenter{new DataEntryPresenter{this}}
    , m_MainPaneInitialized {false}
    , m_IsDataEntryHelpMenuActive{false}
    , m_MainPaneStatisticsResetEnabled {false}
    , m_ClearMainPaneInputEnabled{false}
    , m_ErrorOccured {false}
    , m_QuitDeferred{false}
    , m_WindowTitle{Game::Titles::c_IntroWindowTitle}
    , m_CurrentPane {Pane::INTRO}
    , m_StatusUpdatePane{Pane::INTRO}
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

    bool connected{connect(m_pGameFacade, &GameFacade::statisticsChanged, this, &GamePresenter::_onStatisticsChanged)};
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

    m_pGameFacade->init();
}

void GamePresenter::switchToPane(GamePresenter::Pane pane)
{
    m_PreviousPanesStack.append(m_CurrentPane);
    _switchToPane(pane);

    if (pane == GamePresenter::Pane::DATA_ENTRY)
    {
        qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->startDataEntry();
    }
}

void GamePresenter::goBack()
{
    if (m_CurrentPane == Pane::PROMPT_SAVE_EXIT)
    {
        m_CurrentPane = Pane::DATA_ENTRY;
        m_PreviousPanesStack.pop_back();

        if (m_QuitDeferred)
        {
            m_QuitDeferred = false;
            Q_EMIT quitGameDeferredChanged();
        }

        Q_EMIT currentPaneChanged();

        qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->resumeDataEntry();
    }
    else
    {
        Pane previousPane{m_PreviousPanesStack.last()};
        m_PreviousPanesStack.pop_back();
        _switchToPane(previousPane);
    }
}

void GamePresenter::promptForSavingAddedWordPairs()
{
    Q_ASSERT(m_CurrentPane == Pane::DATA_ENTRY);

    m_PreviousPanesStack.append(m_CurrentPane);
    m_CurrentPane = Pane::PROMPT_SAVE_EXIT;

    Q_EMIT currentPaneChanged();
}

void GamePresenter::promptForDiscardingAddedWordPairs()
{
    Q_ASSERT(m_CurrentPane == Pane::DATA_ENTRY);

    m_PreviousPanesStack.append(m_CurrentPane);
    m_CurrentPane = Pane::PROMPT_DISCARD;

    Q_EMIT currentPaneChanged();
}

void GamePresenter::handleDataSaveInProgress()
{
    m_pGameFacade->handleDataSavingOperationInProgress();
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

void GamePresenter::switchToLevel(int level)
{
    Q_ASSERT(level >= 0 && level < static_cast<int>(Game::Levels::LEVEL_NONE));
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
        _switchToPane(Pane::DATA_ENTRY);
        qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->handleSaveAddedWordPairsRequest();
    }
}

QObject *GamePresenter::getDataEntryPresenter() const
{
    return m_pDataEntryPresenter;
}

bool GamePresenter::getIntroPaneVisible() const
{
    return (m_CurrentPane == Pane::INTRO);
}

bool GamePresenter::getHelpPaneVisible() const
{
    return (m_CurrentPane == Pane::HELP);
}

bool GamePresenter::getMainPaneVisible() const
{
    return (m_CurrentPane == Pane::MAIN);
}

bool GamePresenter::getDataEntryPaneVisible() const
{
    return (m_CurrentPane == Pane::DATA_ENTRY);
}

bool GamePresenter::getPromptSaveExitPaneVisible() const
{
    return (m_CurrentPane == Pane::PROMPT_SAVE_EXIT);
}

bool GamePresenter::getPromptDiscardPaneVisible() const
{
    return (m_CurrentPane == Pane::PROMPT_DISCARD);
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

int GamePresenter::getLevelEasy() const
{
    return static_cast<int>(Game::Levels::LEVEL_EASY);
}

int GamePresenter::getLevelMedium() const
{
    return static_cast<int>(Game::Levels::LEVEL_MEDIUM);
}

int GamePresenter::getLevelHard() const
{
    return static_cast<int>(Game::Levels::LEVEL_HARD);
}

QString GamePresenter::getWindowTitle() const
{
    return (m_CurrentPane == Pane::HELP ? (c_WindowTitles[m_CurrentPane].arg(m_IsDataEntryHelpMenuActive ? DataEntry::Misc::c_DataEntryDescriptor
                                                                                                         : Game::Misc::c_GameDescriptor))
                                        : c_WindowTitles[m_CurrentPane]);
}

QString GamePresenter::getIntroPaneMessage() const
{
    return m_IntroPaneMessage;
}

QString GamePresenter::getHelpPaneMessage() const
{
    return (m_IsDataEntryHelpMenuActive ? DataEntry::Messages::c_DataEntryHelpMessage : Game::Messages::c_GameHelpMessage);
}

QString GamePresenter::getHelpPaneToolTip() const
{
    return (Game::Messages::c_GameHelpPaneToolTip.arg((m_IsDataEntryHelpMenuActive ? DataEntry::Misc::c_DataEntryDescriptor
                                                                                   : Game::Misc::c_GameDescriptor).toLower()));
}

QString GamePresenter::getHelpButtonToolTip() const
{
    return (Game::Messages::c_GameHelpButtonToolTip.arg((m_CurrentPane == Pane::DATA_ENTRY ? DataEntry::Misc::c_DataEntryDescriptor
                                                                                           : Game::Misc::c_GameDescriptor).toLower()));
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

void GamePresenter::_onStatusChanged(Game::StatusCodes statusCode)
{
    // once main pane is accessed exclude handling the status codes that are only applicable for the intro pane (from status message display point of view) ...
    if (!m_MainPaneInitialized || (statusCode != Game::StatusCodes::DATA_SUCCESSFULLY_SAVED &&
                                   statusCode != Game::StatusCodes::ADDITIONAL_DATA_SAVE_IN_PROGRESS) ||
                                  (m_QuitDeferred && statusCode == Game::StatusCodes::DATA_SUCCESSFULLY_SAVED)) // ...except this status when user quits and data save is pending
    {
        if (m_pStatusUpdateTimer->isActive())
        {
            m_pStatusUpdateTimer->stop();
        }

        switch (statusCode)
        {
        case Game::StatusCodes::NO_LANGUAGE_SET:
            _updateStatusMessage(Game::Messages::c_WelcomeMessage, Pane::INTRO, Game::Timing::c_NoDelay);
            break;
        case Game::StatusCodes::FETCHING_DATA:
            if (m_CurrentPane == Pane::INTRO || m_CurrentPane == Pane::MAIN)
            {
                _updateStatusMessage(Game::Messages::c_FetchingDataMessage, m_CurrentPane, Game::Timing::c_NoDelay);
            }
            break;
        case Game::StatusCodes::DATA_FETCHING_COMPLETE:
            if (m_CurrentPane == Pane::INTRO)
            {
                _updateStatusMessage(Game::Messages::c_LanguageChangedMessage, Pane::INTRO, Game::Timing::c_NoDelay);
                _updateStatusMessage(Game::Messages::c_PleasePlayOrEnterDataMessage, Pane::INTRO, Game::Timing::c_ShortStatusUpdateDelay);
            }
            else if (m_CurrentPane == Pane::MAIN)
            {
                _updateStatusMessage(Game::Messages::c_LanguageChangedMessage, Pane::MAIN, Game::Timing::c_NoDelay);
                _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            }
            break;
        case Game::StatusCodes::NO_DATA_ENTRIES_FETCHED:
            if (m_CurrentPane == Pane::INTRO)
            {
                _updateStatusMessage(Game::Messages::c_LanguageChangedMessage, Pane::INTRO, Game::Timing::c_NoDelay);
                _updateStatusMessage(Game::Messages::c_NoValidEntriesFetchedMessage, Pane::INTRO, Game::Timing::c_ShortStatusUpdateDelay);
            }
            else if (m_CurrentPane == Pane::MAIN)
            {
                _updateStatusMessage(Game::Messages::c_CannotChangeLanguageMessage, Pane::MAIN, Game::Timing::c_NoDelay);
                _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            }
            break;
        case Game::StatusCodes::DATA_FETCHING_ERROR:
            _launchErrorPane(Game::Error::c_CannotFetchDataMessage);
            break;
        case Game::StatusCodes::DATA_GOT_AVAILABLE:
            _updateStatusMessage(Game::Messages::c_PleasePlayOrEnterDataMessage, Pane::INTRO, Game::Timing::c_NoDelay);
            break;
        case Game::StatusCodes::DATA_STILL_UNAVAILABLE:
            _updateStatusMessage(Game::Messages::c_NoValidEntriesSavedForGameLanguage, Pane::INTRO, Game::Timing::c_NoDelay);
            break;
        case Game::StatusCodes::GAME_STARTED:
            _updateStatusMessage(Game::Messages::c_GameStartedMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::GAME_PAUSED:
            _updateStatusMessage(Game::Messages::c_GamePausedMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            break;
        case Game::StatusCodes::GAME_RESUMED_COMPLETE_INPUT:
            _updateStatusMessage(Game::Messages::c_GameResumedMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_AllPiecesAddedMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::GAME_RESUMED_INCOMPLETE_INPUT:
            _updateStatusMessage(Game::Messages::c_GameResumedMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::GAME_STOPPED:
            if (m_CurrentPane == Pane::MAIN)
            {
                _updateStatusMessage(Game::Messages::c_GameStoppedMessage, Pane::MAIN, Game::Timing::c_NoDelay);
                QTimer::singleShot(Game::Timing::c_GameQuitDelay, this, [](){QGuiApplication::quit();});
            }
            else
            {
                QGuiApplication::quit();
            }
            break;
        case Game::StatusCodes::NEW_DATA_SAVE_IN_PROGRESS:
            _updateStatusMessage(Game::Messages::c_NewDataSaveInProgressMessage, Pane::INTRO, Game::Timing::c_NoDelay);
            break;
        case Game::StatusCodes::ADDITIONAL_DATA_SAVE_IN_PROGRESS:
            _updateStatusMessage(Game::Messages::c_AdditionalDataSaveInProgressMessage, Pane::INTRO, Game::Timing::c_NoDelay);
            break;
        case Game::StatusCodes::DATA_SUCCESSFULLY_SAVED:
            _updateStatusMessage(Game::Messages::c_AdditionalDataAvailableMessage, Pane::INTRO, Game::Timing::c_NoDelay);
            if (m_QuitDeferred)
            {
                QTimer::singleShot(Game::Timing::c_GameQuitDelay, this, [this](){quit();});
            }
            else if (m_CurrentPane == Pane::INTRO)
            {
                _updateStatusMessage(Game::Messages::c_PleasePlayOrEnterDataMessage, Pane::INTRO, Game::Timing::c_ShortStatusUpdateDelay);
            }
            break;
        case Game::StatusCodes::DATA_ENTRY_SAVING_ERROR:
            _launchErrorPane(Game::Error::c_CannotSaveDataMessage);
            break;
        case Game::StatusCodes::PIECE_NOT_ADDED:
            _updateStatusMessage(Game::Messages::c_PieceNotAddedToInputMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::PIECE_ADDED_COMPLETE_INPUT:
            _updateStatusMessage(Game::Messages::c_PieceAddedToInputMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_AllPiecesAddedMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::PIECE_ADDED_INCOMPLETE_INPUT:
            _updateStatusMessage(Game::Messages::c_PieceAddedToInputMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::PIECES_REMOVED:
            _updateStatusMessage(Game::Messages::c_PiecesRemovedFromInputMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::USER_INPUT_CLEARED:
            _updateStatusMessage(Game::Messages::c_AllPiecesRemovedMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::CORRECT_USER_INPUT:
        {
            QString message{Game::Messages::c_CorrectUserInputMessage.arg(m_pGameFacade->getFirstReferenceWord())
                        .arg(m_pGameFacade->getSecondReferenceWord())
                        .arg(m_pGameFacade->areWordsFromCurrentPairSynonyms() ? Game::Misc::c_SynonymsDescriptor : Game::Misc::c_AntonymsDescriptor)};
            _updateStatusMessage(message, Pane::MAIN, Game::Timing::c_NoDelay);
        }
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_LongStatusUpdateDelay);
            break;
        case Game::StatusCodes::INCORRECT_USER_INPUT:
            _updateStatusMessage(Game::Messages::c_IncorrectUserInputMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_AllPiecesAddedMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::SOLUTION_REQUESTED_BY_USER:
        {
            QString message = Game::Messages::c_ShowPairRequestedByUserMessage.arg(m_pGameFacade->getFirstReferenceWord())
                    .arg(m_pGameFacade->getSecondReferenceWord())
                    .arg(m_pGameFacade->areWordsFromCurrentPairSynonyms() ? Game::Misc::c_SynonymsDescriptor : Game::Misc::c_AntonymsDescriptor);
            _updateStatusMessage(message, Pane::MAIN, Game::Timing::c_NoDelay);
        }
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_LongStatusUpdateDelay);
            break;
        case Game::StatusCodes::STATISTICS_RESET_COMPLETE_INPUT:
            _updateStatusMessage(Game::Messages::c_StatisticsResetMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_AllPiecesAddedMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::STATISTICS_RESET_INCOMPLETE_INPUT:
            _updateStatusMessage(Game::Messages::c_StatisticsResetMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::LEVEL_CHANGED:
            _updateStatusMessage(Game::Messages::c_LevelChangedMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::PERSISTENT_MODE_ENTERED:
            _updateStatusMessage(Game::Messages::c_CursorModeEnabledMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::PERSISTENT_MODE_EXITED:
            _updateStatusMessage(Game::Messages::c_CursorModeDisabledMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::PERSISTENT_INDEX_REQUIRED:
            _updateStatusMessage(Game::Messages::c_ClickSelectOrRemoveDisabledMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::TIME_LIMIT_ENABLED:
            _updateStatusMessage(Game::Messages::c_TimeLimitEnabledMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::TIME_LIMIT_DISABLED:
            _updateStatusMessage(Game::Messages::c_TimeLimitDisabledMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        case Game::StatusCodes::TIME_LIMIT_REACHED:
            _updateStatusMessage(Game::Messages::c_TimeLimitReachedMessage, Pane::MAIN, Game::Timing::c_NoDelay);
            _updateStatusMessage(Game::Messages::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::Timing::c_ShortStatusUpdateDelay);
            break;
        default:
            Q_ASSERT(false);
        }
    }
}

void GamePresenter::_switchToPane(Pane pane)
{
    Q_ASSERT(static_cast<int>(pane) >= 0 && static_cast<int>(pane) < static_cast<int>(Pane::Nr_Of_Panes));
    Q_ASSERT(!(m_CurrentPane == Pane::MAIN && pane == Pane::INTRO));

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
        case Pane::INTRO:
            break;
        case Pane::HELP:
            break;
        case Pane::MAIN:
            isGamePausingRequired = true;
            break;
        case Pane::DATA_ENTRY:
            break;
        case Pane::PROMPT_DISCARD:
            break;
        case Pane::PROMPT_SAVE_EXIT:
            break;
        default:
            Q_ASSERT(false);
        }

        switch(pane)
        {
        case Pane::INTRO:
            if (m_CurrentPane == Pane::DATA_ENTRY)
            {
                qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->stopDataEntry();
                delayedSwitchingRequested = true;
            }
            break;
        case Pane::HELP:
            if (isGamePausingRequired)
            {
                m_pGameFacade->pauseGame();
                delayedSwitchingRequested = true;
            }
            _setDataEntryHelpMenuActive(m_CurrentPane == Pane::DATA_ENTRY);
            break;
        case Pane::MAIN:
            if (!m_MainPaneInitialized)
            {
                m_MainPaneInitialized = true;
                m_pGameFacade -> startGame();
            }
            else
            {
                if (m_CurrentPane == Pane::DATA_ENTRY)
                {
                    qobject_cast<DataEntryPresenter*>(m_pDataEntryPresenter)->stopDataEntry();
                    delayedSwitchingRequested = true;
                }
                m_pGameFacade -> resumeGame();
            }
            break;
        case Pane::DATA_ENTRY:
            if (isGamePausingRequired)
            {
                m_pGameFacade->pauseGame();
                delayedSwitchingRequested = true;
            }
            else if (m_CurrentPane == Pane::HELP)
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
            QTimer::singleShot(Game::Timing::c_PaneSwitchingDelay, this, [triggerPaneSwitching](){triggerPaneSwitching();});
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

void GamePresenter::_updateStatusMessage(const QString& message, GamePresenter::Pane pane, int delay)
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
    case Pane::INTRO:
        m_IntroPaneMessage = m_CurrentStatusMessage;
        Q_EMIT introPaneMessageChanged();
        break;
    case Pane::HELP:
        break;
    case Pane::MAIN:
        m_MainPaneStatusMessage = m_CurrentStatusMessage;
        Q_EMIT mainPaneStatusMessageChanged();
        break;
    case Pane::DATA_ENTRY:
        // data entry pane status to be updated by the data entry presenter only
        Q_ASSERT(false);
    case Pane::ERROR:
        break;
    default:
        Q_ASSERT(static_cast<int>(m_StatusUpdatePane) >= 0 && static_cast<int>(m_StatusUpdatePane) < static_cast<int>(Pane::Nr_Of_Panes));
    }
}

void GamePresenter::_launchErrorPane(const QString& errorMessage)
{
    m_CurrentPane = Pane::ERROR;
    m_ErrorMessage = errorMessage;
    m_ErrorOccured = true;

    Q_EMIT currentPaneChanged();
    Q_EMIT errorMessageChanged();
    Q_EMIT errorOccuredChanged();
}
