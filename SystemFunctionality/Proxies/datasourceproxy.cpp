#include "datasourceproxy.h"
#include "../Management/gamemanager.h"

DataSourceProxy::DataSourceProxy(QObject *parent)
    : QObject(parent)
{
}

void DataSourceProxy::fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult)
{
    GameManager::getManager()->fetchDataForPrimaryLanguage(languageIndex, allowEmptyResult);
}

void DataSourceProxy::provideDataEntryToConsumer(int entryNumber)
{
    GameManager::getManager()->provideDataEntryToConsumer(entryNumber);
}

int DataSourceProxy::getNrOfDataSourceEntries()
{
    return GameManager::getManager()->getNrOfDataSourceEntries();
}
