#include "dataentrystatistics.h"

DataEntryStatistics::DataEntryStatistics(QObject *parent)
    : QObject(parent)
    , m_CurrentNrOfCacheEntries{0}
    , m_TotalNrOfSavedEntries{0}
    , m_NrOfPrimaryLanguageSavedEntries{0}
{
}

int DataEntryStatistics::getCurrentNrOfCacheEntries() const
{
    return m_CurrentNrOfCacheEntries;
}

int DataEntryStatistics::getLastNrOfEntriesSavedToPrimaryLanguage() const
{
    return m_NrOfPrimaryLanguageSavedEntries;
}

int DataEntryStatistics::getLastSavedTotalNrOfEntries() const
{
    return m_TotalNrOfSavedEntries;
}

void DataEntryStatistics::onNewWordsPairAddedToCache()
{
    ++m_CurrentNrOfCacheEntries;
}

void DataEntryStatistics::onWriteDataToDbFinished(int nrOfPrimaryLanguageSavedEntries, int totalNrOfSavedEntries)
{
    m_TotalNrOfSavedEntries = totalNrOfSavedEntries;
    m_NrOfPrimaryLanguageSavedEntries = nrOfPrimaryLanguageSavedEntries;
    m_CurrentNrOfCacheEntries = 0;
}

void DataEntryStatistics::onCacheReset()
{
    m_CurrentNrOfCacheEntries = 0; // only reset the cached entries number, saved entries numbers should persist until the next save operation
}
