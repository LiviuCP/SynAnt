#include "exceptions.h"
#include "gamestrings.h"

GameException::GameException(QString description)
    : exception{}
    , m_Description{description}
{
}

const char* GameException::what() const noexcept
{
    QString exceptionMessage{m_Description};
    return exceptionMessage.toStdString().c_str();
}

FileException::FileException(QString description, QString fileName)
    : GameException{description}
    , m_FileName{fileName}
{
}

const char* FileException::what() const noexcept
{
    QString exceptionMessage{GameException::what() + GameStrings::c_FileNameMessage.arg(m_FileName)};
    return exceptionMessage.toStdString().c_str();
}

WordException::WordException(QString description, QString fileName, int rowNumber)
    : FileException{description, fileName}
    , m_RowNumber{rowNumber}
{
}

const char* WordException::what() const noexcept
{
    QString exceptionMessage{FileException::what() + GameStrings::c_RowNumberMessage.arg(m_RowNumber)};
    return exceptionMessage.toStdString().c_str();
}


