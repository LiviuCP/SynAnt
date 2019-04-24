/*
   This class creates the user input for each of the two words:
   1) uses the pieces provided by word pair owner and assembles them into input words
   2) enforces proper rules for input creation
   3) provides the built input to the facade for checking against the reference words obtained from the WordPairOwner class
*/

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

    bool addPieceToInputWord(Game::InputWordNumber inputWordNumber, int index);
    void removePiecesFromInputWord(Game::InputWordNumber inputWordNumber, int rangeStart);
    bool clearInput();

    const QVector<int> getFirstWordInputIndexes() const;
    const QVector<int> getSecondWordInputIndexes() const;

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

    bool _addPieceToInputWord(WordInput& currentWordInput, const WordInput& otherWordInput, int pieceIndex);
    bool _checkAndUpdateState(WordInput& currentWordInput, const WordInput& otherWordInput, int pieceIndex);
    void _removePiecesFromWordInput(WordInput& currentWordInput, const WordInput& otherWordInput, int rangeStart);

    WordPairOwner* m_pWordPairOwner;
    WordInput m_FirstWordInput;
    WordInput m_SecondWordInput;
};

#endif // INPUTBUILDER_H
