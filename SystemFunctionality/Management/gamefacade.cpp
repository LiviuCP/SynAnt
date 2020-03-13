#include "gamefacade.h"
#include "../CoreFunctionality/wordmixer.h"
#include "../CoreFunctionality/wordpairowner.h"
#include "../CoreFunctionality/inputbuilder.h"
#include "../DataAccess/datasource.h"
#include "../DataAccess/datasourceaccesshelper.h"
#include "../ManagementProxies/gamefunctionalityproxy.h"
#include "../Utilities/statisticsitem.h"
#include "../Utilities/chronometer.h"

GameFacade::GameFacade(QObject *parent)
    : QObject(parent)
    , m_pGameFunctionalityProxy{new GameFunctionalityProxy{this}}
    , m_GameLevel{Game::Levels::LEVEL_MEDIUM}
    , m_CurrentLanguageIndex{-1}
    , m_PreviousLanguageIndex{-1}
    , m_CurrentStatusCode{Game::StatusCodes::NO_LANGUAGE_SET}
    , m_IsConnectedToDataSource{false}
    , m_IsDataAvailable{false}
    , m_IsGameStarted{false}
    , m_IsGamePaused{false}
    , m_IsPersistentIndexModeEnabled{false}
    , m_IsFetchingInProgress{false}
    , m_ShouldRevertLanguageWhenDataUnavailable{false}
{
    m_pDataSourceAccessHelper = m_pGameFunctionalityProxy->getDataSourceAccessHelper();
    m_pWordMixer = m_pGameFunctionalityProxy->getWordMixer();
    m_pWordPairOwner = m_pGameFunctionalityProxy->getWordPairOwner();
    m_pInputBuilder = m_pGameFunctionalityProxy->getInputBuilder();
    m_pStatisticsItem = m_pGameFunctionalityProxy->getStatisticsItem();
    m_pChronometer = m_pGameFunctionalityProxy->getChronometer();

    // all QObjects used by application (except the QML registered ones) should be parented (the non-parented ones would only be used in tests)
    Q_ASSERT(this->parent());
    Q_ASSERT(m_pDataSourceAccessHelper->parent());
    Q_ASSERT(m_pWordMixer->parent());
    Q_ASSERT(m_pWordPairOwner->parent());
    Q_ASSERT(m_pInputBuilder->parent());
    Q_ASSERT(m_pStatisticsItem->parent());
    Q_ASSERT(m_pChronometer->parent());

    bool connected{connect(m_pWordMixer, &WordMixer::newWordsPairMixed, this, &GameFacade::_onNewWordsPairMixed)};
    Q_ASSERT(connected);
    connected = connect(m_pWordPairOwner, &WordPairOwner::newWordsPairSetup, this, &GameFacade::newMixedWordsAvailable);
    Q_ASSERT(connected);
    connected = connect(m_pWordPairOwner, &WordPairOwner::piecesAddedToInputStateChanged, this, &GameFacade::_onPiecesAddedToInputStateChanged);
    Q_ASSERT(connected);
    connected = connect(m_pWordPairOwner, &WordPairOwner::persistentIndexChanged, this, &GameFacade::persistentPieceSelectionIndexChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::pieceAddedToInput, this, &GameFacade::_onPieceAddedToInput);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::piecesRemovedFromInput, this, &GameFacade::_onPiecesRemovedFromInput);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::inputReset, this, &GameFacade::inputChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::inputCompletionChanged, this, &GameFacade::completionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::persistentIndexesChanged, this, &GameFacade::persistentPiecesRemovalIndexesChanged);
    Q_ASSERT(connected);
    connected = connect(m_pStatisticsItem, &StatisticsItem::statisticsUpdated, this, &GameFacade::_onStatisticsUpdated);
    Q_ASSERT(connected);
    connected = connect(m_pChronometer, &Chronometer::enabledChanged, this, &GameFacade::_onChronometerEnabledChanged);
    Q_ASSERT(connected);
    connected = connect(m_pChronometer, &Chronometer::timeoutTriggered, this, &GameFacade::_onChronometerTimeoutTriggered);
    Q_ASSERT(connected);
    connected = connect(m_pChronometer, &Chronometer::refreshTriggered, this, &GameFacade::remainingTimeRefreshed);
    Q_ASSERT(connected);
    connected = connect(m_pGameFunctionalityProxy, &GameFunctionalityProxy::fetchDataForPrimaryLanguageFinished, this, &GameFacade::_onFetchDataForPrimaryLanguageFinished);
    Q_ASSERT(connected);
    connected = connect(m_pGameFunctionalityProxy, &GameFunctionalityProxy::fetchDataForSecondaryLanguageFinished, this, &GameFacade::_onFetchDataForSecondaryLanguageFinished);
    Q_ASSERT(connected);
    connected = connect(m_pGameFunctionalityProxy, &GameFunctionalityProxy::primaryLanguageDataSavingFinished, this, &GameFacade::_onPrimaryLanguageDataSavingFinished);
    Q_ASSERT(connected);
    connected = connect(m_pGameFunctionalityProxy, &GameFunctionalityProxy::dataSavingErrorOccured, this, &GameFacade::_onDataSavingErrorOccured);
    Q_ASSERT(connected);
}

