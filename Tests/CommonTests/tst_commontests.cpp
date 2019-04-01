#include <QString>
#include <QtTest>
#include <QtAlgorithms>

#include <memory>

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
    void testWordPairsAreCorrect();
    void testWordsAreCorrectlyMixed();
    void testFirstLastPieceIndexesAreCorrect();
    void testStatisticsCorrectlyUpdated();
};

CommonTests::CommonTests()
{
}

void CommonTests::testManualWordsEntry()
{
    const QString firstWord{"firstword"};
    const QString secondWord{"secondword"};

    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{GameStrings::c_NoFile}};

    pWordMixer->setFirstWord(firstWord);
    pWordMixer->setSecondWord(secondWord);

    pWordMixer->mixWords();

    QVERIFY2(firstWord==pWordMixer->getFirstWord() && secondWord==pWordMixer->getSecondWord() , "Manual words entry does not work properly");
}

void CommonTests::testWordPairsAreCorrect()
{
    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{GameStrings::c_NoFile}};

    // empty row
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent(""), std::exception);

    // illegal chars (including capitals)
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("firstWord!secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("firstword!second&word"), std::exception);

    // no separator
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("firstwordsecondword"), std::exception);

    // multiple separators
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("firs!tword!secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("firs!tword=secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("firs=tword!secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("firs=tword=secondword"), std::exception);

    // less than minimum required number of chars per word
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("abcd!secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("abcd=secondword"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("firstword=efgh"), std::exception);
    QVERIFY_EXCEPTION_THROWN(pWordMixer->fetchWordsFromRowContent("firstword!efgh"), std::exception);
}

void CommonTests::testWordsAreCorrectlyMixed()
{
    const QString firstWord{"firstword"};
    const QString secondWord{"secondword"};

    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{GameStrings::c_NoFile}};

    pWordMixer->setFirstWord(firstWord);
    pWordMixer->setSecondWord(secondWord);

    pWordMixer->setWordPieceSize(Game::Level::EASY);
    pWordMixer->mixWords();
    _checkCorrectMixing(pWordMixer->getMixedWordsPiecesArray(), QVector<QString>{"fir", "stw", "ord", "sec", "ond", "wor", "d"}, "easy");

    pWordMixer->setWordPieceSize(Game::Level::MEDIUM);
    pWordMixer->mixWords();
    _checkCorrectMixing(pWordMixer->getMixedWordsPiecesArray(), QVector<QString>{"fi", "rs", "tw", "or", "d", "se", "co", "nd", "wo", "rd"}, "medium");

    pWordMixer->setWordPieceSize(Game::Level::HARD);
    pWordMixer->mixWords();
    _checkCorrectMixing(pWordMixer->getMixedWordsPiecesArray(), QVector<QString>{"f", "i", "r", "s", "t", "w", "o", "r", "d", "s", "e", "c", "o", "n", "d", "w", "o", "r", "d"},
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

    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{GameStrings::c_NoFile}};

    pWordMixer->setFirstWord(firstWord);
    pWordMixer->setSecondWord(secondWord);

    pWordMixer->setWordPieceSize(Game::Level::EASY);
    pWordMixer->mixWords();

    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getFirstWordFirstPieceIndex()] == "wor", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getSecondWordFirstPieceIndex()] == "sec", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getSecondWordLastPieceIndex()] == "d", secondWordLastPieceIndexNotCorrect);

    pWordMixer->setWordPieceSize(Game::Level::MEDIUM);
    pWordMixer->mixWords();

    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getFirstWordFirstPieceIndex()] == "wo", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getSecondWordFirstPieceIndex()] == "se", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getSecondWordLastPieceIndex()] == "rd", secondWordLastPieceIndexNotCorrect);

    pWordMixer->setWordPieceSize(Game::Level::HARD);
    pWordMixer->mixWords();

    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getFirstWordFirstPieceIndex()] == "w", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getSecondWordFirstPieceIndex()] == "s", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesArray()[pWordMixer->getSecondWordLastPieceIndex()] == "d",secondWordLastPieceIndexNotCorrect);
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
