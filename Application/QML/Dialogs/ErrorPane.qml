import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameUtils 1.0
import Controls 1.0

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

        AppButton {
            id: closeBtn

            dedicatedShortcutEnabled: presenter.errorOccured

            Layout.minimumWidth: bottomBtnsMinWidth
            Layout.alignment: Layout.Center

            buttonLabel: GameStrings.closeButtonLabel
            buttonToolTip: GameStrings.closeButtonToolTip
            shortcutSequence: GameStrings.closeButtonShortcut

            onButtonClicked: presenter.quit()
        }
    }
}