void GameFacade::init()
{
    Q_ASSERT(m_GameLevel != Game::Levels::LEVEL_NONE);

    _pushCurrentGameLevel();

    if (m_CurrentStatusCode == Game::StatusCodes::NO_LANGUAGE_SET)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode);
    }
    else
    {
        qWarning("Status codes initialization error or wrong usage of the function");
    }
}

void GameFacade::startGame()
{
    if (!m_IsGameStarted)
    {
        Q_ASSERT(m_IsDataAvailable);

        m_pStatisticsItem->doInitialUpdate();

        if (m_pChronometer->isEnabled())
        {
            m_pChronometer->start();
        }

        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_STARTED);

        m_IsGameStarted = true;
    }
}

void GameFacade::resumeGame()
{
    Q_ASSERT(m_IsGamePaused);
    m_IsGamePaused = false;

    if (m_pChronometer->isEnabled())
    {
        m_pChronometer->resume();
    }

    Q_EMIT statusChanged(m_CurrentStatusCode = m_pInputBuilder->isInputComplete() ? Game::StatusCodes::GAME_RESUMED_COMPLETE_INPUT
                                                                                  : Game::StatusCodes::GAME_RESUMED_INCOMPLETE_INPUT);
}

void GameFacade::pauseGame()
{
    Q_ASSERT(m_IsGameStarted);
    m_IsGamePaused = true;

    if (m_pChronometer->isEnabled())
    {
        m_pChronometer->pause();
    }

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_PAUSED);
}

void GameFacade::quitGame()
{
    Q_ASSERT(m_IsGameStarted);
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_STOPPED);
}

void GameFacade::setGameLevel(Game::Levels level)
{
    Q_ASSERT(level != Game::Levels::LEVEL_NONE);

    if (m_GameLevel != level)
    {
        // always stop the chronometer prior to changing level (time limit is changed)
        if (m_pChronometer->isEnabled())
        {
            m_pChronometer->stop();
        }

        m_GameLevel = level;
        _pushCurrentGameLevel();

        m_pGameFunctionalityProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());

        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::LEVEL_CHANGED);

        // the chronometer should always run in main pane if enabled, so start it again once level params have been changed
        if (m_pChronometer->isEnabled())
        {
            m_pChronometer->start();
        }
    }
}

void GameFacade::setLanguage(int languageIndex, bool revertLanguageWhenDataUnavailable)
{
    if (m_CurrentLanguageIndex != languageIndex)
    {
        // pausing only allowed in the main pane
        if (m_pChronometer->isEnabled() && m_IsGameStarted)
        {
            m_pChronometer->pause();
        }

        m_PreviousLanguageIndex = m_CurrentLanguageIndex;
        m_CurrentLanguageIndex = languageIndex;
        m_ShouldRevertLanguageWhenDataUnavailable = revertLanguageWhenDataUnavailable;
        m_IsFetchingInProgress = true;

        Q_EMIT languageChanged();
        Q_EMIT fetchingInProgressChanged();
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::FETCHING_DATA);

        m_IsDataAvailable = false;

        Q_EMIT dataAvailableChanged();

        m_pGameFunctionalityProxy->fetchDataForPrimaryLanguage(languageIndex, !revertLanguageWhenDataUnavailable);
    }
}

void GameFacade::enableTimeLimit()
{
    m_pChronometer->enable();
}

