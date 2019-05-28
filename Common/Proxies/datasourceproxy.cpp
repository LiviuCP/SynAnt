#include "datasourceproxy.h"
#include "../DataAccess/datasource.h"

DataSourceProxy::DataSourceProxy(DataSource* pDataSource, QObject *parent)
    : QObject(parent)
    , m_pDataSource{pDataSource}
{
}

void DataSourceProxy::fetchDataEntry(int entryNumber)
{
    m_pDataSource->fetchDataEntry(entryNumber);
}

int DataSourceProxy::getNrOfEntries()
{
    return m_pDataSource->getNrOfEntries();
}
