#include <QtTest>
#include <QtAlgorithms>

#include <memory>

#include "../../SystemFunctionality/Utilities/scoreitem.h"

class UtilitiesTests : public QObject
{
    Q_OBJECT

public:
    UtilitiesTests();

private slots:
    void testStatisticsCorrectlyUpdated();

private:
    void _checkCorrectStatistics(const ScoreItem& scoreItem, int guessedWordPairs, int totalWordPairs, int obtainedScore, int totalAvailableScore, const QString& status);
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

    std::unique_ptr<ScoreItem> pScoreItem{new ScoreItem{}};

    pScoreItem->initStatistics();
    _checkCorrectStatistics(*pScoreItem, 0, 0, 0, 0, "Checking initial statistics");

    pScoreItem->setScoreIncrement(Game::Levels::LEVEL_EASY);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    pScoreItem->setScoreIncrement(Game::Levels::LEVEL_MEDIUM);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    pScoreItem->setScoreIncrement(Game::Levels::LEVEL_HARD);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pScoreItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                            "Checking statistics after successively switching levels and running one full and one partial update per level");

    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::RESET);

    _checkCorrectStatistics(*pScoreItem, 0, 0, 0, 0, "Checking statistics after reset");

    pScoreItem->setScoreIncrement(Game::Levels::LEVEL_EASY);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pScoreItem, 1, 2, Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY], 2 * Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY],
                            "Checking statistics after setting level to easy and running a full and partial update");

    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::RESET);

    pScoreItem->setScoreIncrement(Game::Levels::LEVEL_MEDIUM);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pScoreItem, 1, 2, Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM], 2 * Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM],
                            "Checking statistics after reset and then setting level to medium and running a full and partial update");

    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::RESET);

    pScoreItem->setScoreIncrement(Game::Levels::LEVEL_HARD);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::PARTIAL_UPDATE);
    pScoreItem->updateStatistics(Game::StatisticsUpdateTypes::FULL_UPDATE);

    _checkCorrectStatistics(*pScoreItem, 1, 2, Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD], 2 * Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD],
            "Checking statistics after reset and then setting level to hard and running a full and partial update");
}

void UtilitiesTests::_checkCorrectStatistics(const ScoreItem& scoreItem, int guessedWordPairs, int totalWordPairs, int obtainedScore, int totalAvailableScore, const QString &status)
{
    qInfo()<<status;
    QVERIFY2(scoreItem.getGuessedWordPairs() == guessedWordPairs, "Number of guessed word pairs is incorrect");
    QVERIFY2(scoreItem.getTotalWordPairs() == totalWordPairs, "Number of total word pairs is incorrect");
    QVERIFY2(scoreItem.getObtainedScore() == obtainedScore, "Obtained score is incorrect");
    QVERIFY2(scoreItem.getTotalAvailableScore() == totalAvailableScore, "Total available score is incorrect");
}

QTEST_APPLESS_MAIN(UtilitiesTests)

#include "tst_utilitiestests.moc"
