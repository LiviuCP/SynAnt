#include "exceptions.h"
#include "gamestrings.h"

GameException::GameException(QString description)
    : exception{}
    , m_Description{description}
{
}

const QString GameException::getDescription() const noexcept
{
    return m_Description;
}

FileException::FileException(QString description, QString fileName)
    : GameException{description}
    , m_FileName{fileName}
{
}

const QString FileException::getDescription() const noexcept
{
    return {GameException::getDescription() + GameStrings::c_FileNameMessage.arg(m_FileName)};
}

