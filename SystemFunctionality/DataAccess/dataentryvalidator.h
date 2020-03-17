/*
   This class fulfills following tasks:
   1) Validates the new word pairs added to the game through the data entry page
   2) Stores the validated pairs into the data entry cache
*/

#ifndef DATAENTRYVALIDATOR_H
#define DATAENTRYVALIDATOR_H

#include <QObject>

#include "datasource.h"
//#include "../Utilities/dataentryutils.h"

class DataEntryValidator : public QObject
{
    Q_OBJECT
public:
    enum class ValidationCodes
    {
        NO_PAIR_VALIDATED,
        VALID_PAIR,
        LESS_MIN_CHARS_PER_WORD,
        LESS_MIN_TOTAL_PAIR_CHARS,
        MORE_MAX_TOTAL_PAIR_CHARS,
        INVALID_CHARACTERS,
        PAIR_ALREADY_EXISTS,
        IDENTICAL_WORDS,
        ValidationCodesCount
    };

    explicit DataEntryValidator(DataSource* pDataSource, QObject *parent = nullptr);

    void validateWordsPair(QPair<QString, QString> newWordsPair, bool areSynonyms, int languageIndex);
    ValidationCodes getValidationCode() const;

signals:
    Q_SIGNAL void addInvalidWordsPairRequested();
    Q_SIGNAL void entryValidated(DataSource::DataEntry rawDataEntry, int languageIndex);

private:
    bool _isValidDataEntry(DataSource::DataEntry& rawDataEntry, const QString& firstWord, const QString& secondWord, bool areSynonyms, int languageIndex);

    ValidationCodes m_ValidationCode;
    DataSource* m_pDataSource;
};

#endif // DATAENTRYVALIDATOR_H
