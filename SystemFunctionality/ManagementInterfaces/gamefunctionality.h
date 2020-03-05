#ifndef GAMEFUNCTIONALITY_H
#define GAMEFUNCTIONALITY_H

#include "../CoreFunctionality/wordmixer.h"
#include "../CoreFunctionality/wordpairowner.h"
#include "../CoreFunctionality/inputbuilder.h"
#include "../DataAccess/datasourceaccesshelper.h"
#include "../ManagementProxies/dataaccessproxy.h"
#include "../Utilities/statisticsitem.h"
#include "../Utilities/chronometer.h"

class IGameFunctionality
{
public:
    virtual ~IGameFunctionality() = 0;
    virtual DataAccessProxy* getDataAccessProxy() const = 0;
    virtual DataSourceAccessHelper* getDataSourceAccessHelper() const = 0;
    virtual WordMixer* getWordMixer() const = 0;
    virtual WordPairOwner* getWordPairOwner() const = 0;
    virtual InputBuilder* getInputBuilder() const = 0;
    virtual StatisticsItem* getStatisticsItem() const = 0;
    virtual Chronometer* getChronometer() const = 0;
};

#endif // GAMEFUNCTIONALITY_H
