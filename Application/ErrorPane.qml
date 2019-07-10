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
    readonly property double buttonRadiusRatio: 0.025

    MouseArea {
        id: errorPaneMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        text: GameStrings.errorPaneToolTip
        visible: errorPaneMouseArea.containsMouse
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
    }

    Rectangle {
        id: errorPaneRect

        width: parent.width
        height: errorPaneRectHeight

        anchors.top: parent.top

        color: Styles.backgroundColor
        border.color: Styles.borderColor

        Text {
            id: errorPaneText

            text: presenter.errorMessage
            color: Styles.fatalErrorTextColor
            width: parent.width
            wrapMode: Text.WordWrap

            font.pointSize: 12
            minimumPointSize: 8
            fontSizeMode: Text.Fit

            anchors {
                fill: parent
                leftMargin: parent.width * 0.02
                rightMargin: parent.width * 0.02
            }
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            bottom: parent.bottom
            topMargin: parent.height * 0.01
            left: errorPaneRect.left
            right: errorPaneRect.right
        }

        Button {
            id: closeBtn

            Layout.minimumWidth: bottomBtnsMinWidth
            Layout.alignment: Layout.Center

            contentItem: Text {
                text: GameStrings.closeButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.closeButtonToolTip
                visible: closeBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.closeButtonShortcut
                enabled: closeBtn.visible
                onActivated: presenter.quit()
            }

            onClicked: presenter.quit();
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }
}
