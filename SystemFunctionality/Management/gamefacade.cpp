#include "gamefacade.h"
#include "../CoreFunctionality/wordmixer.h"
#include "../CoreFunctionality/wordpairowner.h"
#include "../CoreFunctionality/inputbuilder.h"
#include "../DataAccess/datasource.h"
#include "../DataAccess/datasourceaccesshelper.h"
#include "../Proxies/gamefunctionalityproxy.h"
#include "../Proxies/datasourceproxy.h"
#include "../Utilities/scoreitem.h"
#include "../Utilities/gamestrings.h"

GameFacade::GameFacade(QObject *parent)
    : QObject(parent)
    , m_pGameFunctionalityProxy{new GameFunctionalityProxy{this}}
    , m_CurrentStatusCode{Game::StatusCodes::NO_DATA_LOADING_REQUESTED}
    , m_IsDataAvailable{false}
    , m_IsDataEntryAllowed{false}
    , m_IsAddingToCacheAllowed{true}
    , m_IsResettingCacheAllowed{false}
    , m_IsSavingToDbAllowed{false}
    , m_IsSavingInProgress{false}
    , m_IsGameStarted{false}
    , m_IsGamePaused{false}
    , m_IsPersistentIndexModeEnabled{false}
{
    m_pDataSourceProxy = m_pGameFunctionalityProxy->getDataSourceProxy();
    m_pDataSourceAccessHelper = m_pGameFunctionalityProxy->getDataSourceAccessHelper();
    m_pWordMixer = m_pGameFunctionalityProxy->getWordMixer();
    m_pWordPairOwner = m_pGameFunctionalityProxy->getWordPairOwner();
    m_pInputBuilder = m_pGameFunctionalityProxy->getInputBuilder();
    m_pScoreItem = m_pGameFunctionalityProxy->getScoreItem();

    // all QObjects used by application (except the QML registered ones) should be parented (the non-parented ones would only be used in tests)
    Q_ASSERT(this->parent());
    Q_ASSERT(m_pDataSourceProxy->parent());
    Q_ASSERT(m_pDataSourceAccessHelper->parent());
    Q_ASSERT(m_pWordMixer->parent());
    Q_ASSERT(m_pWordPairOwner->parent());
    Q_ASSERT(m_pInputBuilder->parent());
    Q_ASSERT(m_pScoreItem->parent());

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
    connected = connect(m_pScoreItem, &ScoreItem::statisticsUpdated, this, &GameFacade::_onStatisticsUpdated);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::loadDataFromDbFinished, this, &GameFacade::_onLoadDataFromDbFinished);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::writeDataToDbErrorOccured, this, &GameFacade::_onWriteDataToDbErrorOccured);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::newWordsPairAddedToCache, this, &GameFacade::_onNewWordsPairAddedToCache);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::wordsPairAlreadyContainedInCache, this, &GameFacade::_onWordsPairAlreadyContainedInCache);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::cacheReset, this, &GameFacade::_onCacheReset);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::addInvalidWordsPairRequested, this, &GameFacade::_onAddInvalidWordsPairRequested);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::writeDataToDbFinished, this, &GameFacade::_onWriteDataToDbFinished);
    Q_ASSERT(connected);

}

void GameFacade::init()
{
    if (m_CurrentStatusCode == Game::StatusCodes::NO_DATA_LOADING_REQUESTED)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::LOADING_DATA);
        m_pDataSourceProxy->loadDataFromDb();
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

        m_pDataSourceProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());
        m_pScoreItem->initStatistics();

        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_STARTED);

        m_IsGameStarted = true;
    }
}

void GameFacade::resumeGame()
{
    Q_ASSERT(m_IsGamePaused);
    m_IsGamePaused = false;

    Q_EMIT statusChanged(m_CurrentStatusCode = m_pInputBuilder->isInputComplete() ? Game::StatusCodes::GAME_RESUMED_COMPLETE_INPUT
                                                                                  : Game::StatusCodes::GAME_RESUMED_INCOMPLETE_INPUT);
}

void GameFacade::pauseGame()
{
    Q_ASSERT(m_IsGameStarted);
    m_IsGamePaused = true;

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_PAUSED);
}

void GameFacade::quitGame()
{
    Q_ASSERT(m_IsGameStarted);
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_STOPPED);
}

