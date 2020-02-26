#include <QtTest>

#include <memory>

#include "../../SystemFunctionality/CoreFunctionality/wordmixer.h"

class CoreFunctionalityTests : public QObject
{
    Q_OBJECT

public:
    CoreFunctionalityTests();

private slots:
    void testManualWordsEntry();
    void testWordsAreCorrectlyMixed();
    void testFirstLastPieceIndexesAreCorrect();

private:
    void _checkCorrectMixing(QVector<QString> mixedWords, QVector<QString> splitWords, const QString& level);
};

CoreFunctionalityTests::CoreFunctionalityTests()
{
}

void CoreFunctionalityTests::testManualWordsEntry()
{
    const QString firstWord{"firstword"};
    const QString secondWord{"secondword"};

    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{}};

    pWordMixer->setGameLevel(Game::Levels::LEVEL_MEDIUM);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);

    QVERIFY2(firstWord==pWordMixer->getFirstWord() && secondWord==pWordMixer->getSecondWord() , "Manual words entry does not work properly");
}

void CoreFunctionalityTests::testWordsAreCorrectlyMixed()
{
    const QString firstWord{"firstword"};
    const QString secondWord{"secondword"};

    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{}};

    pWordMixer->setGameLevel(Game::Levels::LEVEL_EASY);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);
    _checkCorrectMixing(pWordMixer->getMixedWordsPiecesContent(), QVector<QString>{"fir", "stw", "ord", "sec", "ond", "wor", "d"}, "easy");

    pWordMixer->setGameLevel(Game::Levels::LEVEL_MEDIUM);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);
    _checkCorrectMixing(pWordMixer->getMixedWordsPiecesContent(), QVector<QString>{"fi", "rs", "tw", "or", "d", "se", "co", "nd", "wo", "rd"}, "medium");

    pWordMixer->setGameLevel(Game::Levels::LEVEL_HARD);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);
    _checkCorrectMixing(pWordMixer->getMixedWordsPiecesContent(), QVector<QString>{"f", "i", "r", "s", "t", "w", "o", "r", "d", "s", "e", "c", "o", "n", "d", "w", "o", "r", "d"},
                        "hard");
}

void CoreFunctionalityTests::testFirstLastPieceIndexesAreCorrect()
{
    const QString firstWord{"wordone"};
    const QString secondWord{"secondword"};

    const char* firstWordFirstPieceIndexNotCorrect{"Index of the first piece of first word in the mixed words array is not correct"};
    const char* firstWordLastPieceIndexNotCorrect{"Index of the last piece of first word in the mixed words array is not correct"};
    const char* secondWordFirstPieceIndexNotCorrect{"Index of the first piece of second word in the mixed words array is not correct"};
    const char* secondWordLastPieceIndexNotCorrect{"Index of the last piece of second word in the mixed words array is not correct"};

    std::unique_ptr<WordMixer> pWordMixer{new WordMixer{}};

    pWordMixer->setGameLevel(Game::Levels::LEVEL_EASY);
    // for testing purposes we always assume the words are synonyms
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);

    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordFirstPieceIndex()] == "wor", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordFirstPieceIndex()] == "sec", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordLastPieceIndex()] == "d", secondWordLastPieceIndexNotCorrect);

    pWordMixer->setGameLevel(Game::Levels::LEVEL_MEDIUM);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);

    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordFirstPieceIndex()] == "wo", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordFirstPieceIndex()] == "se", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordLastPieceIndex()] == "rd", secondWordLastPieceIndexNotCorrect);

    pWordMixer->setGameLevel(Game::Levels::LEVEL_HARD);
    pWordMixer->mixWords(QPair<QString, QString>{firstWord, secondWord}, true);

    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordFirstPieceIndex()] == "w", firstWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getFirstWordLastPieceIndex()] == "e", firstWordLastPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordFirstPieceIndex()] == "s", secondWordFirstPieceIndexNotCorrect);
    QVERIFY2(pWordMixer->getMixedWordsPiecesContent()[pWordMixer->getSecondWordLastPieceIndex()] == "d",secondWordLastPieceIndexNotCorrect);
}

void CoreFunctionalityTests::_checkCorrectMixing(QVector<QString> mixedWords, QVector<QString> splitWords, const QString &level)
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

QTEST_APPLESS_MAIN(CoreFunctionalityTests)

#include "tst_corefunctionalitytests.moc"
