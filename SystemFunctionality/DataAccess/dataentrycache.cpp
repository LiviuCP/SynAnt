#include <QThread>
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlQuery>

#include "dataentrycache.h"
#include "../Utilities/gameutils.h"

const QVector<QString> c_LanguageCodes{"EN", "DE", "RO", "IT"};

DataEntryCache::DataEntryCache(DataSource* pDataSource, QObject *parent)
    : QObject(parent)
    , m_CacheEntries{}
    , m_LanguageIndexes{}
    , m_pDataSource{pDataSource}
{
    Q_ASSERT(pDataSource);
}

void DataEntryCache::onValidEntryReceived(DataSource::DataEntry dataEntry, int languageIndex)
{
    Q_ASSERT(m_CacheEntries.size() == m_LanguageIndexes.size());

    if (!m_CacheEntries.contains(dataEntry))
    {
        m_CacheEntries.append(dataEntry);
        m_LanguageIndexes.append(languageIndex);
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
    m_LanguageIndexes.clear();

    // for sync purposes only
    QThread::msleep(Game::Timing::c_ResetCacheDelay);

    Q_EMIT cacheReset();
}

void DataEntryCache::onWriteDataToDbRequested()
{
    Q_ASSERT(m_CacheEntries.size() == m_LanguageIndexes.size());

    if (m_CacheEntries.size() > 0)
    {
        Q_UNUSED(QSqlDatabase::addDatabase(Game::Database::c_DbDriverName));

        // ensure all database related objects are destroyed before the connection is removed
        {
            QSqlDatabase db{QSqlDatabase::database(QSqlDatabase::defaultConnection)};

            db.setDatabaseName(m_pDataSource->getDataFilePath());

            if (db.open())
            {
                QSqlQuery query;

                for (int entry{0}; entry < m_CacheEntries.size(); ++entry)
                {
                    query.prepare(Game::Database::c_InsertEntryForLanguageIntoDbQuery);

                    query.bindValue(Game::Database::c_FirstWordFieldPlaceholder, m_CacheEntries[entry].firstWord);
                    query.bindValue(Game::Database::c_SecondWordFieldPlaceholder, m_CacheEntries[entry].secondWord);
                    query.bindValue(Game::Database::c_AreSynonymsFieldPlaceholder, static_cast<int>(m_CacheEntries[entry].areSynonyms));
                    query.bindValue(Game::Database::c_LanguageFieldPlaceholder, c_LanguageCodes[m_LanguageIndexes[entry]]);

                    if (!query.exec())
                    {
                        Q_EMIT writeDataToDbErrorOccured();
                    }
                }

                db.close();

                int totalNrOfSavedEntries{m_CacheEntries.size()};
                int nrOfPrimaryLanguageSavedEntries{0};

                _moveCachedEntriesToDataSource(nrOfPrimaryLanguageSavedEntries);

                // for sync purposes only
                QThread::msleep(Game::Timing::c_WriteDataThreadDelay);

                Q_EMIT writeDataToDbFinished(nrOfPrimaryLanguageSavedEntries, totalNrOfSavedEntries);
            }
            else
            {
                // user entered data valid but error when writing to DB
                Q_EMIT writeDataToDbErrorOccured();
            }
        }

        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }
}

void DataEntryCache::_moveCachedEntriesToDataSource(int& nrOfEntriesSavedToPrimaryLanguage)
{
    Q_ASSERT(m_CacheEntries.size() == m_LanguageIndexes.size());

    if (m_CacheEntries.size() != 0)
    {
        QVector<DataSource::DataEntry> primaryLanguageEntriesBuffer;
        QVector<DataSource::DataEntry> secondaryLanguageEntriesBuffer;

        for (int entry{0}; entry < m_LanguageIndexes.size(); ++entry)
        {
            if (m_LanguageIndexes[entry] == m_pDataSource->getPrimarySourceLanguageIndex())
            {
                primaryLanguageEntriesBuffer.append(m_CacheEntries[entry]);
            }
            else if (m_LanguageIndexes[entry] == m_pDataSource->getSecondarySourceLanguageIndex())
            {
                secondaryLanguageEntriesBuffer.append(m_CacheEntries[entry]);
            }
        }

        if (primaryLanguageEntriesBuffer.size() != 0)
        {
            m_pDataSource->updateDataEntries(primaryLanguageEntriesBuffer, m_pDataSource->getPrimarySourceLanguageIndex(), DataSource::UpdateOperation::APPEND);
        }

        if (secondaryLanguageEntriesBuffer.size() != 0)
        {
            m_pDataSource->updateDataEntries(secondaryLanguageEntriesBuffer, m_pDataSource->getSecondarySourceLanguageIndex(), DataSource::UpdateOperation::APPEND);
        }

        nrOfEntriesSavedToPrimaryLanguage = primaryLanguageEntriesBuffer.size();

        m_CacheEntries.clear();
        m_LanguageIndexes.clear();
    }
}
