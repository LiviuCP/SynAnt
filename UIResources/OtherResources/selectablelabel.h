#ifndef SELECTABLELABEL_H
#define SELECTABLELABEL_H

#include <QLabel>

class SelectableLabel : public QLabel
{
Q_OBJECT
public:
    SelectableLabel();
    void setLabelSelectedStyleSheet(const QString& styleSheet);
    void setLabelNotSelectedStyleSheet(const QString& styleSheet);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;

private slots:
    void _onClicked();

private:
    bool m_IsSelected;
    QString m_LabelSelectedStyle;
    QString m_LabelNotSelectedStyle;
};

#endif // SELECTABLELABEL_H
