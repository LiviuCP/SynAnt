#include <QVector>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlQuery>

#include "datasourceloader.h"
#include "../Utilities/gameutils.h"

const QVector<QString> c_LanguageCodes{"EN", "DE", "RO", "IT"};

DataSourceLoader::DataSourceLoader(DataSource* pDataSource, QObject *parent)
    : QObject(parent)
    , m_pDataSource{pDataSource}
{
    Q_ASSERT(m_pDataSource);
}

void DataSourceLoader::onLoadDataFromDbRequested(int languageIndex, bool allowEmptyResult)
{
    bool success{true};
    bool validEntriesLoaded{false};

    QVector<DataSource::DataEntry> loadedDataEntries;

    if (_loadEntriesFromDb(loadedDataEntries, languageIndex))
    {
        validEntriesLoaded = (loadedDataEntries.size() != 0);
        if (validEntriesLoaded)
        {
            _validateLoadedDataEntries(loadedDataEntries);
        }

        if (m_ValidDataEntries.size() != 0 || allowEmptyResult)
        {
            m_pDataSource->updateDataEntries(m_ValidDataEntries, false);
            m_ValidDataEntries.resize(0);
            m_ValidDataEntries.squeeze();
        }

        // for sync purposes only
        QThread::msleep(Game::Timing::c_LoadDataThreadDelay);
    }
    else
    {
        success = false;
    }

    Q_EMIT loadDataFromDbFinished(success, validEntriesLoaded);
}

bool DataSourceLoader::_loadEntriesFromDb(QVector<DataSource::DataEntry>& dbEntries, int languageIndex)
{
    bool success{true};

    Q_UNUSED(QSqlDatabase::addDatabase(Game::Database::c_DbDriverName));

    // ensure all database related objects are destroyed before the connection is removed
    {
        QSqlDatabase db{QSqlDatabase::database(QSqlDatabase::defaultConnection)};

        db.setDatabaseName(m_pDataSource->getDataFilePath());

        if (db.open())
        {
            QSqlQuery retrieveDataQuery{Game::Database::c_RetrieveEntriesFromLanguageQuery.arg(c_LanguageCodes[languageIndex])};
            if (retrieveDataQuery.isActive())
            {
                while (retrieveDataQuery.next())
                {
                    dbEntries.append(DataSource::DataEntry{retrieveDataQuery.value(1).toString(),                       // field 1: first word
                                                           retrieveDataQuery.value(2).toString(),                       // field 2: second word
                                                           static_cast<bool>(retrieveDataQuery.value(3).toInt())});     // field 3: synonym/antonym flag
                }
            }
            else
            {
                success = false;
            }

            db.close();
        }
        else
        {
            success = false;
        }
    }

    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    return success;
}

void DataSourceLoader::_validateLoadedDataEntries(const QVector<DataSource::DataEntry> dbEntries)
{
    m_ValidDataEntries.reserve(dbEntries.size());

    for (int row{0}; row < dbEntries.size(); ++row)
    {
        if (_isValidDataEntry(dbEntries[row]))
        {
            m_ValidDataEntries.append(dbEntries[row]);
        }
    }
}

bool DataSourceLoader::_isValidDataEntry(const DataSource::DataEntry &dataEntry)
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

        return (success && (word.size() >= Game::Constraints::c_MinWordSize));
    };

    bool isValidPair{true};
    const int totalPairSize{dataEntry.firstWord.size() + dataEntry.secondWord.size()};

    if (totalPairSize < Game::Constraints::c_MinPairSize || totalPairSize > Game::Constraints::c_MaxPairSize)
    {
        isValidPair = false;
    }
    else if (!isValidWord(dataEntry.firstWord) || !isValidWord(dataEntry.secondWord))
    {
        isValidPair = false;
    }
    else if (dataEntry.firstWord == dataEntry.secondWord)
    {
        isValidPair = false;
    }

    return isValidPair;
}