void GameFacade::disableTimeLimit()
{
    m_pChronometer->disable();
}

void GameFacade::enablePersistentMode()
{
    if (!m_IsPersistentIndexModeEnabled)
    {
        if (!m_pInputBuilder->isInputComplete())
        {
            m_pWordPairOwner->setPersistentPieceSelectionIndex(m_pInputBuilder->isEmptyInput() || m_pInputBuilder->isHalfInput(),
                                                               m_pInputBuilder->isOneWordLeftInProgress() && !m_pWordPairOwner->isOnePieceLeftToAddToInput());
        }
        else
        {
            m_pInputBuilder->setPersistentPiecesRemovalIndex(Game::InputWordNumber::ONE);
        }

        m_pWordPairOwner->enableNewPairAutoIndexSetup(true);
        m_IsPersistentIndexModeEnabled = true;

        Q_EMIT persistentIndexModeEnabledChanged();
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PERSISTENT_MODE_ENTERED);
    }
}

void GameFacade::disablePersistentMode()
{
    if (m_IsPersistentIndexModeEnabled)
    {
        m_IsPersistentIndexModeEnabled = false;

        m_pWordPairOwner->enableNewPairAutoIndexSetup(false);
        m_pWordPairOwner->clearPersistentPieceSelectionIndex();
        m_pInputBuilder->clearPersistentPiecesRemovalIndexes();

        Q_EMIT persistentIndexModeEnabledChanged();
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PERSISTENT_MODE_EXITED);
    }
}

void GameFacade::goToNextPersistentModeContainer()
{
    if (m_IsPersistentIndexModeEnabled)
    {
        if (m_pWordPairOwner->getPersistentPieceSelectionIndex() != -1)
        {
            if (!m_pInputBuilder->isEmptyInput())
            {
                m_pWordPairOwner->clearPersistentPieceSelectionIndex();
                m_pInputBuilder->setPersistentPiecesRemovalIndex(Game::InputWordNumber::ONE);

                if(m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex() == -1)
                {
                    m_pInputBuilder->setPersistentPiecesRemovalIndex(Game::InputWordNumber::TWO);
                }
            }
        }
        else if (m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex() != -1)
        {
            m_pInputBuilder->clearPersistentPiecesRemovalIndexes();
            m_pInputBuilder->setPersistentPiecesRemovalIndex(Game::InputWordNumber::TWO);

            if (m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex() == -1)
            {
                m_pWordPairOwner->setPersistentPieceSelectionIndex(m_pInputBuilder->isHalfInput(),
                                                                   m_pInputBuilder->isOneWordLeftInProgress() && !m_pWordPairOwner->isOnePieceLeftToAddToInput());
            }
        }
        else if (m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex() != -1)
        {
            m_pInputBuilder->clearPersistentPiecesRemovalIndexes();
            if (!m_pInputBuilder->isInputComplete())
            {
                m_pWordPairOwner->setPersistentPieceSelectionIndex(m_pInputBuilder->isHalfInput(),
                                                                   m_pInputBuilder->isOneWordLeftInProgress() && !m_pWordPairOwner->isOnePieceLeftToAddToInput());
            }
            else
            {
                m_pInputBuilder->setPersistentPiecesRemovalIndex(Game::InputWordNumber::ONE);
            }
        }
        else
        {
            // there should always be an active persistent index while in persistent mode
            Q_ASSERT(false);
        }
    }
}

void GameFacade::increasePersistentIndex()
{
    if (m_IsPersistentIndexModeEnabled)
    {
        if (m_pWordPairOwner->getPersistentPieceSelectionIndex() != -1)
        {
            m_pWordPairOwner->increasePersistentPieceSelectionIndex();
        }
        else if (m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex() != -1 || m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex() != -1)
        {
            m_pInputBuilder->increasePersistentPiecesRemovalIndex();
        }
        else
        {
            // there should always be an active persistent index while in persistent mode
            Q_ASSERT(false);
        }
    }
}

void GameFacade::decreasePersistentIndex()
{
    if (m_IsPersistentIndexModeEnabled)
    {
        if (m_pWordPairOwner->getPersistentPieceSelectionIndex() != -1)
        {
            m_pWordPairOwner->decreasePersistentPieceSelectionIndex();
        }
        else if (m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex() != -1 || m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex() != -1)
        {
            m_pInputBuilder->decreasePersistentPiecesRemovalIndex();
        }
        else
        {
            // there should always be an active persistent index while in persistent mode
            Q_ASSERT(false);
        }
    }
}

