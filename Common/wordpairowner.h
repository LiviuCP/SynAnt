#ifndef WORDPAIROWNER_H
#define WORDPAIROWNER_H

#include <QObject>
#include <QVector>

#include "game.h"

class WordMixer;

class WordPairOwner : public QObject
{
    Q_OBJECT
public:
    explicit WordPairOwner(QObject *parent = nullptr);

    void connectToWordMixer(WordMixer* pWordMixer);
    void updateSingleWordPieceSelection(int wordPieceIndex, bool selected);
    void updateMultipleWordPiecesSelection(QVector<int> wordPieceIndexes, bool selected);

    const QVector<Game::WordPiece> getMixedWordsPieces() const;

    QString getFirstWord() const;
    QString getSecondWord() const;

    bool areSynonyms() const;

signals:
    Q_SIGNAL void mixedWordsAvailable();
    Q_SIGNAL void selectionChanged();

private slots:
    void _onMixedWordsAvailable();

private:
    void _buildMixedWordsPieces();

    WordMixer* m_pWordMixer;
    QString m_FirstWord;
    QString m_SecondWord;
    QVector<Game::WordPiece> m_MixedWordsPieces;
    bool m_AreSynonyms;

};

#endif // WORDPAIROWNER_H
