#include <QMap>

#include "dataentryfacade.h"
#include "../ManagementProxies/dataentryproxy.h"

static constexpr int c_NrOfInvalidPairEntryStatusCodes{static_cast<uint16_t>(DataEntryFacade::StatusCodes::Add_Failed_Status_Codes_End) -
                                              static_cast<uint16_t>(DataEntryFacade::StatusCodes::Add_Failed_Status_Codes_Start) - 1};
static constexpr uint16_t c_InvalidPairBaseReasonCode {uint16_t{0x0001} << (c_NrOfInvalidPairEntryStatusCodes-1)};

DataEntryFacade::DataEntryFacade(QObject *parent)
    : QObject(parent)
    , m_pDataEntryProxy{new DataEntryProxy{this}}
    , m_CurrentStatusCode{DataEntryFacade::StatusCodes::NO_DATA_ENTRY_REQUESTED}
    , m_IsDataEntryAllowed{false}
    , m_IsAddingToCacheAllowed{true}
    , m_IsResettingCacheAllowed{false}
    , m_IsSavingToDbAllowed{false}
    , m_IsFetchingInProgress{false}
    , m_IsSavingInProgress{false}
    , m_CurrentLanguageIndex{-1}
    , m_IsSavingDeferred{false}
{
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
    connected = connect(m_pDataEntryProxy, &DataEntryProxy::fetchDataForDataEntryLanguageFinished, this, &DataEntryFacade::_onFetchDataForDataEntryLanguageFinished);
    Q_ASSERT(connected);
}

void DataEntryFacade::startDataEntry()
{
    m_CurrentStatusCode = m_IsSavingInProgress ? DataEntryFacade::StatusCodes::DATA_ENTRY_STARTED_SAVE_IN_PROGRESS : DataEntryFacade::StatusCodes::DATA_ENTRY_STARTED;
    Q_EMIT statusChanged();
}

void DataEntryFacade::resumeDataEntry()
{
    if (!m_IsSavingInProgress)
    {
        m_CurrentStatusCode = DataEntryFacade::StatusCodes::DATA_ENTRY_RESUMED;
        Q_EMIT statusChanged();
    }
}

void DataEntryFacade::stopDataEntry()
{
    m_CurrentStatusCode = m_IsSavingInProgress ? DataEntryFacade::StatusCodes::DATA_ENTRY_STOPPED_SAVE_IN_PROGRESS
                                                                        : isDataFetchingInProgress() ? DataEntryFacade::StatusCodes::DATA_ENTRY_DISABLED :
                                                                                                       DataEntryFacade::StatusCodes::DATA_ENTRY_STOPPED;
    Q_EMIT statusChanged();
}

void DataEntryFacade::requestAddPairToCache(const QString &firstWord, const QString &secondWord, bool areSynonyms)
{
    if (m_IsDataEntryAllowed && m_IsAddingToCacheAllowed)
    {
        _blockAddToCache();
        m_pDataEntryProxy->requestWriteToCache(QPair<QString, QString>{firstWord, secondWord}, areSynonyms, m_CurrentLanguageIndex);
    }
}

void DataEntryFacade::requestSaveDataToDb()
{
    if (m_IsDataEntryAllowed && m_IsSavingToDbAllowed)
    {
        _blockAddToCache();
        _blockSaveToDb();
        _blockCacheReset();

        if (!isDataFetchingInProgress())
        {
            m_IsSavingInProgress = true;
            m_CurrentStatusCode = DataEntryFacade::StatusCodes::DATA_SAVE_IN_PROGRESS;
            Q_EMIT statusChanged();

            m_pDataEntryProxy->saveDataToDb();
        }
        else
        {
            m_IsSavingDeferred = true;
        }
    }
}

void DataEntryFacade::requestCacheReset()
{
    if (m_IsDataEntryAllowed && m_IsResettingCacheAllowed)
    {
        m_CurrentStatusCode = DataEntryFacade::StatusCodes::RESET_CACHE_REQUESTED;
        Q_EMIT statusChanged();

        _blockSaveToDb();
        _blockCacheReset();
        m_pDataEntryProxy->requestCacheReset();
    }
}

int DataEntryFacade::getCurrentNrOfAddedPairs() const
{
    return m_pDataEntryProxy->getCurrentNrOfCachedEntries();
}

int DataEntryFacade::getLastSavedTotalNrOfPairs() const
{
    return m_pDataEntryProxy->getLastSavedTotalNrOfEntries();
}

