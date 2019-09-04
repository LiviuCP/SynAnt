#include "dataentryvalidator.h"
#include "../Utilities/gameutils.h"

DataEntryValidator::DataEntryValidator(DataSource* pDataSource, QObject *parent)
    : QObject(parent)
    , m_ValidationCode{DataEntry::ValidationCodes::NO_PAIR_VALIDATED}
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
        Q_EMIT entryValidated(dataEntry);
    }
    else
    {
        Q_EMIT addInvalidWordsPairRequested();
    }
}

DataEntry::ValidationCodes DataEntryValidator::getValidationCode() const
{
    return m_ValidationCode;
}

bool DataEntryValidator::_isValidDataEntry(DataSource::DataEntry& dataEntry, const QString &firstWord, const QString &secondWord, bool areSynonyms)
{
    auto hasInvalidCharacters = [](const QString &word)
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

        return !areAllCharactersValid;
    };

    bool success{true};

    if (firstWord.size() < Game::Constraints::c_MinWordSize || secondWord.size() < Game::Constraints::c_MinWordSize)
    {
        m_ValidationCode = DataEntry::ValidationCodes::LESS_MIN_CHARS_PER_WORD;
    }
    else if (firstWord.size() + secondWord.size() < Game::Constraints::c_MinPairSize)
    {
        m_ValidationCode = DataEntry::ValidationCodes::LESS_MIN_TOTAL_PAIR_CHARS;
    }
    else if (firstWord.size() + secondWord.size() > Game::Constraints::c_MaxPairSize)
    {
        m_ValidationCode = DataEntry::ValidationCodes::MORE_MAX_TOTAL_PAIR_CHARS;
    }
    else if (hasInvalidCharacters(firstWord) || hasInvalidCharacters(secondWord))
    {
        m_ValidationCode = DataEntry::ValidationCodes::INVALID_CHARACTERS;
    }
    else if (firstWord == secondWord)
    {
        m_ValidationCode = DataEntry::ValidationCodes::IDENTICAL_WORDS;
    }
    else if (m_pDataSource->entryAlreadyExists(DataSource::DataEntry{firstWord, secondWord, areSynonyms}))
    {
        m_ValidationCode = DataEntry::ValidationCodes::PAIR_ALREADY_EXISTS;
    }
    else
    {
        m_ValidationCode = DataEntry::ValidationCodes::VALID_PAIR;
    }

    if (m_ValidationCode == DataEntry::ValidationCodes::VALID_PAIR)
    {
        dataEntry = DataSource::DataEntry{firstWord, secondWord, areSynonyms};
    }
    else
    {
        success = false;
    }

    return success;
}