void GameFacade::executeFirstPersistentModeAction()
{
    if (m_IsPersistentIndexModeEnabled)
    {
        // there should always be an active persistent index while in persistent mode
        Q_ASSERT(m_pWordPairOwner->getPersistentPieceSelectionIndex() != -1 ||
                 m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex() != -1 || m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex() != -1);

        if (m_pWordPairOwner->getPersistentPieceSelectionIndex() != -1)
        {
            _addPieceToInputWord(Game::InputWordNumber::ONE, m_pWordPairOwner->getPersistentPieceSelectionIndex());
        }
        else
        {
            _removePiecesFromInputWordInPersistentMode();
        }
    }
}

void GameFacade::executeSecondPersistentModeAction()
{
    if (m_IsPersistentIndexModeEnabled)
    {
        // there should always be an active persistent index while in persistent mode
        Q_ASSERT(m_pWordPairOwner->getPersistentPieceSelectionIndex() != -1 ||
                 m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex() != -1 || m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex() != -1);

        if (m_pWordPairOwner->getPersistentPieceSelectionIndex() != -1)
        {
            _addPieceToInputWord(Game::InputWordNumber::TWO, m_pWordPairOwner->getPersistentPieceSelectionIndex());
        }
        else
        {
            _removePiecesFromInputWordInPersistentMode();
        }
    }
}

void GameFacade::addPieceToInputWord(Game::InputWordNumber inputWordNumber, int wordPieceIndex)
{
    if (!m_IsPersistentIndexModeEnabled)
    {
        if (!m_pWordPairOwner->getIsWordPieceAddedToInput(wordPieceIndex))
        {
            _addPieceToInputWord(inputWordNumber, wordPieceIndex);
        }
    }
    else
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PERSISTENT_INDEX_REQUIRED);
    }
}

void GameFacade::removePiecesFromInputWord(Game::InputWordNumber inputWordNumber, int inputRangeStart)
{
    if (!m_IsPersistentIndexModeEnabled)
    {
        if (m_pInputBuilder->removePiecesFromInputWord(inputWordNumber, inputRangeStart))
        {
            Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PIECES_REMOVED);
        }
    }
    else
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PERSISTENT_INDEX_REQUIRED);
    }
}

void GameFacade::clearInputWord(Game::InputWordNumber inputWordNumber)
{
    if (m_pInputBuilder->removePiecesFromInputWord(inputWordNumber, 0))
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PIECES_REMOVED);
    }
}

void GameFacade::clearInput()
{
    if (m_pInputBuilder->clearInput())
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::USER_INPUT_CLEARED);
    }
}

void GameFacade::handleSubmitRequest()
{
    QVector<QString> mixedWordPiecesContent{m_pWordPairOwner->getMixedWordsPiecesContent()};

    QString firstInputWord;
    QString secondInputWord;

    for (auto index : m_pInputBuilder->getFirstWordInputIndexes())
    {
        firstInputWord.append(mixedWordPiecesContent.at(index));
    }

    for (auto index : m_pInputBuilder->getSecondWordInputIndexes())
    {
        secondInputWord.append(mixedWordPiecesContent.at(index));
    }

    bool success{(firstInputWord == m_pWordPairOwner->getFirstReferenceWord() && secondInputWord == m_pWordPairOwner->getSecondReferenceWord()) ||
                 (firstInputWord == m_pWordPairOwner->getSecondReferenceWord() && secondInputWord == m_pWordPairOwner->getFirstReferenceWord())};

    Q_EMIT statusChanged(m_CurrentStatusCode = success ? Game::StatusCodes::CORRECT_USER_INPUT : Game::StatusCodes::INCORRECT_USER_INPUT);

    if (success)
    {
        m_pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::FULL_UPDATE);
        m_pGameFunctionalityProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());

        if (m_pChronometer->isEnabled())
        {
            m_pChronometer->restart();
        }
    }
}

void GameFacade::provideCorrectWordsPairToUser()
{
    m_pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::SOLUTION_REQUESTED_BY_USER);
    m_pGameFunctionalityProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());

    if (m_pChronometer->isEnabled())
    {
        m_pChronometer->restart();
    }
}

