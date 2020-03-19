/*
   This class fulfills following tasks:
   1) Provides temporary storage to the pairs added to game through data entry page and validated by data entry validator
   2) Saves the stored pairs to database and appends them to datasource per user request
*/

#ifndef DATAENTRYCACHE_H
#define DATAENTRYCACHE_H

#include <QObject>
#include <QVector>

#include "datasource.h"

class DataEntryCache : public QObject
{
    Q_OBJECT
public:
    explicit DataEntryCache(DataSource* pDataSource, QString databasePath, QObject *parent = nullptr);

    // for testing purposes only
    int getNrOfCachedEntries() const;

public slots:
    void onValidEntryReceived(DataSource::DataEntry dataEntry, int languageIndex);
    void onResetCacheRequested();
    void onWriteDataToDbRequested();

signals:
    Q_SIGNAL void newWordsPairAddedToCache();
    Q_SIGNAL void wordsPairAlreadyContainedInCache();
    Q_SIGNAL void cacheReset();
    Q_SIGNAL void writeDataToDbFinished(int nrOfPrimaryLanguageSavedEntries, int totalNrOfSavedEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();

private:
    void _moveCachedEntriesToDataSource(int& nrOfEntriesSavedToPrimaryLanguage);

    QVector<DataSource::DataEntry> m_CacheEntries;
    QVector<int> m_LanguageIndexes;
    DataSource* m_pDataSource;
    QString m_DatabasePath;
};

#endif // DATAENTRYCACHE_H
