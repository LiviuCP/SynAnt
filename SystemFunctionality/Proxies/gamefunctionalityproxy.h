/* Used by GameManager to avoid exposing full functionality to the Facade */

#ifndef GAMEFUNCTIONALITYPROXY_H
#define GAMEFUNCTIONALITYPROXY_H

#include <QObject>

class GameManager;
class DataSource;
class DataSourceProxy;
class DataSourceAccessHelper;
class WordMixer;
class WordPairOwner;
class InputBuilder;
class ScoreItem;

class GameFunctionalityProxy : public QObject
{
    Q_OBJECT
public:
    explicit GameFunctionalityProxy(QObject *parent = nullptr);

    DataSourceProxy* getDataSourceProxy() const;
    DataSourceAccessHelper* getDataSourceAccessHelper() const;
    WordMixer* getWordMixer() const;
    WordPairOwner* getWordPairOwner() const;
    InputBuilder* getInputBuilder() const;
    ScoreItem* getScoreItem() const;
};

#endif // GAMEFUNCTIONALITYPROXY_H
