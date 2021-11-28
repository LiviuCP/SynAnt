#include "exceptions.h"
#include "gameutils.h"

const QString c_FileExceptionRawMessage{"\n%1\n\nFile: %2\n"};

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
    return
    {
        c_FileExceptionRawMessage
                .arg(GameException::getDescription())
                .arg(m_FileName)
    };
}

