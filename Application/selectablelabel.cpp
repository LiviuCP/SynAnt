#include "selectablelabel.h"
#include "styles.h"

static const QString c_DefaultLabelNotSelectedStyle    {"color: white; border-width:1px; background-color: grey;" };
static const QString c_DefaultLabelSelectedStyle       {"color: white; border-width:1px; background-color: black;"};

SelectableLabel::SelectableLabel()
    : QLabel{}
    , m_IsSelected{false}
    , m_LabelSelectedStyle{c_DefaultLabelSelectedStyle}
    , m_LabelNotSelectedStyle{c_DefaultLabelNotSelectedStyle}
{
    setStyleSheet(m_LabelNotSelectedStyle);

    bool connected{connect(this,&SelectableLabel::clicked,this,&SelectableLabel::_onClicked)};
    Q_ASSERT(connected);
}

void SelectableLabel::setLabelSelectedStyleSheet(const QString &styleSheet)
{
    if (m_LabelSelectedStyle != styleSheet)
    {
        m_LabelSelectedStyle = styleSheet;
        if (m_IsSelected)
        {
            setStyleSheet(m_LabelSelectedStyle);
        }
    }
}

void SelectableLabel::setLabelNotSelectedStyleSheet(const QString &styleSheet)
{
    if (m_LabelNotSelectedStyle != styleSheet)
    {
        m_LabelNotSelectedStyle = styleSheet;
        if (!m_IsSelected)
        {
            setStyleSheet(m_LabelNotSelectedStyle);
        }
    }
}

void SelectableLabel::mousePressEvent(QMouseEvent *mouseEvent)
{
    (void) mouseEvent;
    emit clicked();
}

void SelectableLabel::_onClicked()
{
    m_IsSelected = !m_IsSelected;

    if (m_IsSelected)
    {
        setStyleSheet(m_LabelSelectedStyle);
    }
    else
    {
        setStyleSheet(m_LabelNotSelectedStyle);
    }
}
