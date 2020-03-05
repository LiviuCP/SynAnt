#include "dataaccessproxy.h"
#include "../Management/gamemanager.h"

DataAccessProxy::DataAccessProxy(QObject *parent)
    : QObject(parent)
{
}

void DataAccessProxy::fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult)
{
    GameManager::getManager()->fetchDataForPrimaryLanguage(languageIndex, allowEmptyResult);
}

void DataAccessProxy::provideDataEntryToConsumer(int entryNumber)
{
    GameManager::getManager()->provideDataEntryToConsumer(entryNumber);
}

int DataAccessProxy::getNrOfDataSourceEntries() const
{
    return GameManager::getManager()->getNrOfDataSourceEntries();
}
