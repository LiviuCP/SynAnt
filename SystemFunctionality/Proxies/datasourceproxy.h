#ifndef DATASOURCEPROXY_H
#define DATASOURCEPROXY_H

#include <QObject>

#include "../Utilities/gameutils.h"

class GameManager;

class DataSourceProxy : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceProxy(QObject *parent = nullptr);

    void fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult);
    void provideDataEntryToConsumer(int entryNumber);
    int getNrOfDataSourceEntries();

signals:
    Q_SIGNAL void fetchDataForPrimaryLanguageFinished(bool success, bool validEntriesFetched);
    Q_SIGNAL void fetchDataForSecondaryLanguageFinished(bool success);
    Q_SIGNAL void writeDataToDbFinished(int nrOfPrimaryLanguageSavedEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);
};

#endif // DATASOURCEPROXY_H
