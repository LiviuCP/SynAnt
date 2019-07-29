/*
   This class fulfills following tasks:
   1) Loads the valid word pairs from database
   2) Hands the loaded data to the datasource
*/

#ifndef DATASOURCELOADER_H
#define DATASOURCELOADER_H

#include <QObject>

#include "datasource.h"

class DataSourceLoader : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceLoader(DataSource* pDataSource, QObject *parent = nullptr);

public slots:
    void onLoadDataFromDbRequested();

signals:
    Q_SIGNAL void loadDataFromDbFinished(bool success);

private:
    bool _loadEntriesFromDb(QVector<DataSource::DataEntry>& dbEntries);
    void _validateLoadedDataEntries(const QVector<DataSource::DataEntry> dbEntries);
    bool _isValidDataEntry(const DataSource::DataEntry& dataEntry);

    QVector<DataSource::DataEntry> m_ValidDataEntries;
    DataSource* m_pDataSource;
};

#endif // DATASOURCELOADER_H
