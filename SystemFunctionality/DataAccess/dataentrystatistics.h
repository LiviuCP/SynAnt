#ifndef DATAENTRYSTATISTICS_H
#define DATAENTRYSTATISTICS_H

#include <QObject>

class DataEntryStatistics : public QObject
{
    Q_OBJECT
public:
    explicit DataEntryStatistics(QObject *parent = nullptr);

    int getCurrentNrOfCacheEntries() const;
    int getLastSavedNrOfCacheEntries() const;

public slots:
    void onNewWordsPairAddedToCache();
    void onWriteDataToDbFinished(int nrOfEntries);
    void onCacheReset();

private:
    int m_CurrentNrOfCacheEntries;
    int m_LastNrOfSavedEntries;
};

#endif // DATAENTRYSTATISTICS_H
