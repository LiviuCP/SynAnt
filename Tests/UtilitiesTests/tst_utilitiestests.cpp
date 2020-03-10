#include <QtTest>
#include <QtAlgorithms>

#include <memory>

#include "../../SystemFunctionality/Utilities/statisticsitem.h"

class UtilitiesTests : public QObject
{
    Q_OBJECT

public:
    UtilitiesTests();

private slots:
    void testLevelCorrectlySetup();
    void testStatisticsCorrectlyUpdated();
    void testSetScoreIncrementForLevel();

private:
    void _checkCorrectStatistics(const StatisticsItem& statisticsItem, int guessedWordPairs, int totalWordPairs, int obtainedScore, int totalAvailableScore, const QString& status);
};

UtilitiesTests::UtilitiesTests()
{
}

void UtilitiesTests::testLevelCorrectlySetup()
{
    {
        std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};
        QVERIFY2(pStatisticsItem->getGameLevel() == Game::Levels::LEVEL_NONE, "The level has not been correctly setup for the statistics item");
        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_EASY);
        QVERIFY2(pStatisticsItem->getGameLevel() == Game::Levels::LEVEL_EASY, "The level has not been correctly setup for the statistics item");
        QVERIFY2(pStatisticsItem->getCurrentIncrement() == Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY], "Incorrect default score increment for level easy");
        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
        QVERIFY2(pStatisticsItem->getGameLevel() == Game::Levels::LEVEL_MEDIUM, "The level has not been correctly setup for the statistics item");
        QVERIFY2(pStatisticsItem->getCurrentIncrement() == Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM], "Incorrect default score increment for level medium");
        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_HARD);
        QVERIFY2(pStatisticsItem->getGameLevel() == Game::Levels::LEVEL_HARD, "The level has not been correctly setup for the statistics item");
        QVERIFY2(pStatisticsItem->getCurrentIncrement() == Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD], "Incorrect default score increment for level hard");
    }

    {
        std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};
        pStatisticsItem->setEnhancedIncrement(true);
        QVERIFY2(pStatisticsItem->getGameLevel() == Game::Levels::LEVEL_NONE, "The level has not been correctly setup for the statistics item");
        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_EASY);
        QVERIFY2(pStatisticsItem->getGameLevel() == Game::Levels::LEVEL_EASY, "The level has not been correctly setup for the statistics item");
        QVERIFY2(pStatisticsItem->getCurrentIncrement() == Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_EASY], "Incorrect default enhanced score increment for level easy");
        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
        QVERIFY2(pStatisticsItem->getGameLevel() == Game::Levels::LEVEL_MEDIUM, "The level has not been correctly setup for the statistics item");
        QVERIFY2(pStatisticsItem->getCurrentIncrement() == Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_MEDIUM], "Incorrect default enhanced score increment for level medium");
        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_HARD);
        QVERIFY2(pStatisticsItem->getGameLevel() == Game::Levels::LEVEL_HARD, "The level has not been correctly setup for the statistics item");
        QVERIFY2(pStatisticsItem->getCurrentIncrement() == Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_HARD], "Incorrect default enhanced score increment for level hard");
    }
}

void UtilitiesTests::testStatisticsCorrectlyUpdated()
{
    const int referenceGuessedWordPairs = 3;
    const int referenceTotalWordPairs = 6;
    const int referenceObtainedScore = Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY] + Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM] + Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD];
    const int referenceTotalAvailableScore = 2 * referenceObtainedScore;

    std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
    pStatisticsItem->doInitialUpdate();
    _checkCorrectStatistics(*pStatisticsItem, 0, 0, 0, 0, "Checking initial statistics");

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_EASY);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_HARD);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                            "Checking statistics after successively switching levels and running one full and one partial update per level");

    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::RESET);

    _checkCorrectStatistics(*pStatisticsItem, 0, 0, 0, 0, "Checking statistics after reset");

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_EASY);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, 1, 2, Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY], 2 * Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY],
                            "Checking statistics after setting level to easy and running a full and partial update");

    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::RESET);

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, 1, 2, Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM], 2 * Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM],
                            "Checking statistics after reset and then setting level to medium and running a full and partial update");

    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::RESET);

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_HARD);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, 1, 2, Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD], 2 * Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD],
            "Checking statistics after reset and then setting level to hard and running a full and partial update");
}

void UtilitiesTests::testSetScoreIncrementForLevel()
{
    std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};
    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);

    pStatisticsItem->setIncrementForLevel(10, Game::Levels::LEVEL_HARD);
    pStatisticsItem->setIncrementForLevel(20, Game::Levels::LEVEL_HARD, true);

    QVERIFY2(pStatisticsItem->getCurrentIncrement() == Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM], "Incorrect default score increment for current level");

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_HARD);

    QVERIFY2(pStatisticsItem->getCurrentIncrement() == 10, "The increment has not been correctly setup for the new level");

    pStatisticsItem->setEnhancedIncrement(true);

    QVERIFY2(pStatisticsItem->getCurrentIncrement() == 20, "The enhanced increment has not been correctly setup for the current level");
}

void UtilitiesTests::_checkCorrectStatistics(const StatisticsItem& statisticsItem, int guessedWordPairs, int totalWordPairs, int obtainedScore, int totalAvailableScore, const QString &status)
{
    qInfo()<<status;
    QVERIFY2(statisticsItem.getGuessedWordPairs() == QString::number(guessedWordPairs), "Number of guessed word pairs is incorrect");
    QVERIFY2(statisticsItem.getTotalWordPairs() == QString::number(totalWordPairs), "Number of total word pairs is incorrect");
    QVERIFY2(statisticsItem.getObtainedScore() == QString::number(obtainedScore), "Obtained score is incorrect");
    QVERIFY2(statisticsItem.getTotalAvailableScore() == QString::number(totalAvailableScore), "Total available score is incorrect");
}

QTEST_APPLESS_MAIN(UtilitiesTests)

#include "tst_utilitiestests.moc"
