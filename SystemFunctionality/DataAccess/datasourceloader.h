/*
   This class fulfills following tasks:
   1) Loads the valid word pairs from database for the chosen language
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
    explicit DataSourceLoader(DataSource* pDataSource, QString dataBasePath, QObject *parent = nullptr);

public slots:
    void onLoadDataFromDbForPrimaryLanguageRequested(int languageIndex, bool allowEmptyResult);
    void onLoadDataFromDbForSecondaryLanguageRequested(int languageIndex);

signals:
    Q_SIGNAL void loadDataFromDbForPrimaryLanguageFinished(bool success, bool validEntriesLoaded);
    Q_SIGNAL void requestedPrimaryLanguageAlreadyContainedInDataSource(bool entriesAvailable);
    Q_SIGNAL void loadDataFromDbForSecondaryLanguageFinished(bool success);
    Q_SIGNAL void requestedSecondaryLanguageAlreadySetAsPrimary();

private:
    bool _loadEntriesFromDb(QVector<DataSource::DataEntry>& dbEntries, int languageIndex);
    void _validateLoadedDataEntries(const QVector<DataSource::DataEntry> dbEntries);
    bool _isValidDataEntry(const DataSource::DataEntry& dataEntry);

    static constexpr int sc_LoadDataThreadDelay{1000};

    QVector<DataSource::DataEntry> m_ValidDataEntries;
    DataSource* m_pDataSource;
    QString m_DatabasePath;
};

#endif // DATASOURCELOADER_H
