/*
   This class is responsible for building the backend of the game and can thus be considered the "backbone" of the application:

    1) Creates the backend objects:
       - GameFacade
       - DataSource
       - DataSourceAccessHelper
       - DataSourceLoader
       - DataEntryValidator
       - DataEntryCache
       - WordMixer
       - WordPairOwner
       - InputBuilder
       - StatisticsItem
       - Chronometer
    2) Sets up database and manages the data connections (data source, loader, validator and cache); controls the data related functionality (loading, entry, validation, save to DB)
    3) Makes the non-facade game components connections (InputBuilder, WordPairOwner, WordMixer)
    4) Is responsible for creating/managing threads

   Other notes:
   - implemented as singleton so it is easily accessible from more parts of the code
   - it is protected by several "proxy" classes (GameInitProxy, FacadeSetupProxy, PresenterSetupProxy) so the external objects have only a "need to know" access to its data
*/

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

#include "../ManagementInterfaces/gameinit.h"
#include "../ManagementInterfaces/game.h"
#include "../ManagementInterfaces/gamefunctionality.h"
#include "../ManagementInterfaces/dataaccess.h"
#include "../ManagementInterfaces/dataentry.h"
#include "../ManagementInterfaces/data.h"
#include "../ManagementInterfaces/datafunctionality.h"
#include "../Utilities/dataentryutils.h"

class GameFacade;
class DataEntryFacade;
class DataSource;
class DataSourceLoader;
class DataEntryValidator;
class DataEntryCache;
class DataEntryStatistics;
class DataAccessProxy;
class DataEntryProxy;
class DataSourceAccessHelper;
class WordMixer;
class WordMixerProxy;
class WordPairOwner;
class InputBuilder;
class StatisticsItem;
class Chronometer;

class GameManager : public QObject,
                    public IGameInit,
                    public IGame,
                    public IGameFunctionality,
                    public IDataAccess,
                    public IDataEntry,
                    public IData,
                    public IDataFunctionality
{
    Q_OBJECT
    Q_INTERFACES(IDataAccess)
    Q_INTERFACES(IDataEntry)
public:
    static GameManager* getManager();

    void setDataSource(const QString& dataDirPath);
    void fetchDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult);
    void fetchDataForSecondaryLanguage(int languageIndex);
    void saveDataToDb();
    void requestWriteToCache(QPair<QString, QString> newWordsPair, bool areSynonyms, int languageIndex);
    void requestCacheReset();
    void provideDataEntryToConsumer(int entryNumber);
    void releaseResources();

    int getNrOfDataSourceEntries() const;
    DataEntry::ValidationCodes getPairEntryValidationCode() const;

    GameFacade* getGameFacade() const;
    DataEntryFacade* getDataEntryFacade() const;

    DataAccessProxy* getDataAccessProxy() const;
    DataEntryProxy* getDataEntryProxy() const;
    DataSourceAccessHelper* getDataSourceAccessHelper() const;
    WordMixer* getWordMixer() const;
    WordPairOwner* getWordPairOwner() const;
    InputBuilder* getInputBuilder() const;
    StatisticsItem* getStatisticsItem() const;
    Chronometer* getChronometer() const;

    int getLastSavedTotalNrOfEntries() const;
    int getLastNrOfEntriesSavedToPrimaryLanguage() const;
    int getCurrentNrOfCacheEntries() const;

signals:
    // data access proxy
    Q_SIGNAL void fetchDataForPrimaryLanguageFinished(bool success, bool validEntriesLoaded);
    Q_SIGNAL void fetchDataForSecondaryLanguageFinished(bool success);
    Q_SIGNAL void primaryLanguageDataSavingFinished(int nrOfPrimaryLanguageSavedEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);

    // data entry proxy
    Q_SIGNAL void dataEntryAllowed(bool allowed);
    Q_SIGNAL void newWordsPairAddedToCache();
    Q_SIGNAL void addInvalidWordsPairRequested();
    Q_SIGNAL void wordsPairAlreadyContainedInCache();
    Q_SIGNAL void cacheReset();
    Q_SIGNAL void writeDataToDbFinished();
    Q_SIGNAL void fetchDataForDataEntryLanguageFinished(bool success);

    // data entry statistics
    Q_SIGNAL void recordAddedPairRequested();
    Q_SIGNAL void dataSavedStatisticsUpdateRequested(int nrOfPrimaryLanguageSavedEntries, int totalNrOfSavedEntries);
    Q_SIGNAL void currentEntriesStatisticsResetRequested();

    // data source, loader, cache
    Q_SIGNAL void dataSourceSetupCompleted();
    Q_SIGNAL void readDataForPrimaryLanguage(int languageIndex, bool allowEmptyResult);
    Q_SIGNAL void readDataForSecondaryLanguage(int languageIndex);
    Q_SIGNAL void writeDataToDb();
    Q_SIGNAL void resetCacheRequested();

private slots:
    void _onDataSourceSetupCompleted();
    void _onLoadDataFromDbForPrimaryLanguageFinished(bool success, bool validEntriesLoaded);
    void _onRequestedPrimaryLanguageAlreadyContainedInDataSource(bool validEntriesLoaded);
    void _onLoadDataFromDbForSecondaryLanguageFinished(bool success);
    void _onRequestedSecondaryLanguageAlreadySetAsPrimary();
    void _onNewWordsPairAddedToCache();
    void _onWriteDataToDbFinished(int nrOfPrimaryLanguageSavedEntries, int totalNrOfSavedEntries);
    void _onCacheReset();
    void _onWriteDataToDbErrorOccured();
    void _onWordsPairAlreadyContainedInCache();
    void _onAddInvalidWordsPairRequested();
    void _onEntryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);

private:
    explicit GameManager(QObject *parent = nullptr);
    virtual ~GameManager();

    static void _deallocResources();

    void _setDatabase(const QString& databasePath);
    void _makeDataConnections();
    void _registerMetaTypes();

    static GameManager* s_pGameManager;

    GameFacade* m_pGameFacade;
    DataEntryFacade* m_pDataEntryFacade;
    DataSource* m_pDataSource;
    DataSourceLoader* m_pDataSourceLoader;
    DataEntryValidator* m_pDataEntryValidator;
    DataEntryCache* m_pDataEntryCache;
    DataEntryStatistics* m_pDataEntryStatistics;
    DataAccessProxy* m_pDataAccessProxy;
    DataEntryProxy* m_pDataEntryProxy;
    DataSourceAccessHelper* m_pDataSourceAccessHelper;
    WordMixer* m_pWordMixer;
    WordPairOwner* m_pWordPairOwner;
    InputBuilder* m_pInputBuilder;
    StatisticsItem* m_pStatisticsItem;
    Chronometer* m_pChronometer;
    QThread* m_pDataSourceLoaderThread;
    QThread* m_pDataEntryCacheThread;
};

#endif // GAMEMANAGER_H
