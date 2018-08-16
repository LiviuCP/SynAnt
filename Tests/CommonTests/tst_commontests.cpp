#include <QString>
#include <QtTest>

#include "../../Application/wordmixer.h"
#include "../../Application/gamestrings.h"

class CommonTests : public QObject
{
    Q_OBJECT

public:
    CommonTests();

private Q_SLOTS:
    void testManualWordsEntry();
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

QTEST_APPLESS_MAIN(CommonTests)

#include "tst_commontests.moc"
