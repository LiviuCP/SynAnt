#include <QString>
#include <QtTest>
#include <QtAlgorithms>

#include "../../Application/wordmixer.h"
#include "../../Application/gamestrings.h"

class CommonTests : public QObject
{
    Q_OBJECT

public:
    CommonTests();

private:
    void _checkCorrectMixing(QVector<QString> mixedWords, QVector<QString> splitWords);

private Q_SLOTS:
    void testManualWordsEntry();
    void testWordsAreCorrectlyMixed();
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
    _checkCorrectMixing(wordMixer.getMixedWordsStringArray(), QVector<QString>{"fir", "stW", "ord", "sec", "ond", "Wor", "d"});

    wordMixer.setWordPieceSize(Game::Level::MEDIUM);
    wordMixer.mixWords();
    _checkCorrectMixing(wordMixer.getMixedWordsStringArray(), QVector<QString>{"fi", "rs", "tW", "or", "d", "se", "co", "nd", "Wo", "rd"});

    wordMixer.setWordPieceSize(Game::Level::HARD);
    wordMixer.mixWords();
    _checkCorrectMixing(wordMixer.getMixedWordsStringArray(), QVector<QString>{"f", "i", "r", "s", "t", "W", "o", "r", "d", "s", "e", "c", "o", "n", "d", "W", "o", "r", "d"});
}

void CommonTests::_checkCorrectMixing(QVector<QString> mixedWords, QVector<QString> splitWords)
{
    if (mixedWords.size() != splitWords.size())
    {
        QVERIFY2(false, "Words incorrectly mixed. Vector size differs from the one of the reference vector");
    }
    else
    {
        qSort(mixedWords.begin(), mixedWords.end());
        qSort(splitWords.begin(), splitWords.end());
        QVERIFY2(mixedWords == splitWords, "Words incorrectly mixed. Vector does not contain the exact word pieces");
    }
}

QTEST_APPLESS_MAIN(CommonTests)

#include "tst_commontests.moc"
