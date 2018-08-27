#include <QString>
#include <QtTest>
#include <QtAlgorithms>
#include <QDebug>

#include "../../Common/wordmixer.h"
#include "../../Common/scoreitem.h"
#include "../../Common/gamestrings.h"

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
    void testWordsAreCorrectlyMixed();
    void testFirstLastPieceIndexesAreCorrect();
    void testStatisticsCorrectlyUpdated();
};

CommonTests::CommonTests()
{
}

void CommonTests::testManualWordsEntry()
{
    const QString firstWord{"firstWord"};
    const QString secondWord{"secondWord"};

    WordMixer wordMixer{GameStrings::c_NoFile};

    wordMixer.setFirstWord(firstWord);
    wordMixer.setSecondWord(secondWord);

    wordMixer.mixWords();

    QVERIFY2(firstWord==wordMixer.getFirstWord() && secondWord==wordMixer.getSecondWord() , "Manual words entry does not work properly");
}

void CommonTests::testWordsAreCorrectlyMixed()
{
    const QString firstWord{"firstWord"};
    const QString secondWord{"secondWord"};

    WordMixer wordMixer{GameStrings::c_NoFile};

    wordMixer.setFirstWord(firstWord);
    wordMixer.setSecondWord(secondWord);

    wordMixer.setWordPieceSize(Game::Level::EASY);
    wordMixer.mixWords();
    _checkCorrectMixing(wordMixer.getMixedWordsStringArray(), QVector<QString>{"fir", "stW", "ord", "sec", "ond", "Wor", "d"}, "easy");

    wordMixer.setWordPieceSize(Game::Level::MEDIUM);
    wordMixer.mixWords();
    _checkCorrectMixing(wordMixer.getMixedWordsStringArray(), QVector<QString>{"fi", "rs", "tW", "or", "d", "se", "co", "nd", "Wo", "rd"}, "medium");

    wordMixer.setWordPieceSize(Game::Level::HARD);
    wordMixer.mixWords();
    _checkCorrectMixing(wordMixer.getMixedWordsStringArray(), QVector<QString>{"f", "i", "r", "s", "t", "W", "o", "r", "d", "s", "e", "c", "o", "n", "d", "W", "o", "r", "d"},
                        "hard");
}

void CommonTests::testFirstLastPieceIndexesAreCorrect()
{
    const QString firstWord{"wordOne"};
    const QString secondWord{"secondWord"};

    const char* firstWordFirstPieceIndexNotCorrect{"Index of the first piece of first word in the mixed words array is not correct"};
    const char* firstWordLastPieceIndexNotCorrect{"Index of the last piece of first word in the mixed words array is not correct"};
    const char* secondWordFirstPieceIndexNotCorrect{"Index of the first piece of second word in the mixed words array is not correct"};
    const char* secondWordLastPieceIndexNotCorrect{"Index of the last piece of second word in the mixed words array is not correct"};

    WordMixer wordMixer{GameStrings::c_NoFile};

    wordMixer.setFirstWord(firstWord);
    wordMixer.setSecondWord(secondWord);

    wordMixer.setWordPieceSize(Game::Level::EASY);
    wordMixer.mixWords();

    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getFirstWordFirstPieceIndex()] == "wor", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getSecondWordFirstPieceIndex()] == "sec", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getSecondWordLastPieceIndex()] == "d", secondWordLastPieceIndexNotCorrect);

    wordMixer.setWordPieceSize(Game::Level::MEDIUM);
    wordMixer.mixWords();

    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getFirstWordFirstPieceIndex()] == "wo", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getSecondWordFirstPieceIndex()] == "se", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getSecondWordLastPieceIndex()] == "rd", secondWordLastPieceIndexNotCorrect);

    wordMixer.setWordPieceSize(Game::Level::HARD);
    wordMixer.mixWords();

    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getFirstWordFirstPieceIndex()] == "w", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getSecondWordFirstPieceIndex()] == "s", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(wordMixer.getMixedWordsStringArray()[wordMixer.getSecondWordLastPieceIndex()] == "d",secondWordLastPieceIndexNotCorrect);
}

void CommonTests::testStatisticsCorrectlyUpdated()
{
    const int referenceGuessedWordPairs = 3;
    const int referenceTotalWordPairs = 6;
    const int referenceObtainedScore = Game::c_ScoreIncrements[Game::Level::EASY] + Game::c_ScoreIncrements[Game::Level::MEDIUM] + Game::c_ScoreIncrements[Game::Level::HARD];
    const int referenceTotalAvailableScore = 2 * referenceObtainedScore;

    ScoreItem scoreItem{};

    _checkCorrectStatistics(scoreItem, 0, 0, 0, 0, "Checking initial statistics");

    scoreItem.setScoreIncrement(Game::Level::EASY);
    scoreItem.updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    scoreItem.updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    scoreItem.setScoreIncrement(Game::Level::MEDIUM);
    scoreItem.updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    scoreItem.updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    scoreItem.setScoreIncrement(Game::Level::HARD);
    scoreItem.updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    scoreItem.updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    _checkCorrectStatistics(scoreItem, referenceGuessedWordPairs, referenceTotalWordPairs, referenceObtainedScore, referenceTotalAvailableScore,
                            "Checking statistics after successively switching levels and running one full and one partial update per level");

    scoreItem.resetStatistics();

    _checkCorrectStatistics(scoreItem, 0, 0, 0, 0, "Checking statistics after reset");

    scoreItem.setScoreIncrement(Game::Level::EASY);
    scoreItem.updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    scoreItem.updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    _checkCorrectStatistics(scoreItem, 1, 2, Game::c_ScoreIncrements[Game::Level::EASY], 2 * Game::c_ScoreIncrements[Game::Level::EASY],
                            "Checking statistics after setting level to easy and running a full and partial update");

    scoreItem.resetStatistics();

    scoreItem.setScoreIncrement(Game::Level::MEDIUM);
    scoreItem.updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    scoreItem.updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    _checkCorrectStatistics(scoreItem, 1, 2, Game::c_ScoreIncrements[Game::Level::MEDIUM], 2 * Game::c_ScoreIncrements[Game::Level::MEDIUM],
                            "Checking statistics after reset and then setting level to medium and running a full and partial update");

    scoreItem.resetStatistics();

    scoreItem.setScoreIncrement(Game::Level::HARD);
    scoreItem.updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);
    scoreItem.updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

    _checkCorrectStatistics(scoreItem, 1, 2, Game::c_ScoreIncrements[Game::Level::HARD], 2 * Game::c_ScoreIncrements[Game::Level::HARD],
                            "Checking statistics after reset and then setting level to hard and running a full and partial update");
}

void CommonTests::_checkCorrectMixing(QVector<QString> mixedWords, QVector<QString> splitWords, const QString& level)
{
    qDebug()<<"Checking correct word mixing, level:"<<level;
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
    qDebug()<<status;
    QVERIFY2(scoreItem.getGuessedWordPairs() == guessedWordPairs, "Number of guessed word pairs is incorrect");
    QVERIFY2(scoreItem.getTotalWordPairs() == totalWordPairs, "Number of total word pairs is incorrect");
    QVERIFY2(scoreItem.getObtainedScore() == obtainedScore, "Obtained score is incorrect");
    QVERIFY2(scoreItem.getTotalAvailableScore() == totalAvailableScore, "Total available score is incorrect");
}

QTEST_APPLESS_MAIN(CommonTests)

#include "tst_commontests.moc"
