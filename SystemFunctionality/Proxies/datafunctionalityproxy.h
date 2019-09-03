#ifndef DATAFUNCTIONALITYPROXY_H
#define DATAFUNCTIONALITYPROXY_H

#include <QObject>

class GameManager;
class DataEntryProxy;

class DataFunctionalityProxy : public QObject
{
    Q_OBJECT
public:
    explicit DataFunctionalityProxy(QObject *parent = nullptr);
    DataEntryProxy* getDataEntryProxy() const;

};

#endif // DATAFUNCTIONALITYPROXY_H
