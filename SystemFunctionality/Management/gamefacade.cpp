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
    , m_CachedWordPairs{0}
    , m_IsSavingToDbAllowed{false}
    , m_IsGameStarted{false}
    , m_IsGamePaused{false}
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

    bool connected{connect(m_pWordPairOwner, &WordPairOwner::mixedWordsAvailable, this, &GameFacade::mixedWordsChanged)};
    Q_ASSERT(connected);
    connected = connect(m_pWordPairOwner, &WordPairOwner::selectionChanged, this, &GameFacade::selectionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::inputChanged, this, &GameFacade::inputChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::inputCompletionChanged, this, &GameFacade::completionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pInputBuilder, &InputBuilder::closeInputPermissionRequested, this, &GameFacade::_onCloseInputPermissionRequested);
    Q_ASSERT(connected);
    connected = connect(m_pScoreItem, &ScoreItem::statisticsUpdated, this, &GameFacade::statisticsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::readDataFromDbFinished, this, &GameFacade::_onReadDataFromDbFinished);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::writeDataToDbErrorOccured, this, &GameFacade::_onWriteDataToDbErrorOccured);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::newWordsPairAddedToCache, this, &GameFacade::_onNewWordsPairAddedToCache);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::cacheReset, this, &GameFacade::_onCacheReset);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::invalidWordsPairAddedByUser, this, &GameFacade::_onInvalidWordsPairAddedByUser);
    Q_ASSERT(connected);
    connected = connect(m_pDataSourceProxy, &DataSourceProxy::writeDataToDbFinished, this, &GameFacade::_onWriteDataToDbFinished);
    Q_ASSERT(connected);
}

void GameFacade::init()
{
    if (m_CurrentStatusCode == Game::StatusCodes::NO_DATA_LOADING_REQUESTED)
    {
        m_pDataSourceProxy->loadDataFromDb();
        Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::LOADING_DATA);
    }
    else
    {
        qWarning("Status codes initialization error or wrong usage of the function");
    }
}

void GameFacade::startGame()
{
    Q_ASSERT(m_IsDataAvailable);

    m_pDataSourceProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());

    Q_EMIT statisticsChanged();
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::GAME_STARTED);

    m_IsGameStarted = true;
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
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_STARTED);
}

void GameFacade::resumeWordEntry()
{
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_RESUMED);
}

void GameFacade::addWordPieceToInputWord(Game::InputWordNumber inputWordNumber, int wordPieceIndex)
{
    if (!m_pWordPairOwner->getIsWordPieceSelected(wordPieceIndex))
    {
        // adding piece should always occur before updating status
        Game::PieceTypes pieceType{m_pWordPairOwner->getWordPieceType(wordPieceIndex)};
        bool pieceAdded{m_pInputBuilder->addPieceToInputWord(inputWordNumber, wordPieceIndex, pieceType)};

        Q_EMIT statusChanged(m_CurrentStatusCode = pieceAdded ? (m_pInputBuilder->isInputComplete() ? Game::StatusCodes::PIECE_ADDED_COMPLETE_INPUT
                                                                                                    : Game::StatusCodes::PIECE_ADDED_INCOMPLETE_INPUT)
                                                               : Game::StatusCodes::PIECE_NOT_ADDED);
    }
}

void GameFacade::removeWordPiecesFromInputWord(Game::InputWordNumber inputWordNumber, int inputRangeStart)
{
    m_pInputBuilder->removePiecesFromInputWord(inputWordNumber, inputRangeStart);
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::PIECES_REMOVED);
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

        m_pDataSourceProxy->saveDataToDb();
    }
}

void GameFacade::requestCacheReset()
{
    if (m_IsDataEntryAllowed && m_IsResettingCacheAllowed)
    {
        _blockSaveToDb();
        _blockCacheReset();
        m_pDataSourceProxy->resetDataEntryCache();
    }
}

void GameFacade::provideCorrectWordsPairToUser()
{
    m_pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::REQUESTED_BY_USER);
    m_pDataSourceProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());
}

void GameFacade::setLevel(Game::Level level)
{
    m_pWordMixer->setWordPieceSize(level);
    m_pScoreItem->setScoreIncrement(level);
    m_pDataSourceProxy->provideDataEntryToConsumer(m_pDataSourceAccessHelper->generateEntryNumber());

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::LEVEL_CHANGED);
}

void GameFacade::resetGameStatistics()
{
    m_pScoreItem->resetStatistics();
    Q_EMIT statusChanged(m_CurrentStatusCode = m_pInputBuilder->isInputComplete() ? Game::StatusCodes::STATISTICS_RESET_COMPLETE_INPUT
                                                                                  : Game::StatusCodes::STATISTICS_RESET_INCOMPLETE_INPUT);
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

int GameFacade::getCachedWordPairs() const
{
    return m_CachedWordPairs;
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

void GameFacade::_onCloseInputPermissionRequested()
{
    m_pInputBuilder->setCloseInputPermission(m_pWordPairOwner->isLastAvailableWordPiece());
}

void GameFacade::_onReadDataFromDbFinished(bool success)
{
    if (success)
    {
        int nrOfEntries{m_pDataSourceProxy->getNrOfValidEntries()};

        if (nrOfEntries != 0)
        {
            m_pDataSourceAccessHelper->setEntriesTable(nrOfEntries);
            _connectDataSourceToWordMixer();

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

void GameFacade::_onInvalidWordsPairAddedByUser()
{
    // restore add to cache capability so the user can re-add the entry after modifying the words
    _allowAddToCache();

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::INVALID_DATA_ENTRY);
}

void GameFacade::_onNewWordsPairAddedToCache()
{
    ++m_CachedWordPairs;

    _allowAddToCache();
    _allowCacheReset();
    _allowSaveToDb();

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_SUCCESS);
}

void GameFacade::_onCacheReset()
{
    m_CachedWordPairs = 0;
    _allowAddToCache();

    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::CACHE_RESET);
}

void GameFacade::_onWriteDataToDbFinished(int nrOfEntries)
{
    Q_ASSERT(m_CachedWordPairs == nrOfEntries);

    int initialNrOfEntries{m_pDataSourceAccessHelper->getTotalNrOfEntries()};

    m_pDataSourceAccessHelper->addEntries(nrOfEntries);

    if (initialNrOfEntries == 0)
    {
        _connectDataSourceToWordMixer();
    }

    _allowAddToCache();

    Q_EMIT statusChanged(m_CurrentStatusCode = initialNrOfEntries == 0 ? Game::StatusCodes::DATA_GOT_AVAILABLE : Game::StatusCodes::DATA_SUCCESSFULLY_SAVED);

    // don't reset this variable prior to changing status (is used by presenter)
    m_CachedWordPairs = 0;
}

void GameFacade::_onWriteDataToDbErrorOccured()
{
    Q_EMIT statusChanged(m_CurrentStatusCode = Game::StatusCodes::DATA_ENTRY_SAVING_ERROR);
}

void GameFacade::_connectDataSourceToWordMixer()
{
    bool connected{connect(m_pDataSourceProxy, &DataSourceProxy::entryProvidedToConsumer, m_pWordMixer, &WordMixer::mixWords)};
    Q_ASSERT(connected);

    m_IsDataAvailable = true;
    Q_EMIT dataAvailableChanged();
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
