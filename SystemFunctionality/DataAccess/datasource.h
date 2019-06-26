/*
   This class fulfills following tasks:
   1) Connects to a raw data file.
   2) Reads the raw data, processes it to word pair entries and stores each obtained entry into a processed data array.
   3) Based on a received valid entry number it hands over the requested data entry to the consumer class (WordMixer).
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

    void fetchDataEntry(int entryNumber);
    int getNrOfEntries();

    // for test purposes only
    bool processRawDataEntryForTest(const QString& rawDataEntry);

public slots:
    void onReadDataRequestReceived();
    void onWriteDataRequestReceived(QPair<QString, QString> newWordsPair, bool areSynonyms);

signals:
    Q_SIGNAL void readDataFinished(bool success);
    Q_SIGNAL void entryFetched(QPair<QString, QString> newWordsPair, bool areSynonyms);
    Q_SIGNAL void writeDataFinished(bool success);
    Q_SIGNAL void dataEntrySaveError();

private:
    struct DataEntry
    {
        DataEntry();
        DataEntry(const QString& firstWord, const QString& secondWord, bool areSynonyms);

        QString firstWord;
        QString secondWord;
        bool areSynonyms;
    };

    bool _loadRawData(QVector<QString>& rawData);
    void _createProcessedDataEntries(const QVector<QString>& rawData);
    bool _createProcessedDataEntry(DataEntry& dataEntry, const QString& rawDataEntry);
    bool _createRawDataEntry(QString& rawDataEntry, const QString& firstWord, const QString& secondWord, bool areSynonyms);

    QString m_DataFilePath;
    QVector<DataEntry> m_DataEntries;

};

#endif // DATASOURCE_H
