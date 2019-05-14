#include "datasourceaccess.h"
#include "datasource.h"

DataSourceAccessHelper::DataSourceAccessHelper(QObject *parent)
    : QObject(parent)
{
    std::random_device randomDevice{};
    m_ChooseEntryNumberEngine.seed(randomDevice());
}

void DataSourceAccessHelper::setEntriesTable(int nrOfEntries)
{
    m_EntryUsedStatuses.fill(false, nrOfEntries);
}

int DataSourceAccessHelper::generateEntryNumber()
{
    Q_ASSERT(m_EntryUsedStatuses.size());

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

    m_EntryUsedStatuses[chosenAvailableEntryAbsoluteNr] = true;

    return chosenAvailableEntryAbsoluteNr;
}
