/*
   This class fulfills following tasks:
   1) Connects to the DataSource and keeps track of the datasource entries that had already been delivered to the consumer class.
   2) Generates a random (but valid and still not requested) entry number based on which the DataSource delivers the required data to the consumer.
   3) Forwards the data request (by providing the required entry number) to the DataSource, which in turn sends the data to the consumer.
*/

#ifndef DATASOURCEACCESS_H
#define DATASOURCEACCESS_H

#include <QObject>
#include <QVector>

class DataSource;

class DataSourceAccess : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceAccess(QObject *parent = nullptr);

    void connectToDataSource(DataSource* pDataSource);
    void requestNewDataSourceEntry();

public slots:
    void onDataSourceReady();

private:
    int _generateEntryNumber();

    DataSource* m_pDataSource;
    std::default_random_engine m_ChooseEntryNumberEngine;
    QVector<bool> m_EntryUsedStatuses;
};

#endif // DATASOURCEACCESS_H
