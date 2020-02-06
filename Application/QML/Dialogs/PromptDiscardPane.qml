import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0

Item {
    id: promptDiscardPane

    property QtObject presenter

    readonly property double promptPaneRectHeight: height * 0.89
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (promptDiscardPaneStatusBox.width - bottomBtnsLayout.spacing) / 2
    readonly property double buttonRadiusRatio: 0.02

    StatusBox {
        id: promptDiscardPaneStatusBox

        width: parent.width
        height: promptPaneRectHeight

        anchors.top: parent.top

        statusMessage: GameStrings.promptDiscardMessage
        boxToolTip: GameStrings.promptPaneToolTip
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            top: promptDiscardPaneStatusBox.bottom
            topMargin: parent.height * 0.01
            bottom: parent.bottom
            left: promptDiscardPaneStatusBox.left
            right: promptDiscardPaneStatusBox.right
        }

        AppButton {
            id: yesBtn

            dedicatedShortcutEnabled: presenter.promptDiscardPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.promptDiscardYesButtonLabel
            buttonToolTip: GameStrings.promptDiscardYesButtonToolTip
            shortcutSequence: GameStrings.promptDiscardYesButtonShortcut

            onButtonClicked: {
                presenter.dataEntry.handleClearAddedWordPairsRequest();
                presenter.goBack();
            }
        }

        AppButton {
            id: cancelBtn

            dedicatedShortcutEnabled: presenter.promptDiscardPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.promptDiscardCancelButtonLabel
            buttonToolTip: GameStrings.promptDiscardCancelButtonToolTip
            shortcutSequence: GameStrings.promptDiscardCancelButtonShortcut

            onButtonClicked: presenter.goBack()
        }
    }
}
