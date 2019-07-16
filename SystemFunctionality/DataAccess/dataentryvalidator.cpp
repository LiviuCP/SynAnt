#include "dataentryvalidator.h"
#include "../Utilities/gamestrings.h"

DataEntryValidator::DataEntryValidator(DataSource* pDataSource, QObject *parent)
    : QObject(parent)
    , m_pDataSource{pDataSource}
{
    Q_ASSERT(pDataSource);
}

void DataEntryValidator::validateWordsPair(QPair<QString, QString> newWordsPair, bool areSynonyms)
{
    DataSource::DataEntry dataEntry;

    bool isEntryValid{_isValidDataEntry(dataEntry, newWordsPair.first, newWordsPair.second, areSynonyms)};

    if (isEntryValid)
    {
        entryValidated(dataEntry);
    }
    else
    {
        Q_EMIT invalidWordsPairAddedByUser();
    }
}

bool DataEntryValidator::_isValidDataEntry(DataSource::DataEntry& dataEntry, const QString &firstWord, const QString &secondWord, bool areSynonyms)
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
    success = success && !m_pDataSource->entryAlreadyExists(DataSource::DataEntry{firstWord, secondWord, areSynonyms});

    if (success)
    {
        dataEntry = DataSource::DataEntry{firstWord, secondWord, areSynonyms};
    }

    return success;
}
