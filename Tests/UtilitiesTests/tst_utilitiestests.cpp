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
    void _doFullStatisticsUpdateCheck(std::unique_ptr<StatisticsItem>& pStatisticsItem, const int referenceGuessedWordPairs, const int referenceTotalWordPairs, const int referenceObtainedScore,
                                      const int referenceTotalAvailableScore, const QMap<Game::Levels, int> referenceScoreIncrements);
    void _checkCorrectStatistics(const StatisticsItem& statisticsItem, const int referenceGuessedWordPairs, const int referenceTotalWordPairs, const int referenceObtainedScore, const int referenceTotalAvailableScore, const QString& status);
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
    {
        const int c_ReferenceGuessedWordPairs{3};
        const int c_ReferenceTotalWordPairs{6};
        const int c_ReferenceObtainedScore{Game::c_ScoreIncrements[Game::Levels::LEVEL_EASY] + Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM] + Game::c_ScoreIncrements[Game::Levels::LEVEL_HARD]};
        const int c_ReferenceTotalAvailableScore{2 * c_ReferenceObtainedScore};

        std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};

        qInfo("Checking statistics update with default standard score increments");
        _doFullStatisticsUpdateCheck(pStatisticsItem, c_ReferenceGuessedWordPairs, c_ReferenceTotalWordPairs, c_ReferenceObtainedScore, c_ReferenceTotalAvailableScore, Game::c_ScoreIncrements);
    }

    {
        const int c_ReferenceGuessedWordPairs{3};
        const int c_ReferenceTotalWordPairs{6};
        const int c_ReferenceObtainedScore{Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_EASY] + Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_MEDIUM] + Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_HARD]};
        const int c_ReferenceTotalAvailableScore{2 * c_ReferenceObtainedScore};

        std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};
        pStatisticsItem->setEnhancedIncrement(true);

        qInfo("Checking statistics update with default enhanced score increments");
        _doFullStatisticsUpdateCheck(pStatisticsItem, c_ReferenceGuessedWordPairs, c_ReferenceTotalWordPairs, c_ReferenceObtainedScore, c_ReferenceTotalAvailableScore, Game::c_EnhancedScoreIncrements);
    }

    {
        QMap<Game::Levels, int> scoreIncrements{};
        scoreIncrements[Game::Levels::LEVEL_EASY] = 10;
        scoreIncrements[Game::Levels::LEVEL_MEDIUM] = 12;
        scoreIncrements[Game::Levels::LEVEL_HARD] = 15;

        const int c_ReferenceGuessedWordPairs{3};
        const int c_ReferenceTotalWordPairs{6};
        const int c_ReferenceObtainedScore{scoreIncrements[Game::Levels::LEVEL_EASY] + scoreIncrements[Game::Levels::LEVEL_MEDIUM] + scoreIncrements[Game::Levels::LEVEL_HARD]};
        const int c_ReferenceTotalAvailableScore{2 * c_ReferenceObtainedScore};

        std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};
        pStatisticsItem->setIncrementForLevel(scoreIncrements[Game::Levels::LEVEL_EASY], Game::Levels::LEVEL_EASY);
        pStatisticsItem->setIncrementForLevel(scoreIncrements[Game::Levels::LEVEL_MEDIUM], Game::Levels::LEVEL_MEDIUM);
        pStatisticsItem->setIncrementForLevel(scoreIncrements[Game::Levels::LEVEL_HARD], Game::Levels::LEVEL_HARD);

        qInfo("Checking statistics update with custom standard score increments");
        _doFullStatisticsUpdateCheck(pStatisticsItem, c_ReferenceGuessedWordPairs, c_ReferenceTotalWordPairs, c_ReferenceObtainedScore, c_ReferenceTotalAvailableScore, scoreIncrements);
    }

    {
        QMap<Game::Levels, int> scoreIncrements{};
        scoreIncrements[Game::Levels::LEVEL_EASY] = 20;
        scoreIncrements[Game::Levels::LEVEL_MEDIUM] = 22;
        scoreIncrements[Game::Levels::LEVEL_HARD] = 25;

        const int c_ReferenceGuessedWordPairs{3};
        const int c_ReferenceTotalWordPairs{6};
        const int c_ReferenceObtainedScore{scoreIncrements[Game::Levels::LEVEL_EASY] + scoreIncrements[Game::Levels::LEVEL_MEDIUM] + scoreIncrements[Game::Levels::LEVEL_HARD]};
        const int c_ReferenceTotalAvailableScore{2 * c_ReferenceObtainedScore};

        std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};
        pStatisticsItem->setIncrementForLevel(scoreIncrements[Game::Levels::LEVEL_EASY], Game::Levels::LEVEL_EASY, true);
        pStatisticsItem->setIncrementForLevel(scoreIncrements[Game::Levels::LEVEL_MEDIUM], Game::Levels::LEVEL_MEDIUM, true);
        pStatisticsItem->setIncrementForLevel(scoreIncrements[Game::Levels::LEVEL_HARD], Game::Levels::LEVEL_HARD, true);
        pStatisticsItem->setEnhancedIncrement(true);

        qInfo("Checking statistics update with custom enhanced score increments");
        _doFullStatisticsUpdateCheck(pStatisticsItem, c_ReferenceGuessedWordPairs, c_ReferenceTotalWordPairs, c_ReferenceObtainedScore, c_ReferenceTotalAvailableScore, scoreIncrements);
    }

    {
        int referenceGuessedWordPairs{0};
        int referenceTotalWordPairs{0};
        int referenceObtainedScore{0};
        int referenceTotalAvailableScore{0};

        std::unique_ptr<StatisticsItem> pStatisticsItem{new StatisticsItem{}};

        qInfo("Checking statistics update mixed custom/default standard/enhanced score increments");

        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
        pStatisticsItem->doInitialUpdate();
        _checkCorrectStatistics(*pStatisticsItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                                "Checking statistics after initial update");

        ++referenceTotalWordPairs;
        referenceTotalAvailableScore += Game::c_ScoreIncrements[Game::Levels::LEVEL_MEDIUM];
        pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
        _checkCorrectStatistics(*pStatisticsItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                                "Checking statistics partial update, level medium, default, standard increment");

        pStatisticsItem->setEnhancedIncrement(true);
        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_HARD);
        ++referenceGuessedWordPairs;
        ++referenceTotalWordPairs;
        referenceObtainedScore += Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_HARD];
        referenceTotalAvailableScore += Game::c_EnhancedScoreIncrements[Game::Levels::LEVEL_HARD];
        pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::FULL_UPDATE);
        _checkCorrectStatistics(*pStatisticsItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                                "Checking statistics full update, level hard, default, enhanced increment");

        const int c_LevelEasyEnhancedCustomIncrement{7};
        pStatisticsItem->setIncrementForLevel(c_LevelEasyEnhancedCustomIncrement, Game::Levels::LEVEL_EASY, true);
        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_EASY);
        ++referenceTotalWordPairs;
        referenceTotalAvailableScore += c_LevelEasyEnhancedCustomIncrement;
        pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
        _checkCorrectStatistics(*pStatisticsItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                                "Checking statistics partial update, level easy, custom, enhanced increment");

        const int c_LevelMediumCustomIncrement{12};
        pStatisticsItem->setIncrementForLevel(c_LevelMediumCustomIncrement, Game::Levels::LEVEL_MEDIUM);
        pStatisticsItem->setEnhancedIncrement(false);
        pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
        ++referenceGuessedWordPairs;
        ++referenceTotalWordPairs;
        referenceObtainedScore += c_LevelMediumCustomIncrement;
        referenceTotalAvailableScore += c_LevelMediumCustomIncrement;
        pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::FULL_UPDATE);
        _checkCorrectStatistics(*pStatisticsItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                                "Checking statistics full update, level medium, custom, standard increment");

        pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::RESET);
        referenceGuessedWordPairs = 0;
        referenceTotalWordPairs = 0;
        referenceObtainedScore = 0;
        referenceTotalAvailableScore = 0;
        _checkCorrectStatistics(*pStatisticsItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                                "Checking statistics reset");
    }
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

