import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0

Item {
    id: helpPane

    property QtObject presenter

    property bool quitButtonShortcutActivated: false

    readonly property double helpPaneStatusBoxHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (helpPaneStatusBox.width - bottomBtnsLayout.spacing) * 0.5

    StatusBox {
        id: helpPaneStatusBox

        width: parent.width
        height: helpPaneStatusBoxHeight

        anchors.top: parent.top

        statusMessage: presenter.helpPaneMessage
        boxToolTip: presenter.helpPaneToolTip
        hasFlickable: true
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors {
            top: helpPaneStatusBox.bottom
            topMargin: parent.height * 0.01
            bottom: parent.bottom
            left: helpPaneStatusBox.left
            right: helpPaneStatusBox.right
        }

        height: bottomBtnsLayoutHeight

        AppButton {
            id: okBtn

            dedicatedShortcutEnabled: presenter.helpPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.okButtonLabel
            buttonToolTip: GameStrings.okButtonToolTip
            shortcutSequence: GameStrings.okButtonShortcut

            onButtonClicked: presenter.goBack()
        }

        AppButton {
            id: quitBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.quitButtonLabel
            buttonToolTip: GameStrings.quitButtonToolTip

            onButtonClicked: presenter.quit()
        }
    }

    onQuitButtonShortcutActivatedChanged: {
        Animations.updateButtonOpacityAtShortcutActivation(quitBtn);
        quitButtonShortcutActivated = false;
    }
}
