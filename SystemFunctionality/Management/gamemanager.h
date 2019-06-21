/*
   This class is responsible for building the backend of the game and can thus be considered the "backbone" of the application:
   1) Creates the backend objects:
      - GameFacade
      - DataSource
      - DataSourceAccessHelper
      - WordMixer
      - WordPairOwner
      - InputBuilder
      - ScoreItem
   2) Connects the objects with each other
   3) Is responsible for creating threads requested for the execution of methods belonging to backend objects

   Other notes:
   - implemented as singleton so it is easily accessible from more parts of the code
   - it is protected by several "proxy" classes (GameInitProxy, FacadeSetupProxy, PresenterSetupProxy) so the external objects have only a "need to know" access to its data
*/

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>

class GameFacade;
class DataSource;
class DataSourceProxy;
class DataSourceAccessHelper;
class WordMixer;
class WordMixerProxy;
class WordPairOwner;
class InputBuilder;
class ScoreItem;

class GameManager : public QObject
{
    Q_OBJECT
public:
    static GameManager* getManager();
    static void releaseResources();

    void setDataSource(const QString& dataDirPath);
    void loadData();
    void requestAddPairToDataSource(QPair<QString, QString> newWordsPair, bool areSynonyms);

    GameFacade* getFacade() const;
    DataSourceProxy* getDataSourceProxy() const;
    DataSourceAccessHelper* getDataSourceAccessHelper() const;
    WordMixer* getWordMixer() const;
    WordPairOwner* getWordPairOwner() const;
    InputBuilder* getInputBuilder() const;
    ScoreItem* getScoreItem() const;

    virtual ~GameManager();

signals:
    Q_SIGNAL void dataSourceSetupCompleted();
    Q_SIGNAL void readData();
    Q_SIGNAL void writeDataRequested(QPair<QString, QString> newWordsPair, bool areSynonyms);

private slots:
    void _onDataSourceSetupCompleted();

private:
    explicit GameManager(QObject *parent = nullptr);

    static GameManager* s_pGameManager;

    GameFacade* m_pGameFacade;
    DataSource* m_pDataSource;
    DataSourceProxy* m_pDataSourceProxy;
    DataSourceAccessHelper* m_pDataSourceAccessHelper;
    WordMixer* m_pWordMixer;
    WordMixerProxy* m_pWordMixerProxy;
    WordPairOwner* m_pWordPairOwner;
    InputBuilder* m_pInputBuilder;
    ScoreItem* m_pScoreItem;
    QThread* m_pDataSourceThread;
};

#endif // GAMEMANAGER_H
