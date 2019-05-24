#include "wordmixerproxy.h"
#include "../CoreFunctionality/wordmixer.h"

WordMixerProxy::WordMixerProxy(WordMixer* pWordMixer, QObject *parent)
    : QObject(parent)
    , m_pWordMixer{pWordMixer}
{
}

const QVector<QString>& WordMixerProxy::getMixedWordsPiecesContent() const
{
    return m_pWordMixer->getMixedWordsPiecesContent();
}

QString WordMixerProxy::getFirstWord() const
{
    return m_pWordMixer->getFirstWord();
}

QString WordMixerProxy::getSecondWord() const
{
    return m_pWordMixer->getSecondWord();
}

int WordMixerProxy::getFirstWordFirstPieceIndex() const
{
    return m_pWordMixer->getFirstWordFirstPieceIndex();
}

int WordMixerProxy::getFirstWordLastPieceIndex() const
{
    return m_pWordMixer->getFirstWordLastPieceIndex();
}

int WordMixerProxy::getSecondWordFirstPieceIndex() const
{
    return m_pWordMixer->getSecondWordFirstPieceIndex();
}

int WordMixerProxy::getSecondWordLastPieceIndex() const
{
    return m_pWordMixer->getSecondWordLastPieceIndex();
}

bool WordMixerProxy::areSynonyms() const
{
    return m_pWordMixer->areSynonyms();
}
