#include <QFile>
#include <QTextStream>
#include <QThread>

#include "datasource.h"
#include "Utilities/exceptions.h"
#include "Utilities/gamestrings.h"

DataSource::DataSource(const QString &dataFilePath, QObject *parent)
    : QObject (parent)
    , m_DataFilePath{dataFilePath}
    , m_DataEntries{}
{
}

void DataSource::updateDataEntries(QVector<DataSource::DataEntry> dataEntries, bool append)
{
    if (append)
    {
        m_DataEntries.append(dataEntries);
    }
    else
    {
        m_DataEntries = dataEntries;
    }

    Q_EMIT entriesUpdated();
}

void DataSource::provideDataEntryToConsumer(int entryNumber)
{
    Q_ASSERT(entryNumber >= 0 && entryNumber < m_DataEntries.size());

    DataEntry fetchedDataEntry{m_DataEntries[entryNumber]};
    Q_EMIT entryProvidedToConsumer(QPair<QString, QString>(fetchedDataEntry.firstWord, fetchedDataEntry.secondWord), fetchedDataEntry.areSynonyms);
}

int DataSource::getNrOfValidEntries() const
{
    return m_DataEntries.size();
}

QString DataSource::getDataFilePath() const
{
    return m_DataFilePath;
}

void DataSource::onWriteDataToDbRequested(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    QString rawDataEntry;
    QFile wordPairsFile(m_DataFilePath);

    if (_createRawDataEntry(rawDataEntry, newWordsPair.first, newWordsPair.second, areSynonyms))
    {
        if(!wordPairsFile.open(QIODevice::Append))
        {
            // user entered data valid but error when writing to DB
            Q_EMIT writeDataToDbErrorOccured();
        }
        else
        {
            QTextStream lineWriter{&wordPairsFile};
            lineWriter << rawDataEntry << endl;

            m_DataEntries.append(DataEntry{newWordsPair.first, newWordsPair.second, areSynonyms});

            // for sync purposes only
            QThread::msleep(Game::c_WriteDataThreadDelay);

            // user entered data written successfully to DB
            Q_EMIT writeDataToDbFinished(true);
        }
    }
    else
    {
        // no write error but invalid data entered by user
        Q_EMIT writeDataToDbFinished(false);
    }
}

bool DataSource::_createRawDataEntry(QString& rawDataEntry, const QString &firstWord, const QString &secondWord, bool areSynonyms)
{
    auto hasValidCharacters = [](const QString &word)
    {
        bool areAllCharactersValid{true};

        for (auto currentCharacter : word)
        {
            if (!(currentCharacter.isLower()))
            {
                areAllCharactersValid = false;
                break;
            }
        }

        return areAllCharactersValid;
    };

    bool success{true};

    success = success && (firstWord.size() >= Game::c_MinWordSize && secondWord.size() >= Game::c_MinWordSize);
    success = success && (firstWord.size() + secondWord.size() >= Game::c_MinPairSize);
    success = success && (firstWord.size() + secondWord.size() <= Game::c_MaxPairSize);
    success = success && (hasValidCharacters(firstWord) && hasValidCharacters(secondWord));
    success = success && (firstWord != secondWord);
    success = success && !_entryAlreadyExists(DataEntry{firstWord, secondWord, areSynonyms});

    if (success)
    {
        rawDataEntry = firstWord + (areSynonyms ? GameStrings::c_SynonymsSeparator : GameStrings::c_AntonymsSeparator) + secondWord;
    }

    return success;
}

bool DataSource::_entryAlreadyExists(const DataSource::DataEntry &dataEntry)
{
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