void GameFacade::resetGameStatistics()
{
    m_pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::RESET);
}

void GameFacade::handleDataSavingOperationInProgress()
{
    Q_EMIT statusChanged(m_CurrentStatusCode = m_IsDataAvailable ? Game::StatusCodes::ADDITIONAL_DATA_SAVE_IN_PROGRESS : Game::StatusCodes::NEW_DATA_SAVE_IN_PROGRESS);
}

bool GameFacade::isDataFetchingInProgress() const
{
    return m_IsFetchingInProgress;
}

bool GameFacade::isDataAvailable() const
{
    return m_IsDataAvailable;
}

bool GameFacade::isPersistentModeEnabled() const
{
    return m_IsPersistentIndexModeEnabled;
}

bool GameFacade::isInputComplete() const
{
    return m_pInputBuilder->isInputComplete();
}

bool GameFacade::areWordsFromCurrentPairSynonyms() const
{
    return m_pWordPairOwner->areSynonyms();
}

bool GameFacade::isTimeLimitEnabled() const
{
    return m_pChronometer->isEnabled();
}

bool GameFacade::canResetGameStatistics() const
{
    return m_pStatisticsItem->canResetStatistics();
}

int GameFacade::getCurrentLanguageIndex() const
{
    return m_CurrentLanguageIndex;
}

QVector<QString> GameFacade::getMixedWordsPiecesContent() const
{
    return m_pWordPairOwner->getMixedWordsPiecesContent();
}

QVector<Game::PieceTypes> GameFacade::getMixedWordsPiecesTypes() const
{
    return m_pWordPairOwner->getMixedWordsPiecesTypes();
}

QVector<bool> GameFacade::getAreMixedWordsPiecesSelected() const
{
    return m_pWordPairOwner->getAreMixedWordsPiecesAddedToInput();
}

const QVector<int> GameFacade::getFirstWordInputIndexes() const
{
    return m_pInputBuilder->getFirstWordInputIndexes();
}

const QVector<int> GameFacade::getSecondWordInputIndexes() const
{
    return m_pInputBuilder->getSecondWordInputIndexes();
}

QString GameFacade::getFirstReferenceWord() const
{
    return m_pWordPairOwner->getFirstReferenceWord();
}

QString GameFacade::getSecondReferenceWord() const
{
    return m_pWordPairOwner->getSecondReferenceWord();
}

int GameFacade::getPersistentPieceSelectionIndex() const
{
    return m_pWordPairOwner->getPersistentPieceSelectionIndex();
}

int GameFacade::getFirstPersistentPiecesRemovalIndex() const
{
    return m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex();
}

int GameFacade::getSecondPersistentPiecesRemovalIndex() const
{
    return m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex();
}

QPair<QString, QString> GameFacade::getRemainingTime() const
{
    return m_pChronometer->getRemainingTimeMinSec();
}

QString GameFacade::getObtainedScore() const
{
    return m_pStatisticsItem->getObtainedScore();
}

QString GameFacade::getTotalAvailableScore() const
{
    return m_pStatisticsItem->getTotalAvailableScore();
}

QString GameFacade::getGuessedWordPairs() const
{
    return m_pStatisticsItem->getGuessedWordPairs();
}

QString GameFacade::getTotalWordPairs() const
{
    return m_pStatisticsItem->getTotalWordPairs();
}

void GameFacade::_onFetchDataForPrimaryLanguageFinished(bool success, bool validEntriesFetched)
{
    if (success)
    {
        m_IsFetchingInProgress = false;
        Q_EMIT fetchingInProgressChanged();

        if (validEntriesFetched)
        {
            m_pDataSourceAccessHelper->setEntriesTable(m_pGameFunctionalityProxy->getNrOfDataSourceEntries());
            _connectToDataSource();
            m_pGameFunctionalityProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());
            m_IsDataAvailable = true;

            Q_EMIT dataAvailableChanged();
            Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_FETCHING_COMPLETE);

            // restarting only allowed in the main pane
            if (m_pChronometer->isEnabled() && m_IsGameStarted)
            {
                m_pChronometer->restart();
            }
        }
        else
        {
            if (m_ShouldRevertLanguageWhenDataUnavailable)
            {
                m_CurrentLanguageIndex = m_PreviousLanguageIndex;
                Q_EMIT languageChanged();

                if (m_pChronometer->isEnabled())
                {
                    m_pChronometer->resume();
                }
            }
            else
            {
                // ensure no residual entries from a previously selected language exist in intro pane
                m_pDataSourceAccessHelper->clearEntriesTable();
            }

            Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::NO_DATA_ENTRIES_FETCHED);
        }
    }
    else
    {
        if (m_pChronometer->isEnabled())
        {
            m_pChronometer->disable();
        }

        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_FETCHING_ERROR);
    }
}

