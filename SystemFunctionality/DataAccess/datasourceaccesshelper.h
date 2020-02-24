/*
   This class fulfills following tasks:
   1) Supports the facade connection to the DataSource.
   2) Generates a random (but valid and still not requested) entry number based on which the DataSource delivers the required data to the consumer.
*/

#ifndef DATASOURCEACCESS_H
#define DATASOURCEACCESS_H

#include <QObject>
#include <QVector>

#include <random>

class DataSourceAccessHelper : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceAccessHelper(QObject *parent = nullptr);

    void setEntriesTable(int nrOfEntries);
    void resetUsedEntries();
    void addEntriesToTable(int nrOfEntries);
    void clearEntriesTable();
    int generateEntryNumber();

    int getNrOfUsedEntries() const;
    int getTotalNrOfEntries() const;

private:
    std::default_random_engine m_ChooseEntryNumberEngine;
    QVector<bool> m_EntryUsedStatuses;
};

#endif // DATASOURCEACCESS_H
