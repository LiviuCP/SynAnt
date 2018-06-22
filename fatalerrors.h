                                                                                                    /*

                                                                                                      This class implements the a window which pops up when a fatal error is detected

                                                                                                      The window consists of 2 elements:
                                                                                                      1) Error message
                                                                                                      2) A button named Ok, which when pressed closes the entire application

                                                                                                    */

#ifndef FATALERRORS_H
#define FATALERRORS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class FatalErrors : public QWidget
{
    Q_OBJECT                                                                                        // not required, no custom signals and slots
public:
    explicit FatalErrors(QWidget *parent = 0);                                                      // constructor used for building the window and its elements
    void setFatalErrorText(const QString &text);                                                    // this function is called from outside the class when the exception is encountered
private:
    QString m_ErrorText;                                                                            // error text to be displayed
    QLabel* m_ErrorMessage;                                                                         // error message, shown in the upper part of the window
};

#endif // FATALERRORS_H