void GameFacade::startWordEntry()
{
    if (!m_IsSavingInProgress)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_STARTED);
    }
}

void GameFacade::resumeWordEntry()
{
    if (!m_IsSavingInProgress)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_RESUMED);
    }
}

void GameFacade::enablePersistentPieceSelection()
{
    if (m_IsPersistentIndexModeEnabled)
    {
        m_pInputBuilder->clearPersistentPiecesRemovalIndexes();
    }
    else
    {
        m_pWordPairOwner->enableNewPairAutoIndexSetup(true);
        m_IsPersistentIndexModeEnabled = true;

        Q_EMIT persistentIndexModeEnabledChanged();
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PERSISTENT_MODE_ENTERED);
    }

    if (!m_pInputBuilder->isInputComplete())
    {
        m_pWordPairOwner->setPersistentPieceSelectionIndex(m_pInputBuilder->isEmptyInput() || m_pInputBuilder->isHalfInput());
    }
}

void GameFacade::increasePersistentPieceSelectionIndex()
{
    m_pWordPairOwner->increasePersistentPieceSelectionIndex();
}

void GameFacade::decreasePersistentPieceSelectionIndex()
{
    m_pWordPairOwner->decreasePersistentPieceSelectionIndex();
}

void GameFacade::enablePersistentPiecesRemoval(Game::InputWordNumber inputWordNumber)
{
    if (m_IsPersistentIndexModeEnabled)
    {
        m_pWordPairOwner->clearPersistentPieceSelectionIndex();
    }
    else
    {
        m_IsPersistentIndexModeEnabled = true;

        Q_EMIT persistentIndexModeEnabledChanged();
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PERSISTENT_MODE_ENTERED);
    }

    m_pInputBuilder->setPersistentPiecesRemovalIndex(inputWordNumber);
}

void GameFacade::increasePersistentPiecesRemovalIndex()
{
    m_pInputBuilder->increasePersistentPiecesRemovalIndex();
}

void GameFacade::decreasePersistentPiecesRemovalIndex()
{
    m_pInputBuilder->decreasePersistentPiecesRemovalIndex();
}

void GameFacade::disablePersistentIndexes()
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

void GameFacade::addPieceToInputWordInPersistentMode(Game::InputWordNumber inputWordNumber)
{
    if (m_IsPersistentIndexModeEnabled)
    {
        int persistentIndex{m_pWordPairOwner->getPersistentPieceSelectionIndex()};

        if (persistentIndex != -1)
        {
            _addPieceToInputWord(inputWordNumber, persistentIndex);
        }
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

void GameFacade::removePiecesFromInputWordInPersistentMode(Game::InputWordNumber inputWordNumber)
{
    if (m_IsPersistentIndexModeEnabled)
    {
        int persistentIndex{inputWordNumber == Game::InputWordNumber::ONE ? m_pInputBuilder->getFirstWordPersistentPiecesRemovalIndex()
                                                                          : m_pInputBuilder->getSecondWordPersistentPiecesRemovalIndex()};
        if (persistentIndex != -1)
        {
            if (m_pInputBuilder->removePiecesFromInputWord(inputWordNumber, persistentIndex))
            {
                m_pWordPairOwner->setPersistentPieceSelectionIndex(m_pInputBuilder->isEmptyInput() || m_pInputBuilder->isHalfInput());
                Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PIECES_REMOVED);
            }
        }
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
        m_pScoreItem->updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);
        m_pDataSourceProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());
    }
}

void GameFacade::requestAddPairToCache(const QString &firstWord, const QString &secondWord, bool areSynonyms)
{
    if (m_IsDataEntryAllowed && m_IsAddingToCacheAllowed)
    {
        _blockAddToCache();
        m_pDataSourceProxy->requestWriteToCache(QPair<QString, QString>{firstWord, secondWord}, areSynonyms);
    }
}

void GameFacade::requestSaveDataToDb()
{
    if (m_IsDataEntryAllowed && m_IsSavingToDbAllowed)
    {
        _blockAddToCache();
        _blockSaveToDb();
        _blockCacheReset();

        m_IsSavingInProgress = true;
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_SAVE_IN_PROGRESS);

        m_pDataSourceProxy->saveDataToDb();
    }
}

