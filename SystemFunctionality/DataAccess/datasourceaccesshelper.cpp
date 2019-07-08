#include "datasourceaccesshelper.h"

DataSourceAccessHelper::DataSourceAccessHelper(QObject *parent)
    : QObject(parent)
{
    std::random_device randomDevice{};
    m_ChooseEntryNumberEngine.seed(randomDevice());
}

void DataSourceAccessHelper::setEntriesTable(int nrOfEntries)
{
    Q_ASSERT(nrOfEntries > 0);
    m_EntryUsedStatuses.fill(false, nrOfEntries);
}

void DataSourceAccessHelper::resetUsedEntries()
{
    m_EntryUsedStatuses.fill(false, m_EntryUsedStatuses.size());
}

void DataSourceAccessHelper::addEntries(int nrOfEntries)
{
    Q_ASSERT(nrOfEntries > 0);
    QVector<bool> newEntriesUsedStatuses;
    newEntriesUsedStatuses.fill(false, nrOfEntries);
    m_EntryUsedStatuses.append(newEntriesUsedStatuses);
}

int DataSourceAccessHelper::generateEntryNumber()
{
    Q_ASSERT(m_EntryUsedStatuses.size() > 0);

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

int DataSourceAccessHelper::getNrOfUsedEntries() const
{
    return m_EntryUsedStatuses.count(true);
}

int DataSourceAccessHelper::getTotalNrOfEntries() const
{
    return m_EntryUsedStatuses.size();
}