void GameFacade::_onFetchDataForSecondaryLanguageFinished(bool success)
{
    if (!success)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_FETCHING_ERROR);
    }
}

void GameFacade::_onEntryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    m_pWordMixer->mixWords(newWordsPair, areSynonyms);
}

void GameFacade::_onNewWordsPairMixed()
{
    m_pInputBuilder->resetInput();

    m_pWordPairOwner->setNewWordsPair(m_pWordMixer->getMixedWordsPiecesContent(),
                                      m_pWordMixer->getFirstWord(),
                                      m_pWordMixer->getSecondWord(),
                                      m_pWordMixer->areSynonyms(),
                                      m_pWordMixer->getFirstWordFirstPieceIndex(),
                                      m_pWordMixer->getFirstWordLastPieceIndex(),
                                      m_pWordMixer->getSecondWordFirstPieceIndex(),
                                      m_pWordMixer->getSecondWordLastPieceIndex());

}

void GameFacade::_onPiecesAddedToInputStateChanged()
{
    m_pInputBuilder->setCloseInputAllowed(m_pWordPairOwner->isOnePieceLeftToAddToInput());
    Q_EMIT piecesAddedToInputChanged();
}

void GameFacade::_onPieceAddedToInput(int index)
{
    if (m_IsPersistentIndexModeEnabled)
    {
        // a small hack: we exclude the end pieces from being marked by persistent selection index if only part of a word is left to add to input ...
        m_pWordPairOwner->markPieceAsAddedToInput(index, m_pInputBuilder->isOneWordLeftInProgress());

        if (m_pInputBuilder->isHalfInput() && (m_pWordPairOwner->getPersistentPieceSelectionIndex() != -1))
        {
            m_pWordPairOwner->clearPersistentPieceSelectionIndex();
            m_pWordPairOwner->setPersistentPieceSelectionIndex(true);
        }
        else if (m_pWordPairOwner->isOnePieceLeftToAddToInput())
        {
            // ... but if only the end piece is left to add we re-activate the persistent selection index which marks this piece
            m_pWordPairOwner->setPersistentPieceSelectionIndex(false);
        }
        else if (m_pInputBuilder->isInputComplete())
        {
            m_pInputBuilder->setPersistentPiecesRemovalIndex(Game::InputWordNumber::ONE);
        }
    }
    else
    {
        m_pWordPairOwner->markPieceAsAddedToInput(index);
    }

    Q_EMIT inputChanged();
}

void GameFacade::_onPiecesRemovedFromInput(QVector<int> indexes)
{
    m_pWordPairOwner->markPiecesAsRemovedFromInput(indexes);

    if (m_IsPersistentIndexModeEnabled)
    {
        m_pWordPairOwner->clearPersistentPieceSelectionIndex();
        m_pWordPairOwner->setPersistentPieceSelectionIndex(m_pInputBuilder->isEmptyInput() || m_pInputBuilder->isHalfInput(),
                                                           m_pInputBuilder->isOneWordLeftInProgress() && !m_pWordPairOwner->isOnePieceLeftToAddToInput());
    }

    Q_EMIT inputChanged();
}

void GameFacade::_onChronometerTimeoutTriggered()
{
    m_pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
    m_pGameFunctionalityProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());
    Q_EMIT statusChanged(Game::StatusCodes::TIME_LIMIT_REACHED);
    m_pChronometer->restart();
}

void GameFacade::_onChronometerEnabledChanged()
{
    if (m_pChronometer->isEnabled())
    {
        m_pStatisticsItem->setEnhancedIncrement(true);
        if (m_IsGameStarted)
        {
            m_pChronometer->start();
        }

        Q_EMIT statusChanged(Game::StatusCodes::TIME_LIMIT_ENABLED);
    }
    else
    {
        m_pStatisticsItem->setEnhancedIncrement(false);
        Q_EMIT statusChanged(Game::StatusCodes::TIME_LIMIT_DISABLED);
    }

    Q_EMIT timeLimitEnabledChanged();
}

