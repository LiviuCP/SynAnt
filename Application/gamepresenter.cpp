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
    {GamePresenter::Pane::DATA_ENTRY, GameStrings::c_DataEntryWindowTitle},
    {GamePresenter::Pane::PROMPT_SAVE_EXIT, GameStrings::c_PromptSaveExitWindowTitle},
    {GamePresenter::Pane::ERROR, GameStrings::c_FatalErrorWindowTitle}
};

static const QMap<Game::PieceTypes, QColor> c_WordPieceTextColors
{
    {Game::PieceTypes::BEGIN_PIECE, GameStrings::c_WordFirstPieceTextColor},
    {Game::PieceTypes::MIDDLE_PIECE, GameStrings::c_WordMiddlePieceTextColor},
    {Game::PieceTypes::END_PIECE, GameStrings::c_WordLastPieceTextColor}
};

static const QMap<Game::ValidationCodes, QString> c_InvalidPairEntryReasonMessages
{
    {Game::ValidationCodes::LESS_MIN_CHARS_PER_WORD, GameStrings::c_WordHasLessThanMinimumCharacters},
    {Game::ValidationCodes::LESS_MIN_TOTAL_PAIR_CHARS, GameStrings::c_PairHasLessThanMinimumCharacters},
    {Game::ValidationCodes::MORE_MAX_TOTAL_PAIR_CHARS, GameStrings::c_PairHasMoreThanMaximumCharacters},
    {Game::ValidationCodes::INVALID_CHARACTERS, GameStrings::c_InvalidCharacters},
    {Game::ValidationCodes::IDENTICAL_WORDS, GameStrings::c_PairHasIdenticalWords},
    {Game::ValidationCodes::PAIR_ALREADY_EXISTS, GameStrings::c_PairAlreadyExists}
};

GamePresenter::GamePresenter(QObject *parent)
    : QObject(parent)
    , m_IntroPaneVisible {true}
    , m_HelpPaneVisible {false}
    , m_MainPaneVisible {false}
    , m_DataEntryPaneVisible{false}
    , m_PromptSaveExitPaneVisible{false}
    , m_MainPaneInitialized {false}
    , m_MainPaneStatisticsResetEnabled {false}
    , m_ClearMainPaneInputEnabled{false}
    , m_ErrorOccured {false}
    , m_WindowTitle{GameStrings::c_IntroWindowTitle}
    , m_CurrentPane {Pane::INTRO}
    , m_StatusUpdatePane{Pane::INTRO}
    , m_PreviousPanesStack{}
    , m_FirstWordInputPiecesHoverIndex{-1}
    , m_SecondWordInputPiecesHoverIndex{-1}
    , m_pGameFacade{nullptr}
    , m_pGameProxy {new GameProxy{this}}
    , m_pStatusUpdateTimer {new QTimer{this}}
{
    m_pGameFacade = m_pGameProxy->getFacade();
    Q_ASSERT(m_pGameFacade);

    m_pStatusUpdateTimer->setSingleShot(true);

    bool connected{connect(m_pGameFacade, &GameFacade::statisticsChanged, this, &GamePresenter::_onStatisticsChanged)};
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::statusChanged, this, &GamePresenter::_onStatusChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::mixedWordsChanged, this, &GamePresenter::mixedWordsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::dataAvailableChanged, this, &GamePresenter::playEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::dataEntryAllowedChanged, this, &GamePresenter::dataEntryEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::addPairToCacheAllowedChanged, this, &GamePresenter::addWordsPairEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::saveNewPairsToDbAllowedChanged, this, &GamePresenter::saveAddedWordPairsEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::resetCacheAllowedChanged, this, &GamePresenter::discardAddedWordPairsEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::inputChanged, this, &GamePresenter::_onInputChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::piecesAddedToInputChanged, this, &GamePresenter::selectionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::completionChanged, this, &GamePresenter::submitMainPaneInputEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::persistentPieceSelectionIndexChanged, this, &GamePresenter::pieceSelectionCursorPositionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::persistentPiecesRemovalIndexesChanged, this, &GamePresenter::piecesRemovalCursorPositionChanged);
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
        m_pGameFacade->startWordEntry();
    }
}

