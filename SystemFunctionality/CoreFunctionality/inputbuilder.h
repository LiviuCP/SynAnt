/*
   This class creates the user input for each of the two words:
   1) uses the pieces provided by word pair owner and assembles them into input words
   2) enforces proper rules for input creation
   3) provides the built input to the facade for checking against the reference words obtained from the WordPairOwner class
   4) holds the persistent indexes which are used for removing pieces from input by using the keyboard cursor
*/

#ifndef INPUTBUILDER_H
#define INPUTBUILDER_H

#include <QObject>
#include <QVector>

#include "../Utilities/gameutils.h"

class InputBuilder : public QObject
{
    Q_OBJECT
public:
    explicit InputBuilder(QObject *parent = nullptr);

    bool addPieceToInputWord(Game::InputWordNumber inputWordNumber, int index, Game::PieceTypes pieceType);
    bool removePiecesFromInputWord(Game::InputWordNumber inputWordNumber, int rangeStart);

    // used for erasing input when the user requests this (mixed words pair stays the same)
    bool clearInput();

    // used for erasing the old input when a new word pair is mixed
    void resetInput();

    void setPersistentPiecesRemovalIndex(Game::InputWordNumber inputWordNumber);
    void clearPersistentPiecesRemovalIndexes();
    void increasePersistentPiecesRemovalIndex();
    void decreasePersistentPiecesRemovalIndex();

    // only to be called from "outside" (e.g. from facade) so an active close approval is provided
    void setCloseInputAllowed(bool allowed);

    const QVector<int> getFirstWordInputIndexes() const;
    const QVector<int> getSecondWordInputIndexes() const;

    int getFirstWordPersistentPiecesRemovalIndex() const;
    int getSecondWordPersistentPiecesRemovalIndex() const;

    bool isEmptyInput() const;
    bool isHalfInput() const;
    bool isOneWordLeftInProgress() const;
    bool isInputComplete() const;

signals:
    Q_SIGNAL void pieceAddedToInput(int index);
    Q_SIGNAL void piecesRemovedFromInput(QVector<int> indexes);
    Q_SIGNAL void inputCompletionChanged();
    Q_SIGNAL void persistentIndexesChanged();
    Q_SIGNAL void inputReset();

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
        int persistentPiecesRemovalIndex;
    };

    bool _addPieceToInputWord(WordInput& currentWordInput, const WordInput& otherWordInput, int pieceIndex, Game::PieceTypes pieceType);
    bool _checkAndUpdateState(WordInput& currentWordInput, const WordInput& otherWordInput, Game::PieceTypes pieceType);
    bool _removePiecesFromWordInput(WordInput& currentWordInput, const WordInput& otherWordInput, int rangeStart);

    WordInput m_FirstWordInput;
    WordInput m_SecondWordInput;
    bool m_IsCloseInputAllowed;
};

#endif // INPUTBUILDER_H
