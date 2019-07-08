#include <QMutexLocker>

#include "datasource.h"

DataSource::DataSource(const QString &dataFilePath, QObject *parent)
    : QObject (parent)
    , m_DataFilePath{dataFilePath}
    , m_DataEntries{}
    , m_DataSourceMutex{}
{
}

void DataSource::updateDataEntries(QVector<DataSource::DataEntry> dataEntries, bool append)
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};

    if (append)
    {
        m_DataEntries.append(dataEntries);
    }
    else
    {
        m_DataEntries = dataEntries;
    }
}

void DataSource::provideDataEntryToConsumer(int entryNumber)
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};

    Q_ASSERT(entryNumber >= 0 && entryNumber < m_DataEntries.size());

    DataEntry fetchedDataEntry{m_DataEntries[entryNumber]};
    Q_EMIT entryProvidedToConsumer(QPair<QString, QString>(fetchedDataEntry.firstWord, fetchedDataEntry.secondWord), fetchedDataEntry.areSynonyms);
}

int DataSource::getNrOfValidEntries() const
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};
    return m_DataEntries.size();
}

QString DataSource::getDataFilePath() const
{
    return m_DataFilePath;
}

bool DataSource::entryAlreadyExists(const DataSource::DataEntry &dataEntry)
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};

    bool entryExists{false};

    for (auto currentEntry: m_DataEntries)
    {
        if ((dataEntry.firstWord == currentEntry.firstWord && dataEntry.secondWord == currentEntry.secondWord) ||
            (dataEntry.firstWord == currentEntry.secondWord && dataEntry.secondWord == currentEntry.firstWord))
        {
            entryExists = true;
            break;
        }
    }

    return entryExists;
}

DataSource::DataEntry::DataEntry()
{
}

DataSource::DataEntry::DataEntry(const QString &firstWord, const QString &secondWord, bool areSynonyms)
    : firstWord{firstWord}
    , secondWord{secondWord}
    , areSynonyms{areSynonyms}
{
}
