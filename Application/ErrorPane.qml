import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameUtils 1.0

Item {
    id: errorPane

    property QtObject presenter

    readonly property double errorPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1

    readonly property double bottomBtnsMinWidth: errorPaneRect.width / 3

    MouseArea {
        id: errorPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: errorPaneToolTip
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
        text: GameStrings.errorPaneToolTip
        visible: errorPaneMouseArea.containsMouse
    }

    Rectangle {
        id: errorPaneRect
        anchors.top: parent.top
        width: parent.width
        height: errorPaneRectHeight
        color: Styles.backgroundColor

        border {
            color: Styles.borderColor
        }

        Text {
            id: errorPaneText
            text: presenter.errorMessage
            color: Styles.fatalErrorTextColor

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
                text: GameStrings.closeButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth
            Layout.alignment: Layout.Center

            ToolTip.text: GameStrings.closeButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: GameStrings.closeButtonShortcut
                onActivated: {
                    Qt.quit();
                }
            }

            onClicked: Qt.quit();
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }
}
