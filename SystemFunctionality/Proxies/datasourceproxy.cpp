#include "datasourceproxy.h"
#include "../DataAccess/datasource.h"
#include "../Management/gamemanager.h"

DataSourceProxy::DataSourceProxy(DataSource* pDataSource, QObject *parent)
    : QObject(parent)
    , m_pDataSource{pDataSource}
{
}

void DataSourceProxy::loadData()
{
    GameManager::getManager()->loadData();
}

void DataSourceProxy::requestAddPairToDataSource(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    GameManager::getManager()->requestAddPairToDataSource(newWordsPair, areSynonyms);
}

void DataSourceProxy::fetchDataEntry(int entryNumber)
{
    m_pDataSource->fetchDataEntry(entryNumber);
}

int DataSourceProxy::getNrOfEntries()
{
    return m_pDataSource->getNrOfEntries();
}
