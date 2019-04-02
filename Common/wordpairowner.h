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

    const QVector<Game::WordPiece> getMixedWordsPieces() const;

    QString getFirstWord() const;
    QString getSecondWord() const;

    int getFirstWordFirstPieceIndex() const;
    int getFirstWordLastPieceIndex() const;
    int getSecondWordFirstPieceIndex() const;
    int getSecondWordLastPieceIndex() const;

    bool areSynonyms() const;

signals:
    Q_SIGNAL void mixedWordsAvailable();

private slots:
    void _onMixedWordsAvailable();

private:
    void _buildMixedWordsPieces();

    WordMixer* m_pWordMixer;
    QString m_FirstWord;
    QString m_SecondWord;
    QVector<Game::WordPiece> m_MixedWordsPieces;
    int m_FirstWordFirstPieceIndex;
    int m_FirstWordLastPieceIndex;
    int m_SecondWordFirstPieceIndex;
    int m_SecondWordLastPieceIndex;
    bool m_AreSynonyms;

};

#endif // WORDPAIROWNER_H
