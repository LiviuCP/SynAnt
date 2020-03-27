import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0
import StatusBoxes 1.0
import Animations 1.0

Item {
    id: mainPane

    property bool dataEntryButtonShortcutActivated: false
    property bool languageDropdownShortcutActivated: false
    property bool helpButtonShortcutActivated: false
    property bool quitButtonShortcutActivated: false
    property bool upArrowPressed: false
    property bool downArrowPressed: false
    property bool languageDropdownOpened: languageSelectionDropdown.dropdownOpened
    property bool languageDropdownInFocus: languageSelectionDropdown.focus

    property QtObject presenter

    readonly property double statisticsLayoutHeight: height * 0.1
    readonly property double mainLayoutHeight: height * 0.55
    readonly property double wordPiecesHeight: height * 0.06
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (mainLayout.width - 4 * bottomBtnsLayout.spacing) * 0.2

    MouseArea {
        id: mainPaneMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        text: GameStrings.mainPaneToolTip
        visible: mainPaneMouseArea.containsMouse
        delay: Animations.toolTipDelay
        timeout: Animations.toolTipTimeout
    }

    RowLayout {
        id: statisticsLayout

        height: statisticsLayoutHeight
        width: parent.width

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        StatisticsBox {
            id: highscoresBox

            boxLabel: GameStrings.scoreLabel
            leftValue: presenter.obtainedScore
            rightValue: presenter.totalAvailableScore

            Layout.minimumWidth: (statisticsLayout.width - 2 * statisticsLayout.spacing) / 3
            Layout.minimumHeight: parent.height / 2

            boxToolTip: GameStrings.highscoresToolTip
        }

        StatisticsBox {
            id: wordPairsBox

            boxLabel: GameStrings.pairsLabel
            leftValue: presenter.guessedWordPairs
            rightValue: presenter.totalWordPairs

            Layout.minimumWidth: (statisticsLayout.width - 2 * statisticsLayout.spacing) / 3
            Layout.minimumHeight: parent.height / 2

            boxToolTip: GameStrings.wordPairsToolTip
        }

        StatisticsBox {
            id: timeLimitBox

            boxLabel: GameStrings.remainingTimeLabel
            leftValue: presenter.remainingTimeMinSec[0]
            rightValue: presenter.remainingTimeMinSec[1]

            Layout.minimumWidth: (statisticsLayout.width - 2 * statisticsLayout.spacing) / 3
            Layout.minimumHeight: parent.height / 2

            boxToolTip: GameStrings.remainingTimeToolTip
        }
    }

    RowLayout {
        id: mainLayout

        width: parent.width
        height: mainLayoutHeight

        anchors {
            top: statisticsLayout.bottom
            topMargin: parent.height * 0.01
        }

        ColumnLayout {
            Layout.minimumWidth: highscoresBox.width
            Layout.minimumHeight: parent.height

            AppButton {
                id: resetBtn

                buttonEnabled: presenter.mainPaneStatisticsResetEnabled
                dedicatedShortcutEnabled: presenter.currentPane === GamePresenter.MAIN && presenter.mainPaneStatisticsResetEnabled

                Layout.minimumWidth: highscoresBox.width

                buttonLabel: GameStrings.mainPaneStatisticsResetButtonLabel
                buttonToolTip: GameStrings.mainPaneResetButtonToolTip
                shortcutSequence: GameStrings.mainPaneResetButtonShortcut

                onButtonClicked: presenter.handleMainPaneStatisticsResetRequest()
            }

            AppButton {
                id: dataEntryBtn

                buttonEnabled: presenter.dataEntry.dataEntryEnabled && !presenter.dataEntry.dataFetchingInProgress
                dedicatedShortcutEnabled: false

                Layout.minimumWidth: highscoresBox.width

                buttonLabel: GameStrings.dataEntryButtonLabel
                buttonToolTip: GameStrings.dataEntryButtonToolTip

                onButtonClicked: presenter.currentPane = GamePresenter.DATA_ENTRY
            }

            Dropdown {
                id: languageSelectionDropdown

                Layout.minimumWidth: highscoresBox.width

                dropdownEnabled: presenter.languageSelectionEnabled && !presenter.dataEntry.dataSavingInProgress && !presenter.dataEntry.dataFetchingInProgress

                dataModel: GameStrings.languagesList
                currentIndex: presenter.languageIndex

                dropdownToolTip: GameStrings.languageSelectionToolTip

                onItemChanged: {
                    presenter.handleLanguageChangeRequest(selectedIndex, true);
                    presenter.dataEntry.handleLanguageChangeRequest(selectedIndex);
                }
            }

            Rectangle {
                id: levelBtnsRect

                Layout.minimumWidth: highscoresBox.width
                Layout.fillHeight: true

                color: Styles.statusBoxBackgroundColor
                border.color: Styles.borderColor

                MouseArea {
                    id: levelBtnsMouseArea

                    anchors.fill: parent
                    hoverEnabled: true
                }

                ToolTip {
                    text: GameStrings.levelButtonsToolTip
                    visible: levelBtnsMouseArea.containsMouse
                    delay: Animations.toolTipDelay
                    timeout: Animations.toolTipTimeout
                }

                ColumnLayout {
                    anchors.fill: parent

                    AppRadioButton {
                        id: easyLvlBtn

                        buttonChecked: false
                        buttonEnabled: !presenter.dataFetchingInProgress
                        dedicatedShortcutEnabled: presenter.currentPane === GamePresenter.MAIN

                        buttonLabel: GameStrings.levelEasyButtonLabel
                        shortcutSequence: GameStrings.levelEasyButtonShortcut

                        onButtonToggled: presenter.setLevel(GamePresenter.LEVEL_EASY)
                    }

                    AppRadioButton {
                        id: mediumLvlBtn

                        buttonChecked: true
                        buttonEnabled: !presenter.dataFetchingInProgress
                        dedicatedShortcutEnabled: presenter.currentPane === GamePresenter.MAIN

                        buttonLabel: GameStrings.levelMediumButtonLabel
                        shortcutSequence: GameStrings.levelMediumButtonShortcut

                        onButtonToggled: presenter.setLevel(GamePresenter.LEVEL_MEDIUM)
                    }

                    AppRadioButton {
                        id: hardLvlBtn

                        buttonChecked: false
                        buttonEnabled: !presenter.dataFetchingInProgress
                        dedicatedShortcutEnabled: presenter.currentPane === GamePresenter.MAIN

                        buttonLabel: GameStrings.levelHardButtonLabel
                        shortcutSequence: GameStrings.levelHardButtonShortcut

                        onButtonToggled: presenter.setLevel(GamePresenter.LEVEL_HARD)
                    }
                }
            }
        }

        ColumnLayout {
            Layout.minimumWidth: 2 * wordPairsBox.width + statisticsLayout.spacing
            Layout.minimumHeight: parent.height

            AppSwitch {
                id: timeLimitSwitch

                switchEnabled: !presenter.dataFetchingInProgress
                dedicatedShortcutEnabled: false

                Layout.alignment: Layout.Bottom
                Layout.preferredWidth: 0.998 * parent.width
                Layout.leftMargin: 0.001 * width

                customHeight: resetBtn.height
                Layout.preferredHeight: resetBtn.height

                leftSwitchLabel: GameStrings.timeLimitOffLabel
                rightSwitchLabel: GameStrings.timeLimitOnLabel

                switchChecked: presenter.timeLimitEnabled

                switchToolTip: switchChecked ? GameStrings.timeLimitDisableSwitchToolTip : GameStrings.timeLimitEnableSwitchToolTip

                onSwitchClicked: presenter.setTimeLimitEnabled(!presenter.timeLimitEnabled)
            }

            StatusBox {
                id: mainPaneStatusBox
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: parent.height - resetBtn.height - parent.spacing
                statusMessage: presenter.mainPaneStatusMessage
                boxToolTip: GameStrings.mainPaneStatusBoxToolTip
            }
        }
    }

    Row {
        id: firstWordsInputContainer

        height: wordPiecesHeight

        anchors {
            top: mainLayout.bottom
            topMargin: parent.width * 0.02
            left: mainLayout.left
            right: mainLayout.right
        }

        WordPiecesInput {
            id: firstWordInput
            gamePresenter: presenter
            clearShortcutEnabled: presenter.currentPane === GamePresenter.MAIN
            clearShortcutSequence: GameStrings.clearMainPaneFirstInputWordShortcut
            pieceWidth: parent.width / mixedWordPieces.count
            pieceHeight: parent.height
        }
    }

    Row {
        id: mixedWordPiecesContainer

        height: wordPiecesHeight

        anchors {
            top: firstWordsInputContainer.bottom
            topMargin: parent.height * 0.01
            left: mainLayout.left
            right: mainLayout.right
        }

        MixedWordPieces {
            id: mixedWordPieces

            gamePresenter: presenter
            pieceWidth: parent.width / mixedWordPieces.count
            pieceHeight: parent.height
        }
    }

    Row {
        id: secondWordsInputContainer

        height: wordPiecesHeight

        anchors {
            top: mixedWordPiecesContainer.bottom
            topMargin: parent.width * 0.01
            left: mainLayout.left
            right: mainLayout.right
        }

        WordPiecesInput {
            id: secondWordInput
            gamePresenter: presenter
            clearShortcutEnabled: presenter.currentPane === GamePresenter.MAIN
            clearShortcutSequence: GameStrings.clearMainPaneSecondInputWordShortcut
            pieceWidth: parent.width / mixedWordPieces.count
            pieceHeight: parent.height
            isFirstWord: false
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            bottom: parent.bottom
            left: mixedWordPiecesContainer.left
            right: mixedWordPiecesContainer.right
        }

        AppButton {
            id: submitBtn

            buttonEnabled: presenter.submitMainPaneInputEnabled && !presenter.dataFetchingInProgress
            dedicatedShortcutEnabled: presenter.currentPane === GamePresenter.MAIN && presenter.submitMainPaneInputEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.submitButtonLabel
            buttonToolTip: GameStrings.submitButtonToolTip
            shortcutSequence: GameStrings.submitButtonShortcut

            onButtonClicked: presenter.handleSubmitMainPaneInputRequest()
        }

        AppButton {
            id: clearInputBtn

            buttonEnabled: presenter.clearMainPaneInputEnabled && !presenter.dataFetchingInProgress
            dedicatedShortcutEnabled: presenter.currentPane === GamePresenter.MAIN && presenter.clearMainPaneInputEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.clearMainPaneInputButtonLabel
            buttonToolTip: GameStrings.clearMainPaneInputButtonToolTip
            shortcutSequence: GameStrings.clearMainPaneInputButtonShortcut

            onButtonClicked: presenter.clearMainPaneInput()
        }

        AppButton {
            id: helpBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.helpButtonLabel
            buttonToolTip: presenter.helpButtonToolTip

            onButtonClicked: presenter.currentPane = GamePresenter.HELP
        }

        AppButton {
            id: showPairBtn

            buttonEnabled: !presenter.dataFetchingInProgress
            dedicatedShortcutEnabled: presenter.currentPane === GamePresenter.MAIN

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.showPairButtonLabel
            buttonToolTip: GameStrings.showPairButtonToolTip
            shortcutSequence: GameStrings.showPairButtonShortcut

            onButtonClicked: presenter.handleDisplayCorrectWordsPairRequest()
        }

        AppButton {
            id: quitBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.2

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

    onUpArrowPressedChanged: {
        if (upArrowPressed) {
            animatePiecesControlledByCursor();
            upArrowPressed = false;
        }
    }

    onDownArrowPressedChanged: {
        if (downArrowPressed) {
            animatePiecesControlledByCursor();
            downArrowPressed = false;
        }
    }

    function animatePiecesControlledByCursor() {
        if (presenter.pieceSelectionCursorPosition !== -1) {
            mixedWordPieces.animatePieceSelectedByCursor();
        }
        else if (presenter.piecesRemovalFirstWordCursorPosition !== -1) {
            firstWordInput.animatePiecesRemovedByCursor();
        }
        else if (presenter.piecesRemovalSecondWordCursorPosition !== -1) {
            secondWordInput.animatePiecesRemovedByCursor();
        }
    }
}
