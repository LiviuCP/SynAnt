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
    explicit DataEntryCache(DataSource* pDataSource, QObject *parent = nullptr);
    int getNrOfCachedEntries() const;

public slots:
    void onAddEntryToCacheRequested(DataSource::DataEntry dataEntry);
    void onResetCacheRequested();
    void onWriteDataToDbRequested();

signals:
    Q_SIGNAL void newWordsPairAddedToCache();
    Q_SIGNAL void cacheReset();
    Q_SIGNAL void writeDataToDbFinished(int nrOfEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();

private:
    QVector<DataSource::DataEntry> m_CacheEntries;
    DataSource* m_pDataSource;
};

#endif // DATAENTRYCACHE_H