int DataEntryFacade::getLastNrOfPairsSavedToPrimaryLanguage() const
{
    return m_pDataEntryProxy->getLastNrOfEntriesSavedToPrimaryLanguage();
}

int DataEntryFacade::getCurrentLanguageIndex() const
{
    return m_CurrentLanguageIndex;
}

DataEntryFacade::StatusCodes DataEntryFacade::getStatusCode() const
{
    return m_CurrentStatusCode;
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

bool DataEntryFacade::isDataFetchingInProgress() const
{
    return m_IsFetchingInProgress;
}

bool DataEntryFacade::isDataSavingInProgress() const
{
    return m_IsSavingInProgress;
}

void DataEntryFacade::setLanguage(int languageIndex)
{
    // for the moment no status code will be issued for language change in the data entry dialog
    if (m_CurrentLanguageIndex != languageIndex)
    {
        m_CurrentLanguageIndex = languageIndex;
        m_IsFetchingInProgress = true;

        Q_EMIT languageChanged();
        Q_EMIT fetchingInProgressChanged();
        m_CurrentStatusCode = DataEntryFacade::StatusCodes::FETCHING_DATA;
        Q_EMIT statusChanged();

        m_pDataEntryProxy->fetchDataForSecondaryLanguage(languageIndex);
    }
}

void DataEntryFacade::_onFetchDataForDataEntryLanguageFinished(bool success)
{
    if (success)
    {
        m_IsFetchingInProgress = false;
        Q_EMIT fetchingInProgressChanged();

        if (m_IsSavingDeferred)
        {
            m_IsSavingDeferred = false;
            m_IsSavingInProgress = true;
            m_CurrentStatusCode = DataEntryFacade::StatusCodes::DATA_FETCHING_FINISHED_SAVE_IN_PROGRESS;
            Q_EMIT statusChanged();
            m_pDataEntryProxy->saveDataToDb();
        }
        else
        {
            m_CurrentStatusCode = DataEntryFacade::StatusCodes::DATA_FETCHING_FINISHED;
            Q_EMIT statusChanged();
        }
    }
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

    m_CurrentStatusCode = DataEntryFacade::StatusCodes::DATA_ENTRY_ADD_SUCCESS;
    Q_EMIT statusChanged();
}

void DataEntryFacade::_onAddInvalidWordsPairRequested()
{
    // restore add to cache capability so the user can re-add the entry after modifying the words
    _allowAddToCache();

    m_CurrentStatusCode = _retrieveInvalidWordsPairStatusCode();
    Q_EMIT statusChanged();
}

void DataEntryFacade::_onWordsPairAlreadyContainedInCache()
{
    _allowAddToCache();
    _allowCacheReset();
    _allowSaveToDb();

    m_CurrentStatusCode = DataEntryFacade::StatusCodes::PAIR_ALREADY_ADDED_SAVE_OR_DISCARD;
    Q_EMIT statusChanged();
}

void DataEntryFacade::_onCacheReset()
{
    _allowAddToCache();
    m_CurrentStatusCode = DataEntryFacade::StatusCodes::CACHE_RESET;
    Q_EMIT statusChanged();
}

void DataEntryFacade::_onWriteDataToDbFinished()
{
    _allowAddToCache();
    m_IsSavingInProgress = false;

    m_CurrentStatusCode = DataEntryFacade::StatusCodes::DATA_SUCCESSFULLY_SAVED;
    Q_EMIT statusChanged();
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

DataEntryFacade::StatusCodes DataEntryFacade::_retrieveInvalidWordsPairStatusCode()
{
    bool success{false};
    const uint16_t c_RetrievedReasonCode{m_pDataEntryProxy->getInvalidPairEntryReasonCode()};
    uint16_t localReasonCode{c_InvalidPairBaseReasonCode};
    int offset{0};

    // decode the retrieved reason code to get the offset to be applied for determining the exact status code to be emitted to presenter
    while (localReasonCode != uint16_t{0})
    {
        if (localReasonCode == c_RetrievedReasonCode)
        {
            success = true;
            break;
        }
        localReasonCode = localReasonCode >> 1;
        ++offset;
    }

    // ensure the retrieved reason code is valid (e.g. only one bit should be set)
    Q_ASSERT(success);

    return static_cast<DataEntryFacade::StatusCodes>(static_cast<int>(DataEntryFacade::StatusCodes::Add_Failed_Status_Codes_Start) + offset + 1);
}
