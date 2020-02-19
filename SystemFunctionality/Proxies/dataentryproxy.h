#ifndef DATAENTRYPROXY_H
#define DATAENTRYPROXY_H

#include <QObject>

#include "../Utilities/dataentryutils.h"

class GameManager;

class DataEntryProxy : public QObject
{
    Q_OBJECT
public:
    explicit DataEntryProxy(QObject *parent = nullptr);

    void fetchDataForSecondaryLanguage(int languageIndex);
    void saveDataToDb();
    void requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms, int languageIndex);
    void requestCacheReset();

    DataEntry::ValidationCodes getPairEntryValidationCode() const;
    int getLastSavedTotalNrOfEntries() const;
    int getLastNrOfEntriesSavedToPrimaryLanguage() const;
    int getCurrentNrOfCacheEntries() const;

signals:
    Q_SIGNAL void dataEntryAllowed(bool allowed);
    Q_SIGNAL void newWordsPairValidated(bool isValid);
    Q_SIGNAL void newWordsPairAddedToCache();
    Q_SIGNAL void addInvalidWordsPairRequested();
    Q_SIGNAL void wordsPairAlreadyContainedInCache();
    Q_SIGNAL void cacheReset();
    Q_SIGNAL void writeDataToDbFinished();
    Q_SIGNAL void fetchDataForSecondaryLanguageFinished(bool success);
};

#endif // DATAENTRYPROXY_H
