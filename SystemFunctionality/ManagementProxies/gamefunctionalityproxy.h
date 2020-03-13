/* Used by GameManager to avoid exposing full functionality to the Facade */

#ifndef GAMEFUNCTIONALITYPROXY_H
#define GAMEFUNCTIONALITYPROXY_H

#include <QObject>

#include "../ManagementInterfaces/gamefunctionalityinterface.h"

class GameManager;
class DataSource;
class DataSourceAccessHelper;
class WordMixer;
class WordPairOwner;
class InputBuilder;
class StatisticsItem;
class Chronometer;

class GameFunctionalityProxy : public QObject, public IGameFunctionality
{
    Q_OBJECT
    Q_INTERFACES(IGameFunctionality)
public:
    explicit GameFunctionalityProxy(QObject *parent = nullptr);

    void fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult);
    void provideDataEntryToConsumer(int entryNumber);
    int getNrOfDataSourceEntries() const;

    DataSourceAccessHelper* getDataSourceAccessHelper() const;
    WordMixer* getWordMixer() const;
    WordPairOwner* getWordPairOwner() const;
    InputBuilder* getInputBuilder() const;
    StatisticsItem* getStatisticsItem() const;
    Chronometer* getChronometer() const;

signals:
    Q_SIGNAL void fetchDataForPrimaryLanguageFinished(bool success, bool validEntriesFetched);
    Q_SIGNAL void fetchDataForSecondaryLanguageFinished(bool success);
    Q_SIGNAL void primaryLanguageDataSavingFinished(int nrOfPrimaryLanguageSavedEntries);
    Q_SIGNAL void dataSavingErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);
};

#endif // GAMEFUNCTIONALITYPROXY_H
