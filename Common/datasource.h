/*
   This class fulfills following tasks:
   1) Connects to a raw data file.
   2) Reads the raw data, processes it to word pair entries and stores each obtained entry into a processed data array.
   3) Based on a received valid entry number it hands over the requested data entry to the consumer class (WordMixer)
*/

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QVector>

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(const QString& fileName, QObject *parent = nullptr);

    void init();
    void fetchDataEntry(int entryNumber);
    int getNrOfEntries();

    // for test purposes only
    void processRawDataEntryForTest(const QString& rawDataEntry);

signals:
    Q_SIGNAL void dataReady();
    Q_SIGNAL void entryFetched(QPair<QString, QString> newWordsPair, bool areSynonyms);

private:
    struct DataEntry
    {
        DataEntry();

        QString firstWord;
        QString secondWord;
        bool areSynonyms;
    };

    void _loadRawData(QVector<QString>& rawData);
    void _createProcessedDataEntries(const QVector<QString>& rawData);
    DataEntry _createProcessedDataEntry(const QString& rawDataEntry, int rowNumber);

    QString m_DataFilePath;
    QVector<DataEntry> m_DataEntries;

};

#endif // DATASOURCE_H