void GamePresenter::goBack()
{
    if (m_CurrentPane == Pane::PROMPT_SAVE_EXIT)
    {
        m_CurrentPane = Pane::DATA_ENTRY;
        m_PromptSaveExitPaneVisible = false;
        m_DataEntryPaneVisible = true;
        m_PreviousPanesStack.pop_back();

        if (m_QuitDeferred)
        {
            m_QuitDeferred = false;
            Q_EMIT quitGameDeferredChanged();
        }

        Q_EMIT currentPaneChanged();

        m_pGameFacade->resumeWordEntry();
    }
    else
    {
        Pane previousPane{m_PreviousPanesStack.last()};
        m_PreviousPanesStack.pop_back();
        _switchToPane(previousPane);
    }
}

void GamePresenter::handleAddWordsPairRequest(const QString& firstWord, const QString& secondWord, bool areSynonyms)
{
    m_pGameFacade->requestAddPairToCache(firstWord, secondWord, areSynonyms);
}

void GamePresenter::handleClearAddedWordPairsRequest()
{
    m_pGameFacade->requestCacheReset();
}

void GamePresenter::handleSaveAddedWordPairsRequest()
{
    m_pGameFacade->requestSaveDataToDb();
}

void GamePresenter::promptForSavingAddedWordPairs()
{
    Q_ASSERT(m_CurrentPane == Pane::DATA_ENTRY);

    m_PreviousPanesStack.append(m_CurrentPane);
    m_CurrentPane = Pane::PROMPT_SAVE_EXIT;

    m_DataEntryPaneVisible = false;
    m_PromptSaveExitPaneVisible = true;

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

void GamePresenter::switchToLevel(int level)
{
    Q_ASSERT(level >= 0 && level < static_cast<int>(Game::Level::NrOfLevels));
    m_pGameFacade->setLevel(static_cast<Game::Level>(level));
}

void GamePresenter::enablePieceSelectionFromKeyboard()
{
    if (!getAreFirstWordInputPiecesHovered() && !getAreSecondWordInputPiecesHovered())
    {
        m_pGameFacade->disablePersistentPiecesRemoval();
        m_pGameFacade->enablePersistentPieceSelection();
    }
}

void GamePresenter::disablePieceSelectionFromKeyboard()
{
    m_pGameFacade->disablePersistentPieceSelection();
}

void GamePresenter::movePieceSelectionCursorToLeft()
{
    m_pGameFacade->decreasePersistentPieceSelectionIndex();
}

void GamePresenter::movePieceSelectionCursorToRight()
{
    m_pGameFacade->increasePersistentPieceSelectionIndex();
}

void GamePresenter::enableFirstWordPiecesRemovalFromKeyboard()
{
    if (!getAreFirstWordInputPiecesHovered() && !getAreSecondWordInputPiecesHovered())
    {
        m_pGameFacade->disablePersistentPieceSelection();
        m_pGameFacade->enablePersistentPiecesRemoval(Game::InputWordNumber::ONE);
    }
}

void GamePresenter::enableSecondWordPiecesRemovalFromKeyboard()
{
    if (!getAreFirstWordInputPiecesHovered() && !getAreSecondWordInputPiecesHovered())
    {
        m_pGameFacade->disablePersistentPieceSelection();
        m_pGameFacade->enablePersistentPiecesRemoval(Game::InputWordNumber::TWO);
    }
}

void GamePresenter::disablePiecesRemovalFromKeyboard()
{
    m_pGameFacade->disablePersistentPiecesRemoval();
}

void GamePresenter::movePiecesRemovalCursorToLeft()
{
    m_pGameFacade->decreasePersistentPiecesRemovalIndex();
}

void GamePresenter::movePiecesRemovalCursorToRight()
{
    m_pGameFacade->increasePersistentPiecesRemovalIndex();
}

void GamePresenter::selectWordPieceForFirstInputWord(int wordPieceIndex, bool fromKeyboard)
{
    if (!fromKeyboard && m_pGameFacade->getPersistentPieceSelectionIndex() != -1)
    {
        disablePieceSelectionFromKeyboard();
    }

    m_pGameFacade->addWordPieceToInputWord(Game::InputWordNumber::ONE, wordPieceIndex);
}

void GamePresenter::selectWordPieceForSecondInputWord(int wordPieceIndex, bool fromKeyboard)
{
    if (!fromKeyboard && m_pGameFacade->getPersistentPieceSelectionIndex() != -1)
    {
        disablePieceSelectionFromKeyboard();
    }

    m_pGameFacade->addWordPieceToInputWord(Game::InputWordNumber::TWO, wordPieceIndex);
}

void GamePresenter::removeWordPiecesFromFirstInputWord(int inputRangeStart, bool fromKeyboard)
{
    if (!fromKeyboard && m_pGameFacade->getFirstPersistentPiecesRemovalIndex() != -1)
    {
        disablePiecesRemovalFromKeyboard();
    }

    m_pGameFacade->removeWordPiecesFromInputWord(Game::InputWordNumber::ONE, inputRangeStart);
}

void GamePresenter::removeWordPiecesFromSecondInputWord(int inputRangeStart, bool fromKeyboard)
{
    if (!fromKeyboard && m_pGameFacade->getSecondPersistentPiecesRemovalIndex() != -1)
    {
        disablePiecesRemovalFromKeyboard();
    }

    m_pGameFacade->removeWordPiecesFromInputWord(Game::InputWordNumber::TWO, inputRangeStart);
}

void GamePresenter::clearMainPaneInput()
{
    m_pGameFacade->clearInput();
}

void GamePresenter::clearMainPaneFirstInputWord()
{
    if (!m_pGameFacade->getFirstWordInputIndexes().empty())
    {
        m_pGameFacade->removeWordPiecesFromInputWord(Game::InputWordNumber::ONE, 0);
    }
}

void GamePresenter::clearMainPaneSecondInputWord()
{
    if (!m_pGameFacade->getSecondWordInputIndexes().empty())
    {
        m_pGameFacade->removeWordPiecesFromInputWord(Game::InputWordNumber::TWO, 0);
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

bool GamePresenter::getDataEntryPaneVisible() const
{
    return m_DataEntryPaneVisible;
}

bool GamePresenter::getPromptSaveExitPaneVisible() const
{
    return m_PromptSaveExitPaneVisible;
}

bool GamePresenter::isPlayEnabled() const
{
    return m_pGameFacade->isDataAvailable();
}

bool GamePresenter::isDataEntryEnabled() const
{
    return m_pGameFacade->isDataEntryAllowed();
}

bool GamePresenter::isAddWordsPairEnabled() const
{
    return m_pGameFacade->isAddingToCacheAllowed();
}

bool GamePresenter::isDiscardAddedWordPairsEnabled() const
{
    return m_pGameFacade->isCacheResetAllowed();
}

bool GamePresenter::isSaveAddedWordPairsEnabled() const
{
    return m_pGameFacade->isSavingToDbAllowed();
}

bool GamePresenter::getMainPaneStatisticsResetEnabled() const
{
    return m_MainPaneStatisticsResetEnabled;
}

bool GamePresenter::getClearMainPaneInputEnabled() const
{
    return m_ClearMainPaneInputEnabled;
}

bool GamePresenter::getSubmitMainPaneInputEnabled() const
{
    return m_pGameFacade->isInputComplete();
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

QString GamePresenter::getDataEntryPaneStatusMessage() const
{
    return m_DataEntryPaneStatusMessage;
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
    int obtainedScore{m_pGameFacade->getObtainedScore()};
    int totalAvailableScore{m_pGameFacade->getTotalAvailableScore()};
    int guessedWordPairs{m_pGameFacade->getGuessedWordPairs()};
    int totalWordPairs{m_pGameFacade->getTotalWordPairs()};

    bool emptyStatistics{obtainedScore == 0 && totalAvailableScore == 0 && guessedWordPairs == 0 && totalWordPairs == 0};

    if (!m_MainPaneStatisticsResetEnabled && !emptyStatistics)
    {
        m_MainPaneStatisticsResetEnabled = true;
        Q_EMIT mainPaneStatisticsResetEnabledChanged();
    }
    else if (m_MainPaneStatisticsResetEnabled && emptyStatistics)
    {
        m_MainPaneStatisticsResetEnabled = false;
        Q_EMIT mainPaneStatisticsResetEnabledChanged();
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
        _updateStatusMessage(GameStrings::c_DataLoadingMessage, Pane::INTRO, Game::c_NoDelay);
        break;
    case Game::StatusCodes::DATA_LOADING_COMPLETE:
        _updateStatusMessage(GameStrings::c_DataAvailableMessage, Pane::INTRO, Game::c_NoDelay);
        break;
    case Game::StatusCodes::NO_VALID_DATA_ENTRIES_LOADED:
        _updateStatusMessage(GameStrings::c_NoValidEntriesLoadedMessage, Pane::INTRO, Game::c_NoDelay);
        break;
    case Game::StatusCodes::DATA_LOADING_ERROR:
        _launchErrorPane(GameStrings::c_CannotOpenFileMessage);
        break;
    case Game::StatusCodes::DATA_ENTRY_SAVING_ERROR:
        _launchErrorPane(GameStrings::c_CannotSaveDataMessage);
        break;
    case Game::StatusCodes::DATA_SAVE_IN_PROGRESS:
        _updateStatusMessage(GameStrings::c_DataSaveInProgressMessage, Pane::DATA_ENTRY, Game::c_NoDelay);
        break;
    case Game::StatusCodes::DATA_SUCCESSFULLY_SAVED:
        _updateStatusMessage(GameStrings::c_DataSuccessfullySavedMessage.arg(m_pGameFacade->getLastSavedNrOfWordPairs()), Pane::DATA_ENTRY, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Pane::DATA_ENTRY, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::GAME_STARTED:
        _updateStatusMessage(GameStrings::c_GameStartedMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::GAME_PAUSED:
        _updateStatusMessage(GameStrings::c_GamePausedMessage, Pane::MAIN, Game::c_NoDelay);
        break;
    case Game::StatusCodes::GAME_RESUMED_COMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_GameResumedMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_AllPiecesAddedMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::GAME_RESUMED_INCOMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_GameResumedMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::GAME_STOPPED:
        if (m_MainPaneVisible)
        {
            _updateStatusMessage(GameStrings::c_GameStoppedMessage, Pane::MAIN, Game::c_NoDelay);
            QTimer::singleShot(Game::c_GameQuitDelay, this, [](){QGuiApplication::quit();});
        }
        else
        {
            QGuiApplication::quit();
        }
        break;
    case Game::StatusCodes::CORRECT_USER_INPUT:
        {
            QString message{GameStrings::c_CorrectUserInputMessage.arg(m_pGameFacade->getFirstReferenceWord())
                                                         .arg(m_pGameFacade->getSecondReferenceWord())
                                                         .arg(m_pGameFacade->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms)};
            _updateStatusMessage(message, Pane::MAIN, Game::c_NoDelay);
        }
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_LongStatusUpdateDelay);
        break;
    case Game::StatusCodes::INCORRECT_USER_INPUT:
        _updateStatusMessage(GameStrings::c_IncorrectUserInputMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_AllPiecesAddedMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::SOLUTION_REQUESTED_BY_USER:
        {
            QString message = GameStrings::c_ShowPairRequestedByUserMessage.arg(m_pGameFacade->getFirstReferenceWord())
                                                                   .arg(m_pGameFacade->getSecondReferenceWord())
                                                                   .arg(m_pGameFacade->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
            _updateStatusMessage(message, Pane::MAIN, Game::c_NoDelay);
        }
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_LongStatusUpdateDelay);
        break;
    case Game::StatusCodes::STATISTICS_RESET_COMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_ScoresResetMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_AllPiecesAddedMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::STATISTICS_RESET_INCOMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_ScoresResetMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::LEVEL_CHANGED:
        _updateStatusMessage(GameStrings::c_LevelChangedMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::PIECE_NOT_ADDED:
        _updateStatusMessage(GameStrings::c_PieceNotAddedToInputMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::PIECE_ADDED_COMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_PieceAddedToInputMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_AllPiecesAddedMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::PIECE_ADDED_INCOMPLETE_INPUT:
        _updateStatusMessage(GameStrings::c_PieceAddedToInputMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::PIECES_REMOVED:
        _updateStatusMessage(GameStrings::c_PiecesRemovedFromInputMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::USER_INPUT_CLEARED:
        _updateStatusMessage(GameStrings::c_AllPiecesRemovedMessage, Pane::MAIN, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_SelectOrDeleteWordPiecesMessage, Pane::MAIN, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::DATA_ENTRY_STARTED:
        _updateStatusMessage(GameStrings::c_DataEntryStartMessage, Pane::DATA_ENTRY, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Pane::DATA_ENTRY, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::DATA_ENTRY_RESUMED:
        _updateStatusMessage(GameStrings::c_DataEntryResumeMessage.arg(m_pGameFacade->getCurrentNrOfAddedWordPairs()), Pane::DATA_ENTRY, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Pane::DATA_ENTRY, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::PAIR_ALREADY_ADDED:
        _updateStatusMessage(GameStrings::c_PairAlreadyAddedMessage, Pane::DATA_ENTRY, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Pane::DATA_ENTRY, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::DATA_ENTRY_SUCCESS:
        Q_EMIT dataEntrySucceeded();
        _updateStatusMessage(GameStrings::c_DataEntrySuccessMessage.arg(m_pGameFacade->getCurrentNrOfAddedWordPairs()), Pane::DATA_ENTRY, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Pane::DATA_ENTRY, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::DATA_GOT_AVAILABLE:
        Q_EMIT dataEntrySucceeded();
        _updateStatusMessage(GameStrings::c_DataEntrySuccessMessage.arg(m_pGameFacade->getLastSavedNrOfWordPairs()), Pane::DATA_ENTRY, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataAvailableMessage, Pane::INTRO, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Pane::DATA_ENTRY, Game::c_ShortStatusUpdateDelay);
        break;
    case Game::StatusCodes::DATA_ENTRY_INVALID:
        _updateStatusMessage(GameStrings::c_DataEntryInvalidPairMessage.arg(c_InvalidPairEntryReasonMessages[m_pGameFacade->getDataEntryValidationCode()]),
                            Pane::DATA_ENTRY, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Pane::DATA_ENTRY, Game::c_ShortStatusUpdateDelay);
        Q_EMIT dataEntryInvalid();
        break;
    case Game::StatusCodes::RESET_CACHE_REQUESTED:
        _updateStatusMessage(GameStrings::c_DataEntryCacheResetRequestedMessage, Pane::DATA_ENTRY, Game::c_NoDelay);
        break;
    case Game::StatusCodes::CACHE_RESET:
        _updateStatusMessage(GameStrings::c_DataEntryCacheResetMessage, Pane::DATA_ENTRY, Game::c_NoDelay);
        _updateStatusMessage(GameStrings::c_DataEntryRequestMessage, Pane::DATA_ENTRY, Game::c_ShortStatusUpdateDelay);
        break;
    default:
        Q_ASSERT(false);
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
            isGamePausingRequired = true;
            break;
        case Pane::DATA_ENTRY:
            m_DataEntryPaneVisible = false;
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
            if (isGamePausingRequired)
            {
                m_pGameFacade->pauseGame();
                delayedSwitchingRequested = true;
            }
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
        case Pane::DATA_ENTRY:
            if (isGamePausingRequired)
            {
                m_pGameFacade->pauseGame();
                delayedSwitchingRequested = true;
            }
            else if (m_CurrentPane == Pane::HELP)
            {
                m_pGameFacade->resumeWordEntry();
            }

            m_DataEntryPaneVisible = true;
            break;
        default:
            Q_ASSERT(false);
        }

        m_CurrentPane = pane;

        if (delayedSwitchingRequested)
        {
            QTimer::singleShot(Game::c_PaneSwitchingDelay, this, [triggerPaneSwitching](){triggerPaneSwitching();});
        }
        else
        {
            triggerPaneSwitching();
        }
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
        m_DataEntryPaneStatusMessage = m_CurrentStatusMessage;
        Q_EMIT dataEntryPaneStatusMessageChanged();
        break;
    case Pane::ERROR:
        break;
    default:
        Q_ASSERT(static_cast<int>(m_StatusUpdatePane) >= 0 && static_cast<int>(m_StatusUpdatePane) < static_cast<int>(Pane::Nr_Of_Panes));
    }
}

void GamePresenter::_launchErrorPane(const QString& errorMessage)
{
    switch (m_CurrentPane)
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
    case Pane::DATA_ENTRY:
        m_DataEntryPaneVisible = false;
        break;
    case Pane::PROMPT_SAVE_EXIT:
        m_PromptSaveExitPaneVisible = false;
        break;
    default:
        Q_ASSERT(static_cast<int>(m_CurrentPane) >= 0 && static_cast<int>(m_CurrentPane) < static_cast<int>(Pane::Nr_Of_Panes));
    }

    m_CurrentPane = Pane::ERROR;
    m_ErrorMessage = errorMessage;
    m_ErrorOccured = true;

    Q_EMIT currentPaneChanged();
    Q_EMIT errorMessageChanged();
    Q_EMIT errorOccuredChanged();
}
