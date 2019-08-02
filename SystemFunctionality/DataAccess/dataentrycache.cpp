#include <QThread>
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlQuery>

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

void DataEntryCache::onValidEntryReceived(DataSource::DataEntry dataEntry)
{
    if (!m_CacheEntries.contains(dataEntry))
    {
        m_CacheEntries.append(dataEntry);
        Q_EMIT newWordsPairAddedToCache();
    }
    else
    {
        Q_EMIT wordsPairAlreadyContainedInCache();
    }

}

void DataEntryCache::onResetCacheRequested()
{
    m_CacheEntries.clear();

    // for sync purposes only
    QThread::msleep(Game::c_ResetCacheDelay);

    Q_EMIT cacheReset();
}

void DataEntryCache::onWriteDataToDbRequested()
{
    Q_UNUSED(QSqlDatabase::addDatabase(GameStrings::c_DbDriverName));

    // ensure all database related objects are destroyed before the connection is removed
    {
        QSqlDatabase db{QSqlDatabase::database(QSqlDatabase::defaultConnection)};

        db.setDatabaseName(m_pDataSource->getDataFilePath());

        if (db.open())
        {
            QSqlQuery query;

            for (auto entry : m_CacheEntries)
            {
                query.prepare(GameStrings::c_InsertEntryIntoDbQuery);

                query.bindValue(GameStrings::c_FirstWordFieldPlaceholder, entry.firstWord);
                query.bindValue(GameStrings::c_SecondWordFieldPlaceholder, entry.secondWord);
                query.bindValue(GameStrings::c_AreSynonymsFieldPlaceholder, static_cast<int>(entry.areSynonyms));

                if (!query.exec())
                {
                    Q_EMIT writeDataToDbErrorOccured();
                }
            }

            db.close();

            int nrOfSavedEntries{m_CacheEntries.size()};
            m_pDataSource->updateDataEntries(m_CacheEntries, true);
            m_CacheEntries.clear();

            // for sync purposes only
            QThread::msleep(Game::c_WriteDataThreadDelay);

            Q_EMIT writeDataToDbFinished(nrOfSavedEntries);
        }
        else
        {
            // user entered data valid but error when writing to DB
            Q_EMIT writeDataToDbErrorOccured();
        }
    }

    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
}