void GameFacade::_onStatisticsUpdated()
{
    // for full and partial update no status message required (there are already status messages in the operations that trigger the full/partial update of statistics)
    if (m_pStatisticsItem->getLastUpdateOperation() == StatisticsItem::StatisticsUpdateOperations::RESET)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = m_pInputBuilder->isInputComplete() ? Game::StatusCodes::STATISTICS_RESET_COMPLETE_INPUT
                                                                                      : Game::StatusCodes::STATISTICS_RESET_INCOMPLETE_INPUT);
    }

    Q_EMIT statisticsChanged();
}

void GameFacade::_onPrimaryLanguageDataSavingFinished(int nrOfPrimaryLanguageSavedEntries)
{
    const int initialNrOfPrimarySourceEntries{m_pDataSourceAccessHelper->getTotalNrOfEntries()};

    if (initialNrOfPrimarySourceEntries > 0)
    {
        m_pDataSourceAccessHelper->addEntriesToTable(nrOfPrimaryLanguageSavedEntries);
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_SUCCESSFULLY_SAVED);
    }
    else if (nrOfPrimaryLanguageSavedEntries > 0)
    {
        m_pDataSourceAccessHelper->addEntriesToTable(nrOfPrimaryLanguageSavedEntries);
        m_IsDataAvailable = true;
        _connectToDataSource();
        m_pGameFunctionalityProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());

        Q_EMIT dataAvailableChanged();
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_GOT_AVAILABLE);
    }
    else
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_STILL_UNAVAILABLE);
    }
}

void GameFacade::_onDataSavingErrorOccured()
{
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_SAVING_ERROR);
}

// should only be called once when switching to a language that either has word pairs in the database or pairs have just been added for it to the database
void GameFacade::_connectToDataSource()
{
    if (!m_IsConnectedToDataSource)
    {
        m_IsConnectedToDataSource =  connect(m_pGameFunctionalityProxy, &GameFunctionalityProxy::entryProvidedToConsumer, this, &GameFacade::_onEntryProvidedToConsumer);
        Q_ASSERT(m_IsConnectedToDataSource);
    }
}

void GameFacade::_pushCurrentGameLevel()
{
    m_pWordMixer->setGameLevel(m_GameLevel);
    m_pStatisticsItem->setGameLevel(m_GameLevel);
    m_pChronometer->setTotalCountdownTime(Game::c_TimeLimits[m_GameLevel]);
}

void GameFacade::_addPieceToInputWord(Game::InputWordNumber inputWordNumber, int wordPieceIndex)
{
    // adding piece should always occur before updating status
    Game::PieceTypes pieceType{m_pWordPairOwner->getWordPieceType(wordPieceIndex)};
    bool pieceAdded{m_pInputBuilder->addPieceToInputWord(inputWordNumber, wordPieceIndex, pieceType)};

    Q_EMIT statusChanged(m_CurrentStatusCode = pieceAdded ? (m_pInputBuilder->isInputComplete() ? Game::StatusCodes::PIECE_ADDED_COMPLETE_INPUT
                                                                                                : Game::StatusCodes::PIECE_ADDED_INCOMPLETE_INPUT)
                                                          : Game::StatusCodes::PIECE_NOT_ADDED);
}

void GameFacade::_removePiecesFromInputWordInPersistentMode()
{
    if (m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex() != -1)
    {
        if (m_pInputBuilder->removePiecesFromInputWord(Game::InputWordNumber::ONE, m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex()))
        {
            m_pWordPairOwner->setPersistentPieceSelectionIndex(m_pInputBuilder->isEmptyInput() || m_pInputBuilder->isHalfInput());
            Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PIECES_REMOVED);
        }
    }
    else if (m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex() != -1)
    {
        if (m_pInputBuilder->removePiecesFromInputWord(Game::InputWordNumber::TWO, m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex()))
        {
            m_pWordPairOwner->setPersistentPieceSelectionIndex(m_pInputBuilder->isEmptyInput() || m_pInputBuilder->isHalfInput());
            Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PIECES_REMOVED);
        }
    }
}
