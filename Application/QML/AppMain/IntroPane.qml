import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0

Item {
    id: introPane

    property QtObject presenter

    property bool dataEntryButtonShortcutActivated: false
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

        ActivationSwitch {
            id: timeLimitSwitch

            switchEnabled: !presenter.dataFetchingInProgress

            Layout.minimumWidth: bottomBtnsMinWidth * 0.98

            customHeight: languageSelectionDropdown.height
            leftSwitchLabel: GameStrings.timeLimitSwitchLabel

            switchChecked: presenter.timeLimitEnabled

            switchToolTip: switchChecked ? GameStrings.timeLimitDisableSwitchToolTip : GameStrings.timeLimitEnableSwitchToolTip

            onSwitchToggled: presenter.setTimeLimitEnabled(switchChecked)
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
            dedicatedShortcutEnabled: presenter.introPaneVisible && presenter.playEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.playButtonLabel
            buttonToolTip: GameStrings.playButtonToolTip
            shortcutSequence: GameStrings.playButtonShortcut

            onButtonClicked: presenter.switchToPane(GamePresenter.MAIN)
        }

        AppButton {
            id: dataEntryBtn

            buttonEnabled: presenter.dataEntry.dataEntryEnabled && presenter.dataEntry.languageIndex !== -1 && !presenter.dataEntry.dataFetchingInProgress
            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.dataEntryButtonLabel
            buttonToolTip: GameStrings.dataEntryButtonToolTip

            onButtonClicked: presenter.switchToPane(GamePresenter.DATA_ENTRY)
        }

        AppButton {
            id: helpBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.helpButtonLabel
            buttonToolTip: presenter.helpButtonToolTip

            onButtonClicked: presenter.switchToPane(GamePresenter.HELP)
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

    Connections {
        target: presenter.dataEntry

        onDataSaveInProgress: presenter.handleDataSaveInProgress()
    }

    onDataEntryButtonShortcutActivatedChanged: {
        if (dataEntryButtonShortcutActivated)
        {
            Styles.updateButtonOpacityAtShortcutActivation(dataEntryBtn);
            dataEntryButtonShortcutActivated = false;
        }
    }

    onHelpButtonShortcutActivatedChanged: {
        if (helpButtonShortcutActivated)
        {
            Styles.updateButtonOpacityAtShortcutActivation(helpBtn);
            helpButtonShortcutActivated = false;
        }
    }

    onQuitButtonShortcutActivatedChanged: {
        if (quitButtonShortcutActivated)
        {
            Styles.updateButtonOpacityAtShortcutActivation(quitBtn);
            quitButtonShortcutActivated = false;
        }
    }
}
