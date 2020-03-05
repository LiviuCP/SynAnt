/* Used by GameManager to avoid exposing full functionality to the Facade */

#ifndef GAMEFUNCTIONALITYPROXY_H
#define GAMEFUNCTIONALITYPROXY_H

#include <QObject>

#include "../ManagementInterfaces/gamefunctionality.h"

class GameManager;
class DataSource;
class DataAccessProxy;
class DataSourceAccessHelper;
class WordMixer;
class WordPairOwner;
class InputBuilder;
class StatisticsItem;
class Chronometer;

class GameFunctionalityProxy : public QObject, public IGameFunctionality
{
    Q_OBJECT
public:
    explicit GameFunctionalityProxy(QObject *parent = nullptr);

    DataAccessProxy* getDataAccessProxy() const;
    DataSourceAccessHelper* getDataSourceAccessHelper() const;
    WordMixer* getWordMixer() const;
    WordPairOwner* getWordPairOwner() const;
    InputBuilder* getInputBuilder() const;
    StatisticsItem* getStatisticsItem() const;
    Chronometer* getChronometer() const;
};

#endif // GAMEFUNCTIONALITYPROXY_H
