/*
   This class fulfills following tasks:
   1) Validates the new word pairs added to the game through the data entry page
   2) Stores the validated pairs into the data entry cache
*/

#ifndef DATAENTRYVALIDATOR_H
#define DATAENTRYVALIDATOR_H

#include <QObject>

#include "datasource.h"
#include "../Utilities/game.h"

class DataEntryValidator : public QObject
{
    Q_OBJECT
public:
    explicit DataEntryValidator(DataSource* pDataSource, QObject *parent = nullptr);

    void validateWordsPair(QPair<QString, QString> newWordsPair, bool areSynonyms);
    Game::ValidationCodes getValidationCode() const;

signals:
    Q_SIGNAL void invalidWordsPairAddedByUser();
    Q_SIGNAL void entryValidated(DataSource::DataEntry rawDataEntry);

private:
    bool _isValidDataEntry(DataSource::DataEntry& rawDataEntry, const QString& firstWord, const QString& secondWord, bool areSynonyms);

    Game::ValidationCodes m_ValidationCode;
    DataSource* m_pDataSource;
};

#endif // DATAENTRYVALIDATOR_H
