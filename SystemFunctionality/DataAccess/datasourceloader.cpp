#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QThread>

#include "datasourceloader.h"
#include "../Utilities/gamestrings.h"

DataSourceLoader::DataSourceLoader(DataSource* pDataSource, QObject *parent)
    : QObject(parent)
    , m_pDataSource{pDataSource}
{
    Q_ASSERT(m_pDataSource);
}

bool DataSourceLoader::processRawDataEntryForTest(const QString &rawDataEntry)
{
    // row number is irrelevant as the method is only for testing purposes and no data is read from a file or database
    DataSource::DataEntry dataEntry;
    return _createValidDataEntry(dataEntry, rawDataEntry);
}

void DataSourceLoader::onLoadDataFromDbRequested()
{
    bool success{true};

    QVector<QString> rawData;

    if (_loadRawData(rawData))
    {
        _convertToValidDataEntries(rawData);

        m_pDataSource->updateDataEntries(m_ValidDataEntries, false);

        // for sync purposes only
        QThread::msleep(Game::c_LoadDataThreadDelay);
    }
    else
    {
        success = false;
    }

    Q_EMIT readDataFromDbFinished(success);
}

bool DataSourceLoader::_loadRawData(QVector<QString>& rawData)
{
    bool success{true};

    QFile wordPairsFile(m_pDataSource->getDataFilePath());

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

void DataSourceLoader::_convertToValidDataEntries(const QVector<QString> &rawData)
{
    for (int row{0}; row < rawData.size(); ++row)
    {
        DataSource::DataEntry dataEntry;

        if (_createValidDataEntry(dataEntry, rawData[row]))
        {
            m_ValidDataEntries.append(dataEntry);
        }
    }
}

bool DataSourceLoader::_createValidDataEntry(DataSource::DataEntry &dataEntry, const QString &rawDataEntry)
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

    int synonymsSeparatorIndex{rawDataEntry.indexOf(GameStrings::c_SynonymsSeparator)};
    int antonymsSeparatorIndex{rawDataEntry.indexOf(GameStrings::c_AntonymsSeparator)};
    int separatorIndex{-1};

    // there should be exactly one separator (either = for synonyms or ! for antonyms)
    success = success && (synonymsSeparatorIndex == rawDataEntry.lastIndexOf(GameStrings::c_SynonymsSeparator));
    success = success && (antonymsSeparatorIndex == rawDataEntry.lastIndexOf(GameStrings::c_AntonymsSeparator));
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
