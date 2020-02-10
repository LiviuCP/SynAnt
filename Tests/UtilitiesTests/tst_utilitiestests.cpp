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
    void testStatisticsCorrectlyUpdated();

private:
    void _checkCorrectStatistics(const StatisticsItem& statisticsItem, int guessedWordPairs, int totalWordPairs, int obtainedScore, int totalAvailableScore, const QString& status);
};

UtilitiesTests::UtilitiesTests()
{
}

void UtilitiesTests::testStatisticsCorrectlyUpdated()
{
    const int referenceGuessedWordPairs = 3;
    const int referenceTotalWordPairs = 6;
    const int referenceObtainedScore = Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY] + Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM] + Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD];
    const int referenceTotalAvailableScore = 2 * referenceObtainedScore;

    std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};

    pStatisticsItem->initStatistics();
    _checkCorrectStatistics(*pStatisticsItem, 0, 0, 0, 0, "Checking initial statistics");

    pStatisticsItem->setScoreIncrement(Game::Levels::LEVEL_EASY);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    pStatisticsItem->setScoreIncrement(Game::Levels::LEVEL_MEDIUM);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    pStatisticsItem->setScoreIncrement(Game::Levels::LEVEL_HARD);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                            "Checking statistics after successively switching levels and running one full and one partial update per level");

    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::RESET);

    _checkCorrectStatistics(*pStatisticsItem, 0, 0, 0, 0, "Checking statistics after reset");

    pStatisticsItem->setScoreIncrement(Game::Levels::LEVEL_EASY);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, 1, 2, Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY], 2 * Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY],
                            "Checking statistics after setting level to easy and running a full and partial update");

    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::RESET);

    pStatisticsItem->setScoreIncrement(Game::Levels::LEVEL_MEDIUM);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, 1, 2, Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM], 2 * Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM],
                            "Checking statistics after reset and then setting level to medium and running a full and partial update");

    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::RESET);

    pStatisticsItem->setScoreIncrement(Game::Levels::LEVEL_HARD);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, 1, 2, Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD], 2 * Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD],
            "Checking statistics after reset and then setting level to hard and running a full and partial update");
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
