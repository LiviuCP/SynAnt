#include <QtTest>
#include <QVector>

#include <memory>

#include "../../SystemFunctionality/DataAccess/datasource.h"
#include "../../SystemFunctionality/DataAccess/dataentryvalidator.h"
#include "../../SystemFunctionality/DataAccess/dataentrycache.h"

class DataEntryTests : public QObject
{
    Q_OBJECT

public:
    DataEntryTests();
    ~DataEntryTests();

private slots:
    void testEnteredWordsAreInvalid();
    void testEnteredWordsAreValid();
    void testAddingWordPairsToCache();
};

DataEntryTests::DataEntryTests()
{
}

DataEntryTests::~DataEntryTests()
{

}

void DataEntryTests::testEnteredWordsAreInvalid()
{
    std::unique_ptr<DataSource> pDataSource{new DataSource{}};
    std::unique_ptr<DataEntryValidator> pDataEntryValidator{new DataEntryValidator{pDataSource.get()}};

    QVector<DataSource::DataEntry> entriesLoadedToFirstLanguage{DataSource::DataEntry{"languagewordone", "languagewordtwo", true}};
    QVector<DataSource::DataEntry> entriesLoadedToSecondLanguage{DataSource::DataEntry{"langwordthree", "langwordfour", true}};

    pDataSource->updateDataEntries(entriesLoadedToFirstLanguage, 0, DataSource::UpdateOperation::LOAD_TO_PRIMARY);
    pDataSource->updateDataEntries(entriesLoadedToSecondLanguage, 1, DataSource::UpdateOperation::LOAD_TO_SECONDARY);

    // less than minimum characters per word
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcd", "abcdefghijk", true, 0), "Less than minimum characters for first word has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefghijk", "abcd", true, 0), "Less than minimum characters for second word has not been detected!");
    // less than minimum characters per pair
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "abcdef", true, 0), "Less than minimum total pair characters has not been detected!");
    // more than maximum characters per pair
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefghijklmnop", "abcdefghijklmno", true, 0), "More than maximum total pair characters has not been detected!");
    // invalid characters
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "abCdefg", true, 0), "The invalid character (upper case) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abCdefg", "abcdefgh", true, 0), "The invalid character (upper case) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "Abcdefg", true, 0), "The invalid character (upper case) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("Abcdefg", "abcdefgh", true, 0), "The invalid character (upper case) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("ab1defgh", "abcdefg", true, 0), "The invalid character (number) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefg", "ab1defgh", true, 0), "The invalid character (number) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("1bcdefgh", "abcdefg", true, 0), "The invalid character (number) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefg", "1bcdefgh", true, 0), "The invalid character (number) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "abcde!g", true, 0), "The invalid character (character !) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcde!g", "abcdefgh", true, 0), "The invalid character (character !) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("!bcdefg", "abcdefgh", true, 0), "The invalid character (character !) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "!bcdefg", true, 0), "The invalid character (character !) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcde gh", "abcdefg", true, 0), "The invalid character (space) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefg", "abcde gh", true, 0), "The invalid character (space) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid(" bcdefgh", "abcdefg", true, 0), "The invalid character (space) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefg", " bcdefgh", true, 0), "The invalid character (space) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "abcd-fg", true, 0), "The invalid character (character -) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcd-fg", "abcdefgh", true, 0), "The invalid character (character -) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "-bcdefg", true, 0), "The invalid character (character -) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("-bcdefg", "abcdefgh", true, 0), "The invalid character (character -) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "abcd_fg", true, 0), "The invalid character (underscore) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcd_fg", "abcdefgh", true, 0), "The invalid character (underscore) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "_bcdefg", true, 0), "The invalid character (underscore) has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("_bcdefg", "abcdefgh", true, 0), "The invalid character (underscore) has not been detected!");
    // identical words
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "abcdefgh", true, 0), "The identical words have not been detected!");
    // already existing pair in data source (in the chosen language)
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("languagewordone", "languagewordtwo", true, 0), "Already existing pair in datasource for the given language has not been detected!");
    QVERIFY2(!pDataEntryValidator->isGivenWordsPairValid("langwordfour", "langwordthree", true, 1), "Already existing pair in datasource for the given language has not been detected!");
}

