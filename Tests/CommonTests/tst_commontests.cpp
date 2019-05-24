#include <QString>
#include <QtTest>
#include <QtAlgorithms>

#include <memory>

#include "../../Common/CoreFunctionality/wordmixer.h"
#include "../../Common/Utilities/scoreitem.h"
#include "../../Common/DataAccess/datasource.h"
#include "../../Common/DataAccess/datasourceaccesshelper.h"
#include "../../Common/Utilities/gamestrings.h"

class CommonTests : public QObject
{
    Q_OBJECT

public:
    CommonTests();

private:
    void _checkCorrectMixing(QVector<QString> mixedWords, QVector<QString> splitWords, const QString& level);
    void _checkCorrectStatistics(const ScoreItem& scoreItem, int guessedWordPairs, int totalWordPairs, int obtainedScore, int totalAvailableScore, const QString& status);

private Q_SLOTS:
    void testManualWordsEntry();
    void testWordPairsAreCorrect();
    void testWordsAreCorrectlyMixed();
    void testFirstLastPieceIndexesAreCorrect();
    void testStatisticsCorrectlyUpdated();
    void testDataSourceAccessHelperSetTable();
    void testDataSourceAccessHelperUsedEntries();
    void testDataSourceAccessHelperUseAllEntries();
    void testDataSourceAccessHelperSelfReset();
    void testDataSourceAccessHelperResetUsedEntries();
};

CommonTests::CommonTests()
{
}

void CommonTests::testManualWordsEntry()
{
    const QString firstWord{"firstword"};
    const QString secondWord{"secondword"};

    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{}};

    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);

    QVERIFY2(firstWord==pWordMixer->getFirstWord() && secondWord==pWordMixer->getSecondWord() , "Manual words entry does not work properly");
}

void CommonTests::testWordPairsAreCorrect()
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

void CommonTests::testWordsAreCorrectlyMixed()
{
    const QString firstWord{"firstword"};
    const QString secondWord{"secondword"};

    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{}};

    pWordMixer->setWordPieceSize(Game::Level::EASY);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);
    _checkCorrectMixing(pWordMixer->getMixedWordsPiecesContent(), QVector<QString>{"fir", "stw", "ord", "sec", "ond", "wor", "d"}, "easy");

    pWordMixer->setWordPieceSize(Game::Level::MEDIUM);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);
    _checkCorrectMixing(pWordMixer->getMixedWordsPiecesContent(), QVector<QString>{"fi", "rs", "tw", "or", "d", "se", "co", "nd", "wo", "rd"}, "medium");

    pWordMixer->setWordPieceSize(Game::Level::HARD);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);
    _checkCorrectMixing(pWordMixer->getMixedWordsPiecesContent(), QVector<QString>{"f", "i", "r", "s", "t", "w", "o", "r", "d", "s", "e", "c", "o", "n", "d", "w", "o", "r", "d"},
                        "hard");
}

void CommonTests::testFirstLastPieceIndexesAreCorrect()
{
    const QString firstWord{"wordone"};
    const QString secondWord{"secondword"};

    const char* firstWordFirstPieceIndexNotCorrect{"Index of the first piece of first word in the mixed words array is not correct"};
    const char* firstWordLastPieceIndexNotCorrect{"Index of the last piece of first word in the mixed words array is not correct"};
    const char* secondWordFirstPieceIndexNotCorrect{"Index of the first piece of second word in the mixed words array is not correct"};
    const char* secondWordLastPieceIndexNotCorrect{"Index of the last piece of second word in the mixed words array is not correct"};

    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{}};

    pWordMixer->setWordPieceSize(Game::Level::EASY);
    // for testing purposes we always assume the words are synonyms
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);

    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordFirstPieceIndex()] == "wor", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordFirstPieceIndex()] == "sec", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordLastPieceIndex()] == "d", secondWordLastPieceIndexNotCorrect);

    pWordMixer->setWordPieceSize(Game::Level::MEDIUM);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);

    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordFirstPieceIndex()] == "wo", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordFirstPieceIndex()] == "se", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordLastPieceIndex()] == "rd", secondWordLastPieceIndexNotCorrect);

    pWordMixer->setWordPieceSize(Game::Level::HARD);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);

    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordFirstPieceIndex()] == "w", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordFirstPieceIndex()] == "s", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordLastPieceIndex()] == "d",secondWordLastPieceIndexNotCorrect);
}

