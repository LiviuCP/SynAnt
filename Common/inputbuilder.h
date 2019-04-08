#ifndef INPUTBUILDER_H
#define INPUTBUILDER_H

#include <QObject>
#include <QVector>

#include "game.h"

class WordPairOwner;

class InputBuilder : public QObject
{
    Q_OBJECT
public:
    explicit InputBuilder(QObject *parent = nullptr);
    void connectToWordPairOwner(WordPairOwner* pWordPairOwner);
    bool addPieceToFirstWordInput(int index);
    bool addPieceToSecondWordInput(int index);
    const QVector<int> getFirstWordInputIndexes() const;
    const QVector<int> getSecondWordInputIndexes() const;
    QString getFirstInputWord() const;
    QString getSecondInputWord() const;
    bool isInputComplete() const;

signals:
    Q_SIGNAL void inputChanged();
    Q_SIGNAL void completionChanged();

public slots:
    void resetInput();

private:
    enum class WordInputState
    {
        EMPTY,
        BUILD_IN_PROGRESS,
        COMPLETED
    };

    struct WordInput
    {
        WordInput();

        QVector<int> indexes;
        WordInputState state;
    };

    bool _addPieceToWordInput(WordInput& currentWordInput, const WordInput& otherWordInput, int index);
    bool _checkAndUpdateState(WordInput& currentWordInput, const WordInput& otherWordInput, int index);

    WordPairOwner* m_pWordPairOwner;
    WordInput m_FirstWordInput;
    WordInput m_SecondWordInput;
};

#endif // INPUTBUILDER_H
