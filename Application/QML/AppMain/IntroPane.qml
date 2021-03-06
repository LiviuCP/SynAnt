import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0
import StatusBoxes 1.0
import Animations 1.0

Item {
    id: introPane

    property QtObject presenter

    property bool dataEntryButtonShortcutActivated: false
    property bool languageDropdownShortcutActivated: false
    property bool helpButtonShortcutActivated: false
    property bool quitButtonShortcutActivated: false

    readonly property double introPaneStatusBoxHeight: height * 0.8
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double languageDropdownLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (introPaneStatusBox.width - 3 * bottomBtnsLayout.spacing) / 4
    readonly property double buttonRadiusRatio: 0.025

    StatusBox {
        id: introPaneStatusBox

        width: parent.width
        height: introPaneStatusBoxHeight

        anchors.top: parent.top

        statusMessage: presenter.introPaneMessage
        boxToolTip: GameStrings.introPaneToolTip
    }

    RowLayout {
        id: languageDropdownLayout

        height: languageDropdownLayoutHeight

        anchors {
            top: introPaneStatusBox.bottom
            topMargin: parent.height * 0.01
            left: introPaneStatusBox.left
            right: introPaneStatusBox.right
        }

        Dropdown {
            id: languageSelectionDropdown

            Layout.minimumWidth: 0.75 * parent.width

            dropdownEnabled: presenter.languageSelectionEnabled && !presenter.dataEntry.dataSavingInProgress && !presenter.dataEntry.dataFetchingInProgress

            dataModel: GameStrings.languagesList
            currentIndex: presenter.languageIndex

            dropdownToolTip: GameStrings.languageSelectionToolTip
            noItemSelectedText: GameStrings.selectLanguageLabel

            onItemChanged: {
                presenter.handleLanguageChangeRequest(selectedIndex);
                presenter.dataEntry.handleLanguageChangeRequest(selectedIndex);
            }
        }

        AppSwitch {
            id: timeLimitSwitch

            switchEnabled: !presenter.dataFetchingInProgress
            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth * 0.98

            customHeight: languageSelectionDropdown.height
            leftSwitchLabel: GameStrings.timeLimitSwitchLabel

            switchChecked: presenter.timeLimitEnabled

            switchToolTip: switchChecked ? GameStrings.timeLimitDisableSwitchToolTip : GameStrings.timeLimitEnableSwitchToolTip

            onSwitchClicked: presenter.setTimeLimitEnabled(!presenter.timeLimitEnabled)
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            top: languageDropdownLayout.bottom
            bottom: parent.bottom
            topMargin: parent.height * 0.01
            left: introPaneStatusBox.left
            right: introPaneStatusBox.right
        }

        AppButton {
            id: playBtn

            buttonEnabled: presenter.playEnabled && presenter.languageIndex !== -1
            dedicatedShortcutEnabled: presenter.playEnabled && presenter.currentPane === GamePresenter.INTRO_PANE

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.playButtonLabel
            buttonToolTip: GameStrings.playButtonToolTip
            shortcutSequence: GameStrings.playButtonShortcut

            onButtonClicked: presenter.currentPane = GamePresenter.MAIN_PANE
        }

        AppButton {
            id: dataEntryBtn

            buttonEnabled: presenter.dataEntry.dataEntryEnabled && presenter.dataEntry.languageIndex !== -1 && !presenter.dataEntry.dataFetchingInProgress
            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.dataEntryButtonLabel
            buttonToolTip: GameStrings.dataEntryButtonToolTip

            onButtonClicked: presenter.currentPane = GamePresenter.DATA_ENTRY_PANE
        }

        AppButton {
            id: helpBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.helpButtonLabel
            buttonToolTip: presenter.helpButtonToolTip

            onButtonClicked: presenter.currentPane = GamePresenter.HELP_PANE
        }

        AppButton {
            id: quitBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.5

            buttonLabel: GameStrings.quitButtonLabel
            buttonToolTip: GameStrings.quitButtonToolTip

            onButtonClicked: presenter.quit()
        }
    }

    onDataEntryButtonShortcutActivatedChanged: {
        if (dataEntryButtonShortcutActivated)
        {
            Animations.updateButtonOpacityAtShortcutActivation(dataEntryBtn);
            dataEntryButtonShortcutActivated = false;
        }
    }

    onLanguageDropdownShortcutActivatedChanged: {
        if (languageDropdownShortcutActivated) {
            languageSelectionDropdown.forceActiveFocus();
            languageSelectionDropdown.popup.open();
            languageDropdownShortcutActivated = false;
        }
    }

    onHelpButtonShortcutActivatedChanged: {
        if (helpButtonShortcutActivated)
        {
            Animations.updateButtonOpacityAtShortcutActivation(helpBtn);
            helpButtonShortcutActivated = false;
        }
    }

    onQuitButtonShortcutActivatedChanged: {
        if (quitButtonShortcutActivated)
        {
            Animations.updateButtonOpacityAtShortcutActivation(quitBtn);
            quitButtonShortcutActivated = false;
        }
    }
}
