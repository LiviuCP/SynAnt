#ifndef DATAPROXY_H
#define DATAPROXY_H

#include <QObject>

#include "../ManagementInterfaces/data.h"

class GameManager;
class DataEntryFacade;

class DataProxy : public QObject, public IData
{
    Q_OBJECT
public:
    explicit DataProxy(QObject *parent = nullptr);
    DataEntryFacade* getDataEntryFacade() const;
};

#endif // DATAPROXY_H
