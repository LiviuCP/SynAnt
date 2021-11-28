#include "dataentryproxy.h"
#include "gamemanager.h"

DataEntryProxy::DataEntryProxy(QObject *parent) : QObject(parent)
{
    GameManager* pGameManager{GameManager::getManager()};
    Q_ASSERT(pGameManager);

    auto connected{connect(pGameManager, &GameManager::newWordsPairAddedToCache, this, &DataEntryProxy::newWordsPairAddedToCache, Qt::DirectConnection)};
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::writeDataToDbFinished, this, &DataEntryProxy::writeDataToDbFinished, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::cacheReset, this, &DataEntryProxy::cacheReset, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::addInvalidWordsPairRequested, this, &DataEntryProxy::addInvalidWordsPairRequested, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::wordsPairAlreadyContainedInCache, this, &DataEntryProxy::wordsPairAlreadyContainedInCache, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::fetchDataForDataEntryLanguageFinished, this, &DataEntryProxy::fetchDataForDataEntryLanguageFinished, Qt::DirectConnection);
    Q_ASSERT(connected);
    connected = connect(pGameManager, &GameManager::dataEntryAllowed, this, &DataEntryProxy::dataEntryAllowed, Qt::DirectConnection);
    Q_ASSERT(connected);
}

void DataEntryProxy::fetchDataForSecondaryLanguage(int languageIndex)
{
    GameManager::getManager()->fetchDataForSecondaryLanguage(languageIndex);
}

void DataEntryProxy::requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms, int languageIndex)
{
    GameManager::getManager()->requestWriteToCache(newWordsPair, areSynonyms, languageIndex);
}

void DataEntryProxy::requestCacheReset()
{
    GameManager::getManager()->requestCacheReset();
}

void DataEntryProxy::saveDataToDb()
{
    GameManager::getManager()->saveDataToDb();
}

uint16_t DataEntryProxy::getInvalidPairEntryReasonCode() const
{
    return GameManager::getManager()->getInvalidPairEntryReasonCode();
}

int DataEntryProxy::getCurrentNrOfCachedEntries() const
{
    return GameManager::getManager()->getCurrentNrOfCachedEntries();
}

int DataEntryProxy::getLastSavedTotalNrOfEntries() const
{
    return GameManager::getManager()->getLastSavedTotalNrOfEntries();
}

int DataEntryProxy::getLastNrOfEntriesSavedToPrimaryLanguage() const
{
    return GameManager::getManager()->getLastNrOfEntriesSavedToPrimaryLanguage();
}
