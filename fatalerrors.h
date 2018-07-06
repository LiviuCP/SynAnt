/*

  This class implements the a window which pops up when a fatal error is detected

  The window consists of 2 elements:
  1) Error message
  2) A button named Close, which when pressed closes the entire application

*/

#ifndef FATALERRORS_H
#define FATALERRORS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class FatalErrors : public QWidget
{
    Q_OBJECT
public:
    explicit FatalErrors(QWidget *parent = 0);
    void setFatalErrorText(const QString &text);
private:
    QString m_ErrorText;
    QLabel* m_ErrorMessage;
};

#endif // FATALERRORS_H
