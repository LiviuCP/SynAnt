#include "dataentryfacade.h"
#include "../Proxies/datafunctionalityproxy.h"
#include "../Proxies/dataentryproxy.h"

DataEntryFacade::DataEntryFacade(QObject *parent)
    : QObject(parent)
    , m_pDataFunctionalityProxy{new DataFunctionalityProxy{this}}
    , m_CurrentStatusCode{DataEntry::StatusCodes::NO_DATA_ENTRY_REQUESTED}
    , m_IsDataEntryAllowed{false}
    , m_IsAddingToCacheAllowed{true}
    , m_IsResettingCacheAllowed{false}
    , m_IsSavingToDbAllowed{false}
    , m_IsSavingInProgress{false}
{
    m_pDataEntryProxy = m_pDataFunctionalityProxy->getDataEntryProxy();

    // all QObjects used by application (except the QML registered ones) should be parented (the non-parented ones would only be used in tests)
    Q_ASSERT(this->parent());
    Q_ASSERT(m_pDataEntryProxy->parent());

    bool connected{connect(m_pDataEntryProxy, &DataEntryProxy::dataEntryAllowed, this, &DataEntryFacade::_onDataEntryAllowed)};
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryProxy, &DataEntryProxy::newWordsPairAddedToCache, this, &DataEntryFacade::_onNewWordsPairAddedToCache);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryProxy, &DataEntryProxy::addInvalidWordsPairRequested, this, &DataEntryFacade::_onAddInvalidWordsPairRequested);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryProxy, &DataEntryProxy::wordsPairAlreadyContainedInCache, this, &DataEntryFacade::_onWordsPairAlreadyContainedInCache);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryProxy, &DataEntryProxy::cacheReset, this, &DataEntryFacade::_onCacheReset);
    Q_ASSERT(connected);
    connected = connect(m_pDataEntryProxy, &DataEntryProxy::writeDataToDbFinished, this, &DataEntryFacade::_onWriteDataToDbFinished);
    Q_ASSERT(connected);
}

void DataEntryFacade::startDataEntry()
{
    if (!m_IsSavingInProgress)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = DataEntry::StatusCodes::DATA_ENTRY_STARTED);
    }
}

void DataEntryFacade::resumeDataEntry()
{
    if (!m_IsSavingInProgress)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = DataEntry::StatusCodes::DATA_ENTRY_RESUMED);
    }
}

void DataEntryFacade::requestAddPairToCache(const QString &firstWord, const QString &secondWord, bool areSynonyms)
{
    if (m_IsDataEntryAllowed && m_IsAddingToCacheAllowed)
    {
        _blockAddToCache();
        m_pDataEntryProxy->requestWriteToCache(QPair<QString, QString>{firstWord, secondWord}, areSynonyms);
    }
}

void DataEntryFacade::requestSaveDataToDb()
{
    if (m_IsDataEntryAllowed && m_IsSavingToDbAllowed)
    {
        _blockAddToCache();
        _blockSaveToDb();
        _blockCacheReset();

        m_IsSavingInProgress = true;
        Q_EMIT statusChanged(m_CurrentStatusCode = DataEntry::StatusCodes::DATA_SAVE_IN_PROGRESS);

        m_pDataEntryProxy->saveDataToDb();
    }
}

void DataEntryFacade::requestCacheReset()
{
    if (m_IsDataEntryAllowed && m_IsResettingCacheAllowed)
    {
        Q_EMIT statusChanged(m_CurrentStatusCode = DataEntry::StatusCodes::RESET_CACHE_REQUESTED);

        _blockSaveToDb();
        _blockCacheReset();
        m_pDataEntryProxy->requestCacheReset();
    }
}

int DataEntryFacade::getCurrentNrOfAddedWordPairs() const
{
    return m_pDataEntryProxy->getCurrentNrOfCacheEntries();
}

int DataEntryFacade::getLastSavedNrOfWordPairs() const
{
    return m_pDataEntryProxy->getLastSavedNrOfCacheEntries();
}

