#include "datasourceaccess.h"
#include "datasource.h"

DataSourceAccess::DataSourceAccess(QObject *parent)
    : QObject(parent)
    , m_pDataSource{nullptr}
{
    std::random_device rDev1{};
    m_RowNumberEngine.seed(rDev1());
}

void DataSourceAccess::connectToDataSource(DataSource *pDataSource)
{
    Q_ASSERT(pDataSource);
    m_pDataSource = pDataSource;
    bool connected = connect(m_pDataSource, &DataSource::dataReady, this, &DataSourceAccess::onDataSourceReady);
    Q_ASSERT(connected);
}

void DataSourceAccess::requestNewDataSourceEntry()
{
    int dataEntryNumber(_generateEntryNumber());

    m_pDataSource->fetchDataEntry(dataEntryNumber);
}

void DataSourceAccess::onDataSourceReady()
{
    m_AvailableEntries = m_pDataSource->getNrOfEntries();
}

int DataSourceAccess::_generateEntryNumber()
{
    std::uniform_int_distribution<int> rowNumberDist{0, m_AvailableEntries - 1};
    return rowNumberDist(m_RowNumberEngine);
}