void GameFacade::requestCacheReset()
{
    if (m_IsDataEntryAllowed && m_IsResettingCacheAllowed)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::RESET_CACHE_REQUESTED);

        _blockSaveToDb();
        _blockCacheReset();
        m_pDataSourceProxy->requestCacheReset();
    }
}

void GameFacade::provideCorrectWordsPairToUser()
{
    m_pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::SOLUTION_REQUESTED_BY_USER);
    m_pDataSourceProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());
}

void GameFacade::setLevel(Game::Level level)
{
    m_pWordMixer->setWordPieceSize(level);
    m_pScoreItem->setScoreIncrement(level);

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::LEVEL_CHANGED);

    m_pDataSourceProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());
}

void GameFacade::resetGameStatistics()
{
    m_pScoreItem->updateStatistics(Game::StatisticsUpdate::RESET);
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

bool GameFacade::isPersistentModeEnabled() const
{
    return m_IsPersistentIndexModeEnabled;
}

bool GameFacade::isInputComplete() const
{
    return m_pInputBuilder->isInputComplete();
}

QString GameFacade::getFirstReferenceWord() const
{
    return m_pWordPairOwner->getFirstReferenceWord();
}

QString GameFacade::getSecondReferenceWord() const
{
    return m_pWordPairOwner->getSecondReferenceWord();
}

int GameFacade::getObtainedScore() const
{
    return m_pScoreItem->getObtainedScore();
}

int GameFacade::getTotalAvailableScore() const
{
    return m_pScoreItem->getTotalAvailableScore();
}

int GameFacade::getGuessedWordPairs() const
{
    return m_pScoreItem->getGuessedWordPairs();
}

int GameFacade::getTotalWordPairs() const
{
    return m_pScoreItem->getTotalWordPairs();
}

int GameFacade::getCurrentNrOfAddedWordPairs() const
{
    return m_pDataSourceProxy->getCurrentNrOfCacheEntries();
}

int GameFacade::getLastSavedNrOfWordPairs() const
{
    return m_pDataSourceProxy->getLastSavedNrOfCacheEntries();
}

Game::ValidationCodes GameFacade::getDataEntryValidationCode() const
{
    return m_pDataSourceProxy->getPairEntryValidationCode();
}

bool GameFacade::isDataAvailable() const
{
    return m_IsDataAvailable;
}

bool GameFacade::isDataEntryAllowed() const
{
    return m_IsDataEntryAllowed;
}

bool GameFacade::isAddingToCacheAllowed() const
{
    return m_IsAddingToCacheAllowed;
}

bool GameFacade::isCacheResetAllowed() const
{
    return m_IsResettingCacheAllowed;
}

bool GameFacade::isSavingToDbAllowed() const
{
    return m_IsSavingToDbAllowed;
}

bool GameFacade::areSynonyms() const
{
    return m_pWordPairOwner->areSynonyms();
}

void GameFacade::_onLoadDataFromDbFinished(bool success)
{
    if (success)
    {
        int nrOfEntries{m_pDataSourceProxy->getNrOfValidDataSourceEntries()};

        if (nrOfEntries != 0)
        {
            m_pDataSourceAccessHelper->setEntriesTable(nrOfEntries);
            _connectToDataSource();

            Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_LOADING_COMPLETE);
        }
        else
        {
            Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::NO_VALID_DATA_ENTRIES_LOADED);
        }

        m_IsDataEntryAllowed = true;
        Q_EMIT dataEntryAllowedChanged();
    }
    else
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_LOADING_ERROR);
    }
}

void GameFacade::_onEntryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    m_pWordMixer->mixWords(newWordsPair, areSynonyms);
}

void GameFacade::_onAddInvalidWordsPairRequested()
{
    // restore add to cache capability so the user can re-add the entry after modifying the words
    _allowAddToCache();

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_ADD_INVALID);
}

void GameFacade::_onNewWordsPairAddedToCache()
{
    _allowAddToCache();
    _allowCacheReset();
    _allowSaveToDb();

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_ADD_SUCCESS);
}

void GameFacade::_onWordsPairAlreadyContainedInCache()
{
    _allowAddToCache();
    _allowCacheReset();
    _allowSaveToDb();

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PAIR_ALREADY_ADDED);
}

void GameFacade::_onCacheReset()
{
    _allowAddToCache();

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::CACHE_RESET);
}