DataEntry::ValidationCodes DataEntryFacade::getDataEntryValidationCode() const
{
    return m_pDataEntryProxy->getPairEntryValidationCode();
}

bool DataEntryFacade::isDataEntryAllowed() const
{
    return m_IsDataEntryAllowed;
}

bool DataEntryFacade::isAddingToCacheAllowed() const
{
    return m_IsAddingToCacheAllowed;
}

bool DataEntryFacade::isCacheResetAllowed() const
{
    return m_IsResettingCacheAllowed;
}

bool DataEntryFacade::isSavingToDbAllowed() const
{
    return m_IsSavingToDbAllowed;
}

void DataEntryFacade::_onDataEntryAllowed(bool allowed)
{
    if (m_IsDataEntryAllowed != allowed)
    {
        m_IsDataEntryAllowed = allowed;
        Q_EMIT dataEntryAllowedChanged();
    }
}

void DataEntryFacade::_onNewWordsPairAddedToCache()
{
    _allowAddToCache();
    _allowCacheReset();
    _allowSaveToDb();

    Q_EMIT statusChanged(m_CurrentStatusCode = DataEntry::StatusCodes::DATA_ENTRY_ADD_SUCCESS);
}

void DataEntryFacade::_onAddInvalidWordsPairRequested()
{
    // restore add to cache capability so the user can re-add the entry after modifying the words
    _allowAddToCache();

    Q_EMIT statusChanged(m_CurrentStatusCode = DataEntry::StatusCodes::DATA_ENTRY_ADD_INVALID);
}

void DataEntryFacade::_onWordsPairAlreadyContainedInCache()
{
    _allowAddToCache();
    _allowCacheReset();
    _allowSaveToDb();

    Q_EMIT statusChanged(m_CurrentStatusCode = DataEntry::StatusCodes::PAIR_ALREADY_ADDED);
}

void DataEntryFacade::_onCacheReset()
{
    _allowAddToCache();
    Q_EMIT statusChanged(m_CurrentStatusCode = DataEntry::StatusCodes::CACHE_RESET);
}

void DataEntryFacade::_onWriteDataToDbFinished(int nrOfEntries)
{
    // only used in GameFacade
    Q_UNUSED(nrOfEntries);

    _allowAddToCache();
    m_IsSavingInProgress = false;

    Q_EMIT statusChanged(m_CurrentStatusCode = DataEntry::StatusCodes::DATA_SUCCESSFULLY_SAVED);
}

void DataEntryFacade::_allowAddToCache()
{
    if (!m_IsAddingToCacheAllowed)
    {
        m_IsAddingToCacheAllowed = true;
        Q_EMIT addPairToCacheAllowedChanged();
    }
}

void DataEntryFacade::_blockAddToCache()
{
    if (m_IsAddingToCacheAllowed)
    {
        m_IsAddingToCacheAllowed = false;
        Q_EMIT addPairToCacheAllowedChanged();
    }
}

void DataEntryFacade::_allowCacheReset()
{
    if (!m_IsResettingCacheAllowed)
    {
        m_IsResettingCacheAllowed = true;
        Q_EMIT resetCacheAllowedChanged();
    }
}

void DataEntryFacade::_blockCacheReset()
{
    if (m_IsResettingCacheAllowed)
    {
        m_IsResettingCacheAllowed = false;
        Q_EMIT resetCacheAllowedChanged();
    }
}

void DataEntryFacade::_allowSaveToDb()
{
    if (!m_IsSavingToDbAllowed)
    {
        m_IsSavingToDbAllowed = true;
        Q_EMIT saveNewPairsToDbAllowedChanged();
    }
}

void DataEntryFacade::_blockSaveToDb()
{
    if (m_IsSavingToDbAllowed)
    {
        m_IsSavingToDbAllowed = false;
        Q_EMIT saveNewPairsToDbAllowedChanged();
    }
}
