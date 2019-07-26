/*
   This class fulfills following tasks:
   1) Stores the valid entries from the game database so they are immediately available per user request
   2) Based on a received valid entry number it hands over the requested data entry to the consumer class (WordMixer).
*/

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QVector>
#include <QMutex>

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

        bool operator==(const DataEntry& dataEntry) const;
    };

    explicit DataSource(const QString& dataBasePath, QObject *parent = nullptr);

    void updateDataEntries(QVector<DataEntry> dataEntries, bool append);
    void provideDataEntryToConsumer(int entryNumber);
    int getNrOfValidEntries() const;
    QString getDataFilePath() const;
    bool entryAlreadyExists(const DataEntry& dataEntry);

signals:
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);

private:
    QString m_DataBasePath;
    QVector<DataEntry> m_DataEntries;
    mutable QMutex m_DataSourceMutex;
};

Q_DECLARE_METATYPE(DataSource::DataEntry)

#endif // DATASOURCE_H
