import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameUtils 1.0
import Controls 1.0

Item {
    id: errorPane

    property QtObject presenter

    readonly property double errorPaneStatusBoxHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: errorPaneStatusBox.width / 3
    readonly property double buttonRadiusRatio: 0.025

    StatusBox {
        id: errorPaneStatusBox

        isErrorStatus: true

        width: parent.width
        height: errorPaneStatusBoxHeight

        anchors.top: parent.top

        statusMessage: presenter.errorMessage
        boxToolTip: GameStrings.errorPaneToolTip
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            bottom: parent.bottom
            topMargin: parent.height * 0.01
            left: errorPaneStatusBox.left
            right: errorPaneStatusBox.right
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
