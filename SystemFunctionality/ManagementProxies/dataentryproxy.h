#ifndef DATAENTRYPROXY_H
#define DATAENTRYPROXY_H

#include <QObject>

#include "../ManagementInterfaces/dataentryinterface.h"
#include "../Utilities/dataentryutils.h"

class GameManager;

class DataEntryProxy : public QObject, public IDataEntry
{
    Q_OBJECT
    Q_INTERFACES(IDataEntry)
public:
    explicit DataEntryProxy(QObject *parent = nullptr);

    void fetchDataForSecondaryLanguage(int languageIndex);
    void requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms, int languageIndex);
    void requestCacheReset();
    void saveDataToDb();

    QString getPairEntryValidationCode() const;
    int getCurrentNrOfCachedEntries() const;
    int getLastSavedTotalNrOfEntries() const;
    int getLastNrOfEntriesSavedToPrimaryLanguage() const;

signals:
    Q_SIGNAL void fetchDataForDataEntryLanguageFinished(bool success);
    Q_SIGNAL void dataEntryAllowed(bool allowed);
    Q_SIGNAL void newWordsPairAddedToCache();
    Q_SIGNAL void addInvalidWordsPairRequested();
    Q_SIGNAL void wordsPairAlreadyContainedInCache();
    Q_SIGNAL void cacheReset();
    Q_SIGNAL void writeDataToDbFinished();
};

#endif // DATAENTRYPROXY_H