void DataEntryTests::testEnteredWordsAreValid()
{
    std::unique_ptr<DataSource> pDataSource{new DataSource{}};
    std::unique_ptr<DataEntryValidator> pDataEntryValidator{new DataEntryValidator{pDataSource.get()}};

    QVector<DataSource::DataEntry> entriesLoadedToFirstLanguage{DataSource::DataEntry{"languagewordone", "languagewordtwo", true}};
    QVector<DataSource::DataEntry> entriesLoadedToSecondLanguage{DataSource::DataEntry{"langwordthree", "langwordfour", true}};

    pDataSource->updateDataEntries(entriesLoadedToFirstLanguage, 0, DataSource::UpdateOperation::LOAD_TO_PRIMARY);
    pDataSource->updateDataEntries(entriesLoadedToSecondLanguage, 1, DataSource::UpdateOperation::LOAD_TO_SECONDARY);

    QVERIFY2(pDataEntryValidator->isGivenWordsPairValid("abcdefgh", "abcdefg", true, 2), "Invalid words pair incorrectly detected!");
    QVERIFY2(pDataEntryValidator->isGivenWordsPairValid("languagewordone", "languagewordtwo", true, 1), "Invalid words pair incorrectly detected!");
    QVERIFY2(pDataEntryValidator->isGivenWordsPairValid("languagewordtwo", "languagewordone", true, 2), "Invalid words pair incorrectly detected!");
    QVERIFY2(pDataEntryValidator->isGivenWordsPairValid("langwordfour", "langwordthree", true, 0), "Invalid words pair incorrectly detected!");
    QVERIFY2(pDataEntryValidator->isGivenWordsPairValid("langwordthree", "langwordfour", true, 2), "Invalid words pair incorrectly detected!");
}

void DataEntryTests::testAddingWordPairsToCache()
{
    std::unique_ptr<DataSource> pDataSource{new DataSource{}};
    std::unique_ptr<DataEntryCache> pDataEntryCache{new DataEntryCache{pDataSource.get(), ""}};

    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 0, "The data entry cache has not been correctly initialized!");

    pDataEntryCache->onValidEntryReceived(DataSource::DataEntry{"languagewordone", "languagewordtwo", true}, 0);
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 1, "The data entry cache has not been correctly updated!");

    pDataEntryCache->onValidEntryReceived(DataSource::DataEntry{"langwordthree", "langwordfour", true}, 1);
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 2, "The data entry cache has not been correctly updated!");

    pDataEntryCache->onValidEntryReceived(DataSource::DataEntry{"languagewordtwo", "languagewordone", true}, 0);
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 2, "The data entry cache has not been correctly updated!");

    pDataEntryCache->onValidEntryReceived(DataSource::DataEntry{"langwordthree", "langwordfour", true}, 0);
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 3, "The data entry cache has not been correctly updated!");

    pDataEntryCache->onValidEntryReceived(DataSource::DataEntry{"langwordthree", "langwordfour", true}, 1);
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 3, "The data entry cache has not been correctly updated!");

    pDataEntryCache->onValidEntryReceived(DataSource::DataEntry{"langwordfive", "langwordsix", true}, 0);
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 4, "The data entry cache has not been correctly updated!");

    pDataEntryCache->onValidEntryReceived(DataSource::DataEntry{"languagewordone", "languagewordtwo", true}, 1);
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 5, "The data entry cache has not been correctly updated!");

    pDataEntryCache->onValidEntryReceived(DataSource::DataEntry{"languagewordone", "languagewordtwo", true}, 2);
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 6, "The data entry cache has not been correctly updated!");

    pDataEntryCache->onValidEntryReceived(DataSource::DataEntry{"langwordfour", "langwordthree", true}, 3);
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 7, "The data entry cache has not been correctly updated!");

    pDataEntryCache->onResetCacheRequested();
    QVERIFY2(pDataEntryCache->getNrOfCachedEntries() == 0, "The data entry cache has not been correctly reset!");
}

QTEST_APPLESS_MAIN(DataEntryTests)

#include "tst_dataentrytests.moc"
