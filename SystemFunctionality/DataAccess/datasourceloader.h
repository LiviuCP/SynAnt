#ifndef DATASOURCELOADER_H
#define DATASOURCELOADER_H

#include <QObject>

#include "datasource.h"

class DataSourceLoader : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceLoader(DataSource* pDataSource, QObject *parent = nullptr);

    // for test purposes only
    bool processRawDataEntryForTest(const QString& rawDataEntry);

public slots:
    void onLoadDataFromDbRequested();

signals:
    Q_SIGNAL void readDataFromDbFinished(bool success);

private:
    bool _loadRawData(QVector<QString>& rawData);
    void _convertToValidDataEntries(const QVector<QString>& rawData);
    bool _createValidDataEntry(DataSource::DataEntry& dataEntry, const QString& rawDataEntry);

    QVector<DataSource::DataEntry> m_ValidDataEntries;
    DataSource* m_pDataSource;
};

#endif // DATASOURCELOADER_H
