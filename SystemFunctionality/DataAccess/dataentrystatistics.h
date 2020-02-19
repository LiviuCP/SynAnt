#ifndef DATAENTRYSTATISTICS_H
#define DATAENTRYSTATISTICS_H

#include <QObject>

class DataEntryStatistics : public QObject
{
    Q_OBJECT
public:
    explicit DataEntryStatistics(QObject *parent = nullptr);

    int getCurrentNrOfCacheEntries() const;
    int getLastNrOfEntriesSavedToPrimaryLanguage() const;
    int getLastSavedTotalNrOfEntries() const;

public slots:
    void onNewWordsPairAddedToCache();
    void onWriteDataToDbFinished(int nrOfPrimaryLanguageSavedEntries, int totalNrOfSavedEntries);
    void onCacheReset();

private:
    int m_CurrentNrOfCacheEntries;
    int m_TotalNrOfSavedEntries; // last saved number of entries in all languages
    int m_NrOfPrimaryLanguageSavedEntries; // last saved number of entries in the primary language that was setup at the time of the save operation
};

#endif // DATAENTRYSTATISTICS_H
