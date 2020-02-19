#include "dataentryproxy.h"
#include "../Management/gamemanager.h"

DataEntryProxy::DataEntryProxy(QObject *parent) : QObject(parent)
{
}

void DataEntryProxy::fetchDataForSecondaryLanguage(int languageIndex)
{
    GameManager::getManager()->fetchDataForSecondaryLanguage(languageIndex);
}

void DataEntryProxy::saveDataToDb()
{
    GameManager::getManager()->saveDataToDb();
}

void DataEntryProxy::requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms, int languageIndex)
{
    GameManager::getManager()->requestWriteToCache(newWordsPair, areSynonyms, languageIndex);
}

void DataEntryProxy::requestCacheReset()
{
    GameManager::getManager()->requestCacheReset();
}

DataEntry::ValidationCodes DataEntryProxy::getPairEntryValidationCode() const
{
    return GameManager::getManager()->getPairEntryValidationCode();
}

int DataEntryProxy::getLastSavedTotalNrOfEntries() const
{
    return GameManager::getManager()->getLastSavedTotalNrOfEntries();
}

int DataEntryProxy::getLastNrOfEntriesSavedToPrimaryLanguage() const
{
    return GameManager::getManager()->getLastNrOfEntriesSavedToPrimaryLanguage();
}

int DataEntryProxy::getCurrentNrOfCacheEntries() const
{
    return GameManager::getManager()->getCurrentNrOfCacheEntries();
}
