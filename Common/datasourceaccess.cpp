#include "datasourceaccess.h"
#include "datasource.h"

DataSourceAccess::DataSourceAccess(QObject *parent)
    : QObject(parent)
    , m_pDataSource{nullptr}
{
    std::random_device randomDevice{};
    m_ChooseEntryNumberEngine.seed(randomDevice());
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
    m_EntryUsedStatuses[dataEntryNumber] = true;
}

void DataSourceAccess::onDataSourceReady()
{
    m_EntryUsedStatuses.fill(false, m_pDataSource->getNrOfEntries());
}

int DataSourceAccess::_generateEntryNumber()
{
    int alreadyAccessedEntriesCount{m_EntryUsedStatuses.count(true)};

    if (alreadyAccessedEntriesCount == m_EntryUsedStatuses.size())
    {
        m_EntryUsedStatuses.fill(false);
        alreadyAccessedEntriesCount = 0;
    }

    std::uniform_int_distribution<int> chooseEntryNumberDist{0, m_EntryUsedStatuses.size() - alreadyAccessedEntriesCount-1};

    int chosenAvailableEntryRelativeNr{chooseEntryNumberDist(m_ChooseEntryNumberEngine)};
    int chosenAvailableEntryAbsoluteNr{-1};

    for (int iterationNr{0}; iterationNr <= chosenAvailableEntryRelativeNr; ++iterationNr)
    {
        ++chosenAvailableEntryAbsoluteNr;

        while(m_EntryUsedStatuses[chosenAvailableEntryAbsoluteNr])
        {
            ++chosenAvailableEntryAbsoluteNr;
        }
    }

    return chosenAvailableEntryAbsoluteNr;
}