void UtilitiesTests::_doFullStatisticsUpdateCheck(std::unique_ptr<StatisticsItem> &pStatisticsItem, const int referenceGuessedWordPairs, const int referenceTotalWordPairs, const int referenceObtainedScore,
                                                  const int referenceTotalAvailableScore, const QMap<Game::Levels, int> referenceScoreIncrements)
{
    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
    pStatisticsItem->doInitialUpdate();
    _checkCorrectStatistics(*pStatisticsItem, 0, 0, 0, 0, "Checking initial statistics");

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_EASY);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::FULL_UPDATE);

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::FULL_UPDATE);

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_HARD);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                            "Checking statistics after successively switching levels and running one full and one partial update per level");

    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::RESET);

    _checkCorrectStatistics(*pStatisticsItem, 0, 0, 0, 0, "Checking statistics after reset");

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_EASY);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, 1, 2, referenceScoreIncrements[Game::Levels::LEVEL_EASY], 2 * referenceScoreIncrements[Game::Levels::LEVEL_EASY],
                            "Checking statistics after setting level to easy and running a full and partial update");

    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::RESET);

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_MEDIUM);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, 1, 2, referenceScoreIncrements[Game::Levels::LEVEL_MEDIUM], 2 * referenceScoreIncrements[Game::Levels::LEVEL_MEDIUM],
                            "Checking statistics after reset and then setting level to medium and running a full and partial update");

    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::RESET);

    pStatisticsItem->setGameLevel(Game::Levels::LEVEL_HARD);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::PARTIAL_UPDATE);
    pStatisticsItem->updateStatistics(StatisticsItem::StatisticsUpdateOperations::FULL_UPDATE);

    _checkCorrectStatistics(*pStatisticsItem, 1, 2, referenceScoreIncrements[Game::Levels::LEVEL_HARD], 2 * referenceScoreIncrements[Game::Levels::LEVEL_HARD],
            "Checking statistics after reset and then setting level to hard and running a full and partial update");
}

void UtilitiesTests::_checkCorrectStatistics(const StatisticsItem& statisticsItem, const int referenceGuessedWordPairs, const int referenceTotalWordPairs, const int referenceObtainedScore,
                                             const int referenceTotalAvailableScore, const QString &status)
{
    qInfo()<<status;
    QVERIFY2(statisticsItem.getGuessedWordPairs() == QString::number(referenceGuessedWordPairs), "Number of guessed word pairs is incorrect");
    QVERIFY2(statisticsItem.getTotalWordPairs() == QString::number(referenceTotalWordPairs), "Number of total word pairs is incorrect");
    QVERIFY2(statisticsItem.getObtainedScore() == QString::number(referenceObtainedScore), "Obtained score is incorrect");
    QVERIFY2(statisticsItem.getTotalAvailableScore() == QString::number(referenceTotalAvailableScore), "Total available score is incorrect");
}

QTEST_APPLESS_MAIN(UtilitiesTests)

#include "tst_utilitiestests.moc"
