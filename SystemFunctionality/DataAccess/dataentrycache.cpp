#include <QFile>
#include <QTextStream>
#include <QThread>

#include "dataentrycache.h"
#include "../Utilities/game.h"
#include "../Utilities/gamestrings.h"

DataEntryCache::DataEntryCache(DataSource* pDataSource, QObject *parent)
    : QObject(parent)
    , m_CacheEntries{}
    , m_pDataSource{pDataSource}
{
    Q_ASSERT(pDataSource);
}

int DataEntryCache::getNrOfCachedEntries() const
{
    return m_CacheEntries.size();
}

void DataEntryCache::onAddEntryToCacheRequested(DataSource::DataEntry dataEntry)
{
    m_CacheEntries.append(dataEntry);
    Q_EMIT newWordsPairAddedToCache();
}

void DataEntryCache::onWriteDataToDbRequested()
{
    QFile wordPairsFile(m_pDataSource->getDataFilePath());

    if(!wordPairsFile.open(QIODevice::Append))
    {
        // user entered data valid but error when writing to DB
        Q_EMIT writeDataToDbErrorOccured();
    }
    else
    {
        QTextStream lineWriter{&wordPairsFile};

        for (auto entry : m_CacheEntries)
        {
            QString rawDataEntry{entry.firstWord + (entry.areSynonyms ? GameStrings::c_SynonymsSeparator : GameStrings::c_AntonymsSeparator) + entry.secondWord};
            lineWriter << rawDataEntry << endl;
        }

        wordPairsFile.close();

        int nrOfSavedEntries{m_CacheEntries.size()};
        m_pDataSource->updateDataEntries(m_CacheEntries, true);
        m_CacheEntries.clear();

        // for sync purposes only
        QThread::msleep(Game::c_WriteDataThreadDelay);

        Q_EMIT writeDataToDbFinished(nrOfSavedEntries);
    }
}
