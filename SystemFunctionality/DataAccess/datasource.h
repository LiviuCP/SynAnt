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

    explicit DataSource(QObject *parent = nullptr);

    void updateDataEntries(const QVector<DataEntry>& dataEntries, int languageIndex, DataSource::UpdateOperation updateOperation = DataSource::UpdateOperation::LOAD_TO_PRIMARY);
    void provideDataEntryToConsumer(int entryNumber);

    int getPrimarySourceLanguageIndex() const;
    int getSecondarySourceLanguageIndex() const;
    int getPrimarySourceNrOfEntries() const;
    int getSecondarySourceNrOfEntries() const;

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

    Source m_PrimarySource;
    Source m_SecondarySource;
    mutable QMutex m_DataSourceMutex;
};

Q_DECLARE_METATYPE(DataSource::DataEntry)

#endif // DATASOURCE_H
