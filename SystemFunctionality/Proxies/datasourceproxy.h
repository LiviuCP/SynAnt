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

    void loadDataFromDb();
    void saveDataToDbIfValid(QPair<QString, QString> newWordsPair, bool areSynonyms);
    void provideDataEntryToConsumer(int entryNumber);
    int getNrOfValidEntries();

signals:
    Q_SIGNAL void readDataFromDbFinished(bool success);
    Q_SIGNAL void writeDataToDbFinished(bool success);
    Q_SIGNAL void writeDataToDbErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);

private:
    DataSource* m_pDataSource;
};

#endif // DATASOURCEPROXY_H
