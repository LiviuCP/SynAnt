/*
   This class fulfills following tasks:
   1) Connects to a raw data file (a.k.a. database).
   2) Reads the raw data, processes it to word pair entries and stores each obtained entry into a processed data array (a.k.a. active array).
   3) Based on a received valid entry number it hands over the requested data entry to the consumer class (WordMixer).
   4) Validates the new data entries (provided by user through data entry page), converts them into raw data and stores them into the database
   5) Adds the new (valid) data entries to active array so they are immediately used in the game
*/

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QVector>

class DataSource : public QObject
{
    Q_OBJECT
public:
    struct DataEntry
    {
        DataEntry();
        DataEntry(const QString& firstWord, const QString& secondWord, bool areSynonyms);

        QString firstWord;
        QString secondWord;
        bool areSynonyms;
    };

    explicit DataSource(const QString& fileName, QObject *parent = nullptr);

    void updateDataEntries(QVector<DataEntry> dataEntries, bool append);
    void provideDataEntryToConsumer(int entryNumber);
    int getNrOfValidEntries() const;
    QString getDataFilePath() const;

public slots:
    void onWriteDataToDbRequested(QPair<QString, QString> newWordsPair, bool areSynonyms);

signals:
    Q_SIGNAL void writeDataToDbFinished(bool success);
    Q_SIGNAL void writeDataToDbErrorOccured();
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);
    Q_SIGNAL void entriesUpdated();

private:
    bool _createRawDataEntry(QString& rawDataEntry, const QString& firstWord, const QString& secondWord, bool areSynonyms);
    bool _entryAlreadyExists(const DataEntry& dataEntry);

    QString m_DataFilePath;
    QVector<DataEntry> m_DataEntries;

};

#endif // DATASOURCE_H
