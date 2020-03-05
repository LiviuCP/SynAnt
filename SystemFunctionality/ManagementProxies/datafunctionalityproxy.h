#ifndef DATAFUNCTIONALITYPROXY_H
#define DATAFUNCTIONALITYPROXY_H

#include <QObject>

#include "../ManagementInterfaces/datafunctionality.h"

class GameManager;
class DataEntryProxy;

class DataFunctionalityProxy : public QObject, public IDataFunctionality
{
    Q_OBJECT
public:
    explicit DataFunctionalityProxy(QObject *parent = nullptr);
    DataEntryProxy* getDataEntryProxy() const;

};

#endif // DATAFUNCTIONALITYPROXY_H
