import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: errorPane

    property QtObject presenter

    readonly property double errorPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1

    readonly property double bottomBtnsMinWidth: errorPaneRect.width / 3

    readonly property color paneColor: "grey"
    readonly property color borderColor: "white"
    readonly property color textColor: "red"

    MouseArea {
        id: errorPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: errorPaneToolTip
        delay: presenter.toolTipDelay
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
            color: textColor

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
            text: presenter.closeButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth
            Layout.alignment: Layout.Center

            ToolTip.text: presenter.closeButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            Shortcut {
                sequence: presenter.closeButtonShortcut
                onActivated: {
                    Qt.quit();
                }
            }

            onClicked: {
                Qt.quit();
            }
        }
    }
}
