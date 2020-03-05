#ifndef DATAACCESSPROXY_H
#define DATAACCESSPROXY_H

#include <QObject>

#include "../ManagementInterfaces/dataaccess.h"
#include "../Utilities/gameutils.h"

class GameManager;

class DataAccessProxy : public QObject,
                              public IDataAccess
{
    Q_OBJECT
public:
    explicit DataAccessProxy(QObject *parent = nullptr);

    void fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult);
    void provideDataEntryToConsumer(int entryNumber);
    int getNrOfDataSourceEntries() const;

signals:
    Q_SIGNAL void fetchDataForPrimaryLanguageFinished(bool success, bool validEntriesFetched);
    Q_SIGNAL void fetchDataForSecondaryLanguageFinished(bool success);
    Q_SIGNAL void primaryLanguageDataSavingFinished(int nrOfPrimaryLanguageSavedEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);
};

#endif // DATAACCESSPROXY_H
