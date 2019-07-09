#ifndef DATASOURCEPROXY_H
#define DATASOURCEPROXY_H

#include <QObject>

class DataSource;
class DataEntryValidator;
class DataEntryCache;
class GameManager;

class DataSourceProxy : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceProxy(QObject *parent = nullptr);

    void loadDataFromDb();
    void saveDataToDb();
    void resetDataEntryCache();
    void provideDataEntryToConsumer(int entryNumber);
    void validateWordsPair(QPair<QString, QString> newWordsPair, bool areSynonyms);
    int getNrOfValidEntries();
    int getNrOfCachedEntries();

signals:
    Q_SIGNAL void newWordsPairValidated(bool isValid);
    Q_SIGNAL void newWordsPairAddedToCache();
    Q_SIGNAL void cacheReset();
    Q_SIGNAL void readDataFromDbFinished(bool success);
    Q_SIGNAL void writeDataToDbFinished(int nrOfEntries);
    Q_SIGNAL void writeDataToDbErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);

private:
    DataSource* m_pDataSource;
    DataEntryValidator* m_pDataEntryValidator;
    DataEntryCache* m_pDataEntryCache;
};

#endif // DATASOURCEPROXY_H
