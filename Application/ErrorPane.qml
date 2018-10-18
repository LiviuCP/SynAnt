import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: errorPane

    property QtObject presenter

    readonly property double errorPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1

    readonly property double bottomBtnsMinWidth: errorPaneRect.width / 3

    readonly property double pressedButtonOpacity: 0.5
    readonly property double releasedButtonOpacity: 1.0

    readonly property color paneColor: presenter.backgroundColor
    readonly property color buttonColor: presenter.pushButtonColor
    readonly property color borderColor: presenter.borderColor
    readonly property color textColor: presenter.textColor
    readonly property color fatalErrorTextColor: presenter.fatalErrorTextColor

    MouseArea {
        id: errorPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: errorPaneToolTip
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
        text: presenter.errorPaneToolTip
        visible: errorPaneMouseArea.containsMouse
    }

    Rectangle {
        id: errorPaneRect
        anchors.top: parent.top
        width: parent.width
        height: errorPaneRectHeight
        color: paneColor

        border {
            color: borderColor
        }

        Text {
            id: errorPaneText
            text: presenter.errorMessage
            color: fatalErrorTextColor

            font.pointSize: 12
            minimumPointSize: 8
            fontSizeMode: Text.Fit

            anchors.fill: parent
            anchors.leftMargin: parent.width * 0.02
            anchors.rightMargin: parent.width * 0.02

            width: parent.width
            wrapMode: Text.WordWrap
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors.bottom: parent.bottom
        anchors.topMargin: parent.height * 0.01
        anchors.left: errorPaneRect.left
        anchors.right: errorPaneRect.right

        height: bottomBtnsLayoutHeight

        Button {
            id: closeBtn

            contentItem: Text {
                text: presenter.closeButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth
            Layout.alignment: Layout.Center

            ToolTip.text: presenter.closeButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: presenter.closeButtonShortcut
                onActivated: {
                    Qt.quit();
                }
            }

            onClicked: Qt.quit();
            onPressed: opacity = pressedButtonOpacity
            onReleased: opacity = releasedButtonOpacity
            onCanceled: opacity = releasedButtonOpacity
        }
    }
}
