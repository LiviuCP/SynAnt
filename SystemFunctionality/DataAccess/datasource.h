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

    enum class UpdateOperation
    {
        LOAD_TO_PRIMARY,
        LOAD_TO_SECONDARY,
        SWAP,
        APPEND,
    };

    explicit DataSource(const QString& dataBasePath, QObject *parent = nullptr);

    void updateDataEntries(QVector<DataEntry> dataEntries, bool append); // remove this method after implementing multiple languages in data entry
    void updateDataEntries(const QVector<DataEntry>& dataEntries, int languageIndex, DataSource::UpdateOperation updateOperation = DataSource::UpdateOperation::LOAD_TO_PRIMARY);
    void provideDataEntryToConsumer(int entryNumber);

    int getPrimarySourceLanguageIndex() const;
    int getSecondarySourceLanguageIndex() const;
    int getPrimarySourceNrOfEntries() const;
    int getSecondarySourceNrOfEntries() const;

    QString getDataFilePath() const;
    bool entryAlreadyExists(const DataEntry& dataEntry); // remove this method after implementing multiple languages in data entry
    bool entryAlreadyExists(const DataEntry& dataEntry, int languageIndex);

signals:
    Q_SIGNAL void entryProvidedToConsumer(QPair<QString, QString> newWordsPair, bool areSynonyms);

private:
    struct Source
    {
        Source();

        int languageIndex;
        QVector<DataEntry> entries;
    };

    QString m_DataBasePath;
    QVector<DataEntry> m_DataEntries;
    Source m_PrimarySource;
    Source m_SecondarySource;
    mutable QMutex m_DataSourceMutex;
};

Q_DECLARE_METATYPE(DataSource::DataEntry)

#endif // DATASOURCE_H
