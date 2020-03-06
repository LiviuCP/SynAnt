#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QObject>

class IDataAccess
{
public:
    virtual void fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult) = 0;
    virtual void provideDataEntryToConsumer(int entryNumber) = 0;
    virtual int getNrOfDataSourceEntries() const = 0;
    virtual ~IDataAccess() = 0;

signals:
    Q_SIGNAL virtual void fetchDataForPrimaryLanguageFinished(bool success, bool validEntriesFetched) = 0;
    Q_SIGNAL virtual void fetchDataForSecondaryLanguageFinished(bool success) = 0;
    Q_SIGNAL virtual void primaryLanguageDataSavingFinished(int nrOfPrimaryLanguageSavedEntries) = 0;
    Q_SIGNAL virtual void writeDataToDbErrorOccured() = 0;
    Q_SIGNAL virtual void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms) = 0;
};

Q_DECLARE_INTERFACE(IDataAccess, "IDataAccess");

#endif // DATAACCESS_H
