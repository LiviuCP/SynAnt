#include <QString>
#include <QtTest>
#include <QtAlgorithms>
#include <QDebug>

#include "../../Application/wordmixer.h"
#include "../../Application/gamestrings.h"

class CommonTests : public QObject
{
    Q_OBJECT

public:
    CommonTests();

private:
    void _checkCorrectMixing(QVector<QString> mixedWords, QVector<QString> splitWords, const QString& level);

private Q_SLOTS:
    void testManualWordsEntry();
    void testWordsAreCorrectlyMixed();
    void testFirstLastPieceIndexesAreCorrect();
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

QTEST_APPLESS_MAIN(CommonTests)

#include "tst_commontests.moc"
