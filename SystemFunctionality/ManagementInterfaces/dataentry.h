#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <QString>
#include "../Utilities/dataentryutils.h"

class IDataEntry
{
public:
    virtual void fetchDataForSecondaryLanguage(int languageIndex) = 0;
    virtual void saveDataToDb() = 0;
    virtual void requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms, int languageIndex) = 0;
    virtual void requestCacheReset() = 0;

    virtual DataEntry::ValidationCodes getPairEntryValidationCode() const = 0;
    virtual int getLastSavedTotalNrOfEntries() const = 0;
    virtual int getLastNrOfEntriesSavedToPrimaryLanguage() const = 0;
    virtual int getCurrentNrOfCacheEntries() const = 0;
    virtual ~IDataEntry() = 0;
};

#endif // DATAENTRY_H
