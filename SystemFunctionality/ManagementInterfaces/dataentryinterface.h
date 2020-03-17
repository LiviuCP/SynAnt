#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <QObject>
#include <QString>
#include "../Utilities/dataentryutils.h"

class IDataEntry
{
public:
    virtual void fetchDataForSecondaryLanguage(int languageIndex) = 0;
    virtual void requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms, int languageIndex) = 0;
    virtual void requestCacheReset() = 0;
    virtual void saveDataToDb() = 0;

    virtual QString getPairEntryValidationCode() const = 0;
    virtual int getCurrentNrOfCachedEntries() const = 0;
    virtual int getLastSavedTotalNrOfEntries() const = 0;
    virtual int getLastNrOfEntriesSavedToPrimaryLanguage() const = 0;

    virtual ~IDataEntry() = 0;

signals:
    Q_SIGNAL virtual void fetchDataForDataEntryLanguageFinished(bool success) = 0;
    Q_SIGNAL virtual void dataEntryAllowed(bool allowed) = 0;
    Q_SIGNAL virtual void newWordsPairAddedToCache() = 0;
    Q_SIGNAL virtual void addInvalidWordsPairRequested() = 0;
    Q_SIGNAL virtual void wordsPairAlreadyContainedInCache() = 0;
    Q_SIGNAL virtual void cacheReset() = 0;
    Q_SIGNAL virtual void writeDataToDbFinished() = 0;
};

Q_DECLARE_INTERFACE(IDataEntry, "IDataEntry");

#endif // DATAENTRY_H