void CommonTests::testStatisticsCorrectlyUpdated()
{
    const int referenceGuessedWordPairs = 3;
    const int referenceTotalWordPairs = 6;
    const int referenceObtainedScore = Game::c_ScoreIncrements[Game::Level::EASY] + Game::c_ScoreIncrements[Game::Level::MEDIUM] + Game::c_ScoreIncrements[Game::Level::HARD];
    const int referenceTotalAvailableScore = 2 * referenceObtainedScore;

    std::unique_ptr<ScoreItem> pScoreItem{new ScoreItem{}};

    _checkCorrectStatistics(*pScoreItem, 0, 0, 0, 0, "Checking initial statistics");

    pScoreItem->setScoreIncrement(Game::Level::EASY);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    pScoreItem->setScoreIncrement(Game::Level::MEDIUM);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    pScoreItem->setScoreIncrement(Game::Level::HARD);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    _checkCorrectStatistics(*pScoreItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                            "Checking statistics after successively switching levels and running one full and one partial update per level");

    pScoreItem->resetStatistics();

    _checkCorrectStatistics(*pScoreItem, 0, 0, 0, 0, "Checking statistics after reset");

    pScoreItem->setScoreIncrement(Game::Level::EASY);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    _checkCorrectStatistics(*pScoreItem, 1, 2, Game::c_ScoreIncrements[Game::Level::EASY], 2 * Game::c_ScoreIncrements[Game::Level::EASY],
                            "Checking statistics after setting level to easy and running a full and partial update");

    pScoreItem->resetStatistics();

    pScoreItem->setScoreIncrement(Game::Level::MEDIUM);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    _checkCorrectStatistics(*pScoreItem, 1, 2, Game::c_ScoreIncrements[Game::Level::MEDIUM], 2 * Game::c_ScoreIncrements[Game::Level::MEDIUM],
                            "Checking statistics after reset and then setting level to medium and running a full and partial update");

    pScoreItem->resetStatistics();

    pScoreItem->setScoreIncrement(Game::Level::HARD);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    _checkCorrectStatistics(*pScoreItem, 1, 2, Game::c_ScoreIncrements[Game::Level::HARD], 2 * Game::c_ScoreIncrements[Game::Level::HARD],
            "Checking statistics after reset and then setting level to hard and running a full and partial update");
}

void CommonTests::testDataSourceAccessHelperSetTable()
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

void CommonTests::testDataSourceAccessHelperUsedEntries()
{
    std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};

    pDataSourceAccessHelper->setEntriesTable(5);

    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());

    QVERIFY2(pDataSourceAccessHelper->getNrOfUsedEntries() == 4, "The number of used entries has not been correctly updated!");
}

void CommonTests::testDataSourceAccessHelperUseAllEntries()
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

void CommonTests::testDataSourceAccessHelperSelfReset()
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

void CommonTests::testDataSourceAccessHelperResetUsedEntries()
{
    std::unique_ptr<DataSourceAccessHelper> pDataSourceAccessHelper{new DataSourceAccessHelper{}};

    pDataSourceAccessHelper->setEntriesTable(3);
    Q_UNUSED(pDataSourceAccessHelper->generateEntryNumber());
    pDataSourceAccessHelper->resetUsedEntries();

    QVERIFY2(pDataSourceAccessHelper->getNrOfUsedEntries() == 0, "The number of used entries has not been correctly reset!");
    QVERIFY2(pDataSourceAccessHelper->getTotalNrOfEntries() == 3, "The total number of entries after reset is not correct");
}

void CommonTests::_checkCorrectMixing(QVector<QString> mixedWords, QVector<QString> splitWords, const QString& level)
{
    qInfo() << "Checking correct word mixing, level:" << level;
    if (mixedWords.size() != splitWords.size())
    {
        QVERIFY2(false, "Words incorrectly mixed. Vector size differs from the one of the reference vector");
    }
    else
    {
        std::sort(mixedWords.begin(), mixedWords.end());
        std::sort(splitWords.begin(), splitWords.end());
        QVERIFY2(mixedWords == splitWords, "Words incorrectly mixed. Vector does not contain the exact word pieces");
    }
}

void CommonTests::_checkCorrectStatistics(const ScoreItem& scoreItem, int guessedWordPairs, int totalWordPairs, int obtainedScore, int totalAvailableScore, const QString &status)
{
    qInfo()<<status;
    QVERIFY2(scoreItem.getGuessedWordPairs() == guessedWordPairs, "Number of guessed word pairs is incorrect");
    QVERIFY2(scoreItem.getTotalWordPairs() == totalWordPairs, "Number of total word pairs is incorrect");
    QVERIFY2(scoreItem.getObtainedScore() == obtainedScore, "Obtained score is incorrect");
    QVERIFY2(scoreItem.getTotalAvailableScore() == totalAvailableScore, "Total available score is incorrect");
}

QTEST_APPLESS_MAIN(CommonTests)

#include "tst_commontests.moc"
