#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <QString>

class GameException : public std::exception
{
public:
    GameException() = delete;
    GameException(QString description);
    const char* what() const noexcept override;
private:
    QString m_Description;
};

class FileException : public GameException
{
public:
    FileException() = delete;
    FileException(QString description, QString fileName);
    const char* what() const noexcept override;
private:
    QString m_FileName;
};

class WordException : public FileException
{
public:
    WordException() = delete;
    WordException(QString description, QString fileName, int rowNumber);
    const char* what() const noexcept override;
private:
    int m_RowNumber;
};

#endif // EXCEPTIONS_H
