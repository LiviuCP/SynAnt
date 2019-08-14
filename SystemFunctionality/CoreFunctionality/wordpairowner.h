/*
   This class gets the output from the WordMixer class:
      - mixed words array
      - original words used for mixing
      - syn/ant flag

   It becomes the owner of the mixed words (pieces and reference words) and fulfills following tasks:
   1) Provides the owned data to the "consumer" classes (facade, presenter) on demand
   2) Keeps track of the selection status of the mixed words pieces (when a piece is added to input it is marked as selected and viceversa)
   3) Holds the persistent index which is used for adding pieces to input by using the keyboard cursor
*/

#ifndef WORDPAIROWNER_H
#define WORDPAIROWNER_H

#include <QObject>
#include <QVector>

#include "../Utilities/game.h"

class WordMixerProxy;

class WordPairOwner : public QObject
{
    Q_OBJECT
public:
    explicit WordPairOwner(QObject *parent = nullptr);

    void setNewWordsPair(const QVector<QString>& content, const QString& firstWord, const QString& secondWord, bool areSynonyms,
                         int firstWordFirstPieceIndex, int firstWordLastPieceIndex, int secondWordFirstPieceIndex, int secondWordLastPieceIndex);

    void markPieceAsAddedToInput(int wordPieceIndex);
    void markPiecesAsRemovedFromInput(QVector<int> wordPieceIndexes);

    void setPersistentPieceSelectionIndex(bool isStartPieceRequired);
    void clearPersistentPieceSelectionIndex();
    void increasePersistentPieceSelectionIndex();
    void decreasePersistentPieceSelectionIndex();

    QVector<QString> getMixedWordsPiecesContent() const;
    QVector<Game::PieceTypes> getMixedWordsPiecesTypes() const;
    QVector<bool> getAreMixedWordsPiecesAddedToInput() const;
    Game::PieceTypes getWordPieceType(int index) const;
    bool getIsWordPieceAddedToInput(int index) const;

    int getPersistentPieceSelectionIndex() const;

    QString getFirstReferenceWord() const;
    QString getSecondReferenceWord() const;

    bool isOnePieceLeftToAddToInput() const;
    bool areSynonyms() const;

signals:
    Q_SIGNAL void newWordsPairSetup();
    Q_SIGNAL void piecesAddedToInputStateChanged();
    Q_SIGNAL void persistentIndexChanged();

private:
    void _buildMixedWordsPiecesArray(const QVector<QString>& content, int firstBeginIndex, int firstEndIndex, int secondBeginIndex, int secondEndIndex);
    void _updateSingleWordPieceStatus(int wordPieceIndex, bool addedToInput);
    void _updateMultipleWordPiecesStatus(QVector<int> wordPieceIndexes, bool addedToInput);
    void _updatePersistentPieceSelectionIndex();

    struct WordPiece
    {
        WordPiece();

        QString content;
        Game::PieceTypes pieceType;
        bool isAddedToInput;
    };

    QString m_FirstReferenceWord;
    QString m_SecondReferenceWord;
    QVector<WordPiece> m_MixedWordsPieces;
    bool m_AreSynonyms;
    int m_PersistentPieceSelectionIndex;
};

#endif // WORDPAIROWNER_H
