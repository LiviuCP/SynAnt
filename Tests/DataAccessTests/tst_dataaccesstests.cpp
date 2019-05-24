#include <QtTest>

#include <memory>

#include "../../Common/DataAccess/datasource.h"
#include "../../Common/DataAccess/datasourceaccesshelper.h"
#include "../../Common/Utilities/gamestrings.h"

class DataAccessTests : public QObject
{
    Q_OBJECT

public:
    DataAccessTests();
    ~DataAccessTests();

private slots:
    void testWordPairsAreCorrect();
    void testDataSourceAccessHelperSetTable();
    void testDataSourceAccessHelperUsedEntries();
    void testDataSourceAccessHelperUseAllEntries();
    void testDataSourceAccessHelperSelfReset();
    void testDataSourceAccessHelperResetUsedEntries();
};

DataAccessTests::DataAccessTests()
{

}

DataAccessTests::~DataAccessTests()
{

}

void DataAccessTests::testWordPairsAreCorrect()
{
    std::unique_ptr<DataSource> pDataSource{new DataSource{GameStrings::c_NoFile}};

    // empty row
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest(""), std::exception);

    // illegal chars (including capitals)
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("firstWord!secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("firstword!second&word"), std::exception);

    // no separator
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("firstwordsecondword"), std::exception);

    // multiple separators
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("firs!tword!secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("firs!tword=secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("firs=tword!secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("firs=tword=secondword"), std::exception);

    // less than minimum required number of chars per word
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("abcd!secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("abcd=secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("firstword=efgh"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("firstword!efgh"), std::exception);

    // less than minimum required total number of chars per pair
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("abcdefg=hijklmn"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("abcdefg!hijklmn"), std::exception);

    // more than maximum allowed total number of chars per pair
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("abcdefghijklmnop=onmlkjihgfedcba"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("abcdefghijklmnop!onmlkjihgfedcba"), std::exception);

    // same words in pair
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("abcdefghij=abcdefghij"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pDataSource->processRawDataEntryForTest("abcdefghij!abcdefghij"), std::exception);
}

void DataAccessTests::testDataSourceAccessHelperSetTable()
{
    auto setAndChangeNumberOfEntries = [](DataSourceAccessHelper& dataSourceAccessHelper, int firstSetupNrOfEntries, int secondSetupNrOfEntries)
    {
        dataSourceAccessHelper.setEntriesTable(firstSetupNrOfEntries);
        Q_UNUSED(dataSourceAccessHelper.generateEntryNumber());
        dataSourceAccessHelper.setEntriesTable(secondSetupNrOfEntries);
    };

    {
        std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};
        pDataSourceAccessHelper->setEntriesTable(4);

        QVERIFY2(pDataSourceAccessHelper->getNrOfUsedEntries() == 0, "The number of used entries is not correct!");
        QVERIFY2(pDataSourceAccessHelper->getTotalNrOfEntries() == 4, "The total number of entries is not correct!");
    }

    {
        std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};
        setAndChangeNumberOfEntries(*pDataSourceAccessHelper, 5, 5);

        QVERIFY2(pDataSourceAccessHelper->getNrOfUsedEntries() == 0, "The number of used entries after second setup is not correct!");
        QVERIFY2(pDataSourceAccessHelper->getTotalNrOfEntries() == 5, "The total number of entries after second setup is not correct!");
    }

    {
        std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};
        setAndChangeNumberOfEntries(*pDataSourceAccessHelper, 5, 4);

        QVERIFY2(pDataSourceAccessHelper->getNrOfUsedEntries() == 0, "The number of used entries after second setup is not correct!");
        QVERIFY2(pDataSourceAccessHelper->getTotalNrOfEntries() == 4, "The total number of entries after second setup is not correct!");
    }

    {
        std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};
        setAndChangeNumberOfEntries(*pDataSourceAccessHelper, 5, 6);

        QVERIFY2(pDataSourceAccessHelper->getNrOfUsedEntries() == 0, "The number of used entries after second setup is not correct!");
        QVERIFY2(pDataSourceAccessHelper->getTotalNrOfEntries() == 6, "The total number of entries after second setup is not correct!");
    }
}

void DataAccessTests::testDataSourceAccessHelperUsedEntries()
{
    std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};

    pDataSourceAccessHelper->setEntriesTable(5);

    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());

    QVERIFY2(pDataSourceAccessHelper->getNrOfUsedEntries() == 4, "The number of used entries has not been correctly updated!");
}

void DataAccessTests::testDataSourceAccessHelperUseAllEntries()
{
    std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};

    const int nrOfEntries{10};
    QVector<int> usedEntryNumbers;

    pDataSourceAccessHelper->setEntriesTable(nrOfEntries);

    for (int entry{0}; entry < nrOfEntries; ++entry)
    {
        usedEntryNumbers.append(pDataSourceAccessHelper->generateEntryNumber());
    }

    std::sort(usedEntryNumbers.begin(), usedEntryNumbers.end());

    bool allEntriesUsed{true};
    for (int entry{0}; entry < nrOfEntries; ++entry)
    {
        allEntriesUsed = allEntriesUsed && (usedEntryNumbers[entry] == entry);
    }

    QVERIFY2(allEntriesUsed, "Not all entries have been used. Duplicate entry numbers have been generated");
}

void DataAccessTests::testDataSourceAccessHelperSelfReset()
{
    std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};

    pDataSourceAccessHelper->setEntriesTable(3);

    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());

    QVERIFY2(pDataSourceAccessHelper->getNrOfUsedEntries() == 1, "The used number of entries after self-reset is not correct");
    QVERIFY2(pDataSourceAccessHelper->getTotalNrOfEntries() == 3, "The total number of entries after self-reset is not correct");
}

void DataAccessTests::testDataSourceAccessHelperResetUsedEntries()
{
    std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};

    pDataSourceAccessHelper->setEntriesTable(3);
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    pDataSourceAccessHelper->resetUsedEntries();

    QVERIFY2(pDataSourceAccessHelper->getNrOfUsedEntries() == 0, "The number of used entries has not been correctly reset!");
    QVERIFY2(pDataSourceAccessHelper->getTotalNrOfEntries() == 3, "The total number of entries after reset is not correct");
}

QTEST_APPLESS_MAIN(DataAccessTests)

#include "tst_dataaccesstests.moc"
