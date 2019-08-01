#include "dataentrystatistics.h"

DataEntryStatistics::DataEntryStatistics(QObject *parent)
    : QObject(parent)
    , m_CurrentNrOfCacheEntries{0}
    , m_LastNrOfSavedEntries{0}
{
}

int DataEntryStatistics::getCurrentNrOfCacheEntries() const
{
    return m_CurrentNrOfCacheEntries;
}

int DataEntryStatistics::getLastSavedNrOfCacheEntries() const
{
    return m_LastNrOfSavedEntries;
}

void DataEntryStatistics::onNewWordsPairAddedToCache()
{
    ++m_CurrentNrOfCacheEntries;
}

void DataEntryStatistics::onWriteDataToDbFinished(int nrOfEntries)
{
    m_LastNrOfSavedEntries = nrOfEntries;
    m_CurrentNrOfCacheEntries = 0;
}

void DataEntryStatistics::onCacheReset()
{
    m_CurrentNrOfCacheEntries = 0;
}
