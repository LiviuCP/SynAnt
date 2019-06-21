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

void DataSource::fetchDataEntry(int entryNumber)
{
    Q_ASSERT(entryNumber >= 0 && entryNumber < m_DataEntries.size());

    DataEntry fetchedDataEntry{m_DataEntries[entryNumber]};
    Q_EMIT entryFetched(QPair<QString, QString>(fetchedDataEntry.firstWord, fetchedDataEntry.secondWord), fetchedDataEntry.areSynonyms);
}

int DataSource::getNrOfEntries()
{
    return m_DataEntries.size();
}

void DataSource::processRawDataEntryForTest(const QString &rawDataEntry)
{
    // row number is irrelevant as the method is only for testing purposes and no data is read from a file or database
    Q_UNUSED(_createProcessedDataEntry(rawDataEntry, -1));
}

void DataSource::onReadDataRequestReceived()
{
    QVector<QString> rawData;

    _loadRawData(rawData);
    _createProcessedDataEntries(rawData);

    // for sync purposes only
    QThread::msleep(Game::c_LoadDataThreadDelay);

    Q_EMIT dataReady();
}

void DataSource::onWriteDataRequestReceived(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    QString rawDataEntry;
    QFile wordPairsFile(m_DataFilePath);

    if(!wordPairsFile.open(QIODevice::Append))
    {
        throw FileException{GameStrings::c_CannotOpenFileMessage, m_DataFilePath};
    }

    bool success{_createRawDataEntry(rawDataEntry, newWordsPair.first, newWordsPair.second, areSynonyms)};

    if (success)
    {
        QTextStream lineWriter{&wordPairsFile};
        lineWriter << rawDataEntry << endl;

        // for sync purposes only
        QThread::msleep(Game::c_WriteDataThreadDelay);
    }

    Q_EMIT writeDataFinished(success);
}

void DataSource::_loadRawData(QVector<QString>& rawData)
{
    QFile wordPairsFile(m_DataFilePath);

    if (!wordPairsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw FileException{GameStrings::c_CannotOpenFileMessage, m_DataFilePath};
    }

    QTextStream lineReader{&wordPairsFile};

    while (!lineReader.atEnd())
    {
        rawData.append(lineReader.readLine());
    }

    wordPairsFile.close();
}

void DataSource::_createProcessedDataEntries(const QVector<QString>& rawData)
{
    for (int row{0}; row < rawData.size(); ++row)
    {
        m_DataEntries.append(_createProcessedDataEntry(rawData[row], row));
    }
}

DataSource::DataEntry DataSource::_createProcessedDataEntry(const QString& rawDataEntry, int rowNumber)
{
    auto checkWordIsCorrect = [this, rowNumber](const QString &word, const QString &wordIdentifier)
    {
        for (auto currentCharacter : word)
        {
            if (!(currentCharacter.isLower()))
            {
                throw WordException{GameStrings::c_IllegalCharactersMessage.arg(wordIdentifier), m_DataFilePath, rowNumber};
            }
        }

        if (word.size() < Game::c_MinWordSize)
        {
            throw WordException{GameStrings::c_LessThanMinWordCharsMessage.arg(wordIdentifier), m_DataFilePath, rowNumber};
        }
    };

    DataEntry currentDataEntry;

    if (rawDataEntry.size() == 0)
    {
        throw WordException{GameStrings::c_EmptyRowMessage, m_DataFilePath, rowNumber};
    }

    // total number of characters of the two words (excluding separator) should not be lower than the minimum required otherwise the game might become trivial
    if (rawDataEntry.size()-1 < Game::c_MinPairSize)
    {
        throw WordException{GameStrings::c_LessThanMinPairCharsMessage, m_DataFilePath, rowNumber};
    }

    // total maximum number of characters per pair is necessary for UI space/aesthetic reasons
    if (rawDataEntry.size()-1 > Game::c_MaxPairSize)
    {
        throw WordException{GameStrings::c_GreaterThanMaxPairCharsMessage, m_DataFilePath, rowNumber};
    }

    int synonymsSeparatorIndex{rawDataEntry.indexOf(c_SynonymsSeparator)};
    int antonymsSeparatorIndex{rawDataEntry.indexOf(c_AntonymsSeparator)};
    int separatorIndex{-1};

    if (synonymsSeparatorIndex != rawDataEntry.lastIndexOf(c_SynonymsSeparator) || antonymsSeparatorIndex != rawDataEntry.lastIndexOf(c_AntonymsSeparator))
    {
        throw WordException{GameStrings::c_MultipleSeparatorsMessage, m_DataFilePath, rowNumber};
    }
    else if (synonymsSeparatorIndex == antonymsSeparatorIndex)
    {
        throw WordException{GameStrings::c_NoSeparatorMessage, m_DataFilePath, rowNumber};
    }
    else if ((synonymsSeparatorIndex != -1) && (antonymsSeparatorIndex != -1))
    {
        throw WordException{GameStrings::c_MultipleSeparatorsMessage, m_DataFilePath, rowNumber};
    }
    else if (synonymsSeparatorIndex != -1)
    {
        currentDataEntry.areSynonyms = true;
        separatorIndex = synonymsSeparatorIndex;
    }
    else
    {
        currentDataEntry.areSynonyms = false;
        separatorIndex = antonymsSeparatorIndex;
    }

    currentDataEntry.firstWord = rawDataEntry.left(separatorIndex);
    checkWordIsCorrect(currentDataEntry.firstWord, GameStrings::c_FirstWordCamelCase);

    currentDataEntry.secondWord = rawDataEntry.mid(separatorIndex+1);
    checkWordIsCorrect(currentDataEntry.secondWord, GameStrings::c_SecondWordCamelCase);

    // don't allow identical words
    if (currentDataEntry.firstWord == currentDataEntry.secondWord)
    {
        throw WordException{GameStrings::c_SameWordsInPairMessage, m_DataFilePath, rowNumber};
    }

    return currentDataEntry;
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

    if (success)
    {
        rawDataEntry = firstWord + (areSynonyms ? c_SynonymsSeparator : c_AntonymsSeparator) + secondWord;
    }

    return success;
}

DataSource::DataEntry::DataEntry()
{
}
