#include <QMutexLocker>

#include "datasource.h"

DataSource::DataSource(const QString &dataBasePath, QObject *parent)
    : QObject (parent)
    , m_DataBasePath{dataBasePath}
    , m_DataEntries{}
    , m_PrimarySource{}
    , m_SecondarySource{}
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

void DataSource::updateDataEntries(const QVector<DataSource::DataEntry>& dataEntries, int languageIndex, DataSource::UpdateOperation updateOperation)
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};

    if (languageIndex >= 0)
    {
        switch (updateOperation)
        {
        case DataSource::UpdateOperation::LOAD_TO_PRIMARY:
            if (m_PrimarySource.languageIndex != -1)
            {
                m_SecondarySource.entries = m_PrimarySource.entries;
                m_SecondarySource.languageIndex = m_PrimarySource.languageIndex;
            }
            m_PrimarySource.entries = dataEntries;
            m_PrimarySource.languageIndex = languageIndex;
            break;
        case DataSource::UpdateOperation::LOAD_TO_SECONDARY:
            m_SecondarySource.entries = dataEntries;
            m_SecondarySource.languageIndex = languageIndex;
            break;
        case DataSource::UpdateOperation::SWAP:
            Q_UNUSED(dataEntries);
            std::swap(m_PrimarySource.entries, m_SecondarySource.entries);
            std::swap(m_PrimarySource.languageIndex, m_SecondarySource.languageIndex);
            break;
        case DataSource::UpdateOperation::APPEND:
            // to be updated
            break;
        }
    }
}

void DataSource::provideDataEntryToConsumer(int entryNumber)
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};

    Q_ASSERT(entryNumber >= 0 && entryNumber < m_PrimarySource.entries.size());

    DataEntry fetchedDataEntry{m_PrimarySource.entries[entryNumber]};
    Q_EMIT entryProvidedToConsumer(QPair<QString, QString>(fetchedDataEntry.firstWord, fetchedDataEntry.secondWord), fetchedDataEntry.areSynonyms);
}

int DataSource::getPrimarySourceLanguageIndex() const
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};
    return m_PrimarySource.languageIndex;
}

int DataSource::getSecondarySourceLanguageIndex() const
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};
    return m_SecondarySource.languageIndex;
}

int DataSource::getPrimarySourceNrOfEntries() const
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};
    return m_PrimarySource.entries.size();
}

int DataSource::getSecondarySourceNrOfEntries() const
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};
    return m_SecondarySource.entries.size();
}

QString DataSource::getDataFilePath() const
{
    return m_DataBasePath;
}

bool DataSource::entryAlreadyExists(const DataSource::DataEntry &dataEntry)
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};
    return m_DataEntries.contains(dataEntry);
}

bool DataSource::entryAlreadyExists(const DataSource::DataEntry &dataEntry, int languageIndex)
{
    QMutexLocker mutexLocker{&m_DataSourceMutex};
    return languageIndex == m_PrimarySource.languageIndex ? m_PrimarySource.entries.contains(dataEntry)
                                                          : languageIndex == m_SecondarySource.languageIndex ? m_SecondarySource.entries.contains(dataEntry)
                                                                                                             : false;
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

bool DataSource::DataEntry::operator==(const DataSource::DataEntry& dataEntry) const
{
    // for the moment the synonym/antonym flag is not taken into consideration
    return ((firstWord == dataEntry.firstWord && secondWord == dataEntry.secondWord) || (firstWord == dataEntry.secondWord && secondWord == dataEntry.firstWord));
}

DataSource::Source::Source()
    : languageIndex{-1}
    , entries{}
{
}
