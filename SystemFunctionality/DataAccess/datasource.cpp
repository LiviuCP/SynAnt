#include <QFile>
#include <QTextStream>
#include <QThread>

#include "datasource.h"
#include "Utilities/exceptions.h"
#include "Utilities/gamestrings.h"

static constexpr char c_SynonymsSeparator{'='};
static constexpr char c_AntonymsSeparator{'!'};

DataSource::DataSource(const QString &dataFilePath, QObject *parent)
    : QObject (parent)
    , m_DataFilePath{dataFilePath}
    , m_DataEntries{}
{
}

void DataSource::provideDataEntryToConsumer(int entryNumber)
{
    Q_ASSERT(entryNumber >= 0 && entryNumber < m_DataEntries.size());

    DataEntry fetchedDataEntry{m_DataEntries[entryNumber]};
    Q_EMIT entryProvidedToConsumer(QPair<QString, QString>(fetchedDataEntry.firstWord, fetchedDataEntry.secondWord), fetchedDataEntry.areSynonyms);
}

int DataSource::getNrOfValidEntries()
{
    return m_DataEntries.size();
}

bool DataSource::processRawDataEntryForTest(const QString &rawDataEntry)
{
    // row number is irrelevant as the method is only for testing purposes and no data is read from a file or database
    DataEntry dataEntry;
    return _createProcessedDataEntry(dataEntry, rawDataEntry);
}

void DataSource::onReadDataFromDbRequested()
{
    bool success{true};

    QVector<QString> rawData;

    if (_loadRawData(rawData))
    {
        _createProcessedDataEntries(rawData);

        // for sync purposes only
        QThread::msleep(Game::c_LoadDataThreadDelay);
    }
    else
    {
        success = false;
    }

    Q_EMIT readDataFromDbFinished(success);
}

void DataSource::onWriteDataToDbRequested(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    QString rawDataEntry;
    QFile wordPairsFile(m_DataFilePath);

    if(!wordPairsFile.open(QIODevice::Append))
    {
        Q_EMIT writeDataToDbErrorOccured();
    }
    else
    {
        bool success{_createRawDataEntry(rawDataEntry, newWordsPair.first, newWordsPair.second, areSynonyms)};

        if (success)
        {
            QTextStream lineWriter{&wordPairsFile};
            lineWriter << rawDataEntry << endl;

            m_DataEntries.append(DataEntry{newWordsPair.first, newWordsPair.second, areSynonyms});

            // for sync purposes only
            QThread::msleep(Game::c_WriteDataThreadDelay);
        }

        Q_EMIT writeDataToDbFinished(success);
    }
}

bool DataSource::_loadRawData(QVector<QString>& rawData)
{
    bool success{true};

    QFile wordPairsFile(m_DataFilePath);

    if (!wordPairsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        success = false;
    }
    else
    {
        QTextStream lineReader{&wordPairsFile};

        while (!lineReader.atEnd())
        {
            rawData.append(lineReader.readLine());
        }

        wordPairsFile.close();
    }

    return success;
}

void DataSource::_createProcessedDataEntries(const QVector<QString>& rawData)
{
    for (int row{0}; row < rawData.size(); ++row)
    {
        DataEntry dataEntry;

        if (_createProcessedDataEntry(dataEntry, rawData[row]))
        {
            m_DataEntries.append(dataEntry);
        }
    }
}

bool DataSource::_createProcessedDataEntry(DataEntry& dataEntry, const QString& rawDataEntry)
{
    auto isValidWord = [](const QString &word)
    {
        bool success{true};

        for (auto currentCharacter : word)
        {
            if (!currentCharacter.isLower())
            {
                success = false;
                break;
            }
        }

        return (success && (word.size() >= Game::c_MinWordSize));
    };

    bool success{true};

    success = success && (rawDataEntry.size() != 0);

    // total number of characters of the two words (excluding separator) should not be lower than the minimum required otherwise the game might become trivial
    success = success && (rawDataEntry.size()-1 >= Game::c_MinPairSize);

    // total maximum number of characters per pair is necessary for UI space/aesthetic reasons
    success = success && (rawDataEntry.size()-1 <= Game::c_MaxPairSize);

    int synonymsSeparatorIndex{rawDataEntry.indexOf(c_SynonymsSeparator)};
    int antonymsSeparatorIndex{rawDataEntry.indexOf(c_AntonymsSeparator)};
    int separatorIndex{-1};

    // there should be exactly one separator (either = for synonyms or ! for antonyms)
    success = success && (synonymsSeparatorIndex == rawDataEntry.lastIndexOf(c_SynonymsSeparator));
    success = success && (antonymsSeparatorIndex == rawDataEntry.lastIndexOf(c_AntonymsSeparator));
    success = success && (synonymsSeparatorIndex != antonymsSeparatorIndex);
    success = success && (synonymsSeparatorIndex == -1 || antonymsSeparatorIndex == -1);

    if (success)
    {
        separatorIndex = synonymsSeparatorIndex != -1 ? synonymsSeparatorIndex : antonymsSeparatorIndex;

        dataEntry.areSynonyms = synonymsSeparatorIndex != -1 ? true : false;
        dataEntry.firstWord = rawDataEntry.left(separatorIndex);
        dataEntry.secondWord = rawDataEntry.mid(separatorIndex+1);

        // don't allow words containing invalid characters (only small letters allowed)
        success = success && (isValidWord(dataEntry.firstWord) && isValidWord(dataEntry.secondWord));

        // don't allow identical words
        success = success && (dataEntry.firstWord != dataEntry.secondWord);
    }

    return success;
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
        rawDataEntry = firstWord + (areSynonyms ? c_SynonymsSeparator : c_AntonymsSeparator) + secondWord;
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
