#ifndef DATASOURCEPROXY_H
#define DATASOURCEPROXY_H

#include <QObject>

class DataSource;
class GameManager;

class DataSourceProxy : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceProxy(DataSource* pDataSource, QObject *parent = nullptr);

    void loadData();
    void requestAddPairToDataSource(QPair<QString, QString> newWordsPair, bool areSynonyms);
    void fetchDataEntry(int entryNumber);
    int getNrOfEntries();

signals:
    Q_SIGNAL void readDataFinished(bool success);
    Q_SIGNAL void writeDataFinished(bool success);
    Q_SIGNAL void entryFetched(QPair<QString, QString> newWordsPair, bool areSynonyms);

private:
    DataSource* m_pDataSource;
};

#endif // DATASOURCEPROXY_H
