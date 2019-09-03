#ifndef DATAPROXY_H
#define DATAPROXY_H

#include <QObject>

class GameManager;
class DataEntryFacade;

class DataProxy : public QObject
{
    Q_OBJECT
public:
    explicit DataProxy(QObject *parent = nullptr);
    DataEntryFacade* getDataEntryFacade();
};

#endif // DATAPROXY_H