void GameFacade::_onWriteDataToDbFinished(int nrOfEntries)
{
    int initialNrOfEntries{m_pDataSourceAccessHelper->getTotalNrOfEntries()};

    m_pDataSourceAccessHelper->addEntriesToTable(nrOfEntries);

    if (initialNrOfEntries == 0)
    {
        _connectToDataSource();
    }

    _allowAddToCache();
    m_IsSavingInProgress = false;

    Q_EMIT statusChanged(m_CurrentStatusCode = initialNrOfEntries == 0 ? Game::StatusCodes::DATA_GOT_AVAILABLE : Game::StatusCodes::DATA_SUCCESSFULLY_SAVED);
}

void GameFacade::_onWriteDataToDbErrorOccured()
{
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_SAVING_ERROR);
}

void GameFacade::_onPiecesAddedToInputStateChanged()
{
    m_pInputBuilder->setCloseInputAllowed(m_pWordPairOwner->isOnePieceLeftToAddToInput());
    Q_EMIT piecesAddedToInputChanged();
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

void GameFacade::_onPieceAddedToInput(int index)
{
    m_pWordPairOwner->markPieceAsAddedToInput(index);

    if (m_pInputBuilder->isHalfInput() && (m_pWordPairOwner->getPersistentPieceSelectionIndex() != -1))
    {
        m_pWordPairOwner->clearPersistentPieceSelectionIndex();
        m_pWordPairOwner->setPersistentPieceSelectionIndex(true);
    }

    Q_EMIT inputChanged();
}

void GameFacade::_onPiecesRemovedFromInput(QVector<int> indexes)
{
    m_pWordPairOwner->markPiecesAsRemovedFromInput(indexes);

    if (m_IsPersistentIndexModeEnabled)
    {
        m_pWordPairOwner->clearPersistentPieceSelectionIndex();
        m_pWordPairOwner->setPersistentPieceSelectionIndex(m_pInputBuilder->isEmptyInput() || m_pInputBuilder->isHalfInput());
    }

    Q_EMIT inputChanged();
}

void GameFacade::_onStatisticsUpdated(Game::StatisticsUpdate updateType)
{
    // for full and partial update no status message required (there are already status messages in the operations that trigger the full/partial update of statistics)
    if (updateType == Game::StatisticsUpdate::RESET)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = m_pInputBuilder->isInputComplete() ? Game::StatusCodes::STATISTICS_RESET_COMPLETE_INPUT
                                                                                      : Game::StatusCodes::STATISTICS_RESET_INCOMPLETE_INPUT);
    }

    Q_EMIT statisticsChanged();
}

void GameFacade::_connectToDataSource()
{
    bool connected{connect(m_pDataSourceProxy, &DataSourceProxy::entryProvidedToConsumer, this, &GameFacade::_onEntryProvidedToConsumer)};
    Q_ASSERT(connected);

    m_IsDataAvailable = true;
    Q_EMIT dataAvailableChanged();
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

void GameFacade::_allowAddToCache()
{
    if (!m_IsAddingToCacheAllowed)
    {
        m_IsAddingToCacheAllowed = true;
        Q_EMIT addPairToCacheAllowedChanged();
    }
}

void GameFacade::_blockAddToCache()
{
    if (m_IsAddingToCacheAllowed)
    {
        m_IsAddingToCacheAllowed = false;
        Q_EMIT addPairToCacheAllowedChanged();
    }
}

void GameFacade::_allowCacheReset()
{
    if (!m_IsResettingCacheAllowed)
    {
        m_IsResettingCacheAllowed = true;
        Q_EMIT resetCacheAllowedChanged();
    }
}

void GameFacade::_blockCacheReset()
{
    if (m_IsResettingCacheAllowed)
    {
        m_IsResettingCacheAllowed = false;
        Q_EMIT resetCacheAllowedChanged();
    }
}

void GameFacade::_allowSaveToDb()
{
    if (!m_IsSavingToDbAllowed)
    {
        m_IsSavingToDbAllowed = true;
        Q_EMIT saveNewPairsToDbAllowedChanged();
    }
}

void GameFacade::_blockSaveToDb()
{
    if (m_IsSavingToDbAllowed)
    {
        m_IsSavingToDbAllowed = false;
        Q_EMIT saveNewPairsToDbAllowedChanged();
    }
}
