import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0

Item {
    id: mainPane

    property QtObject presenter

    readonly property double scoresLayoutHeight: height * 0.1
    readonly property double infoLevelsAndStatusLayoutHeight: height * 0.55
    readonly property double wordPiecesHeight: height * 0.06
    readonly property double wordsEntryLayoutHeight: height * 0.05
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (infoLevelsAndStatusLayout.width - 4 * bottomBtnsLayout.spacing) * 0.2
    readonly property double buttonRadiusRatio: 0.035

    MouseArea {
        id: mainPaneMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        text: GameStrings.mainPaneToolTip
        visible: mainPaneMouseArea.containsMouse
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
    }

    RowLayout {
        id: scoresLayout

        height: scoresLayoutHeight

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Rectangle {
            id: highscoresRect

            Layout.minimumWidth: (infoRect.width - scoresLayout.spacing) / 2
            Layout.minimumHeight: parent.height / 2

            color: Styles.backgroundColor
            border.color: Styles.borderColor

            MouseArea {
                id: highscoresMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                text: GameStrings.highscoresToolTip
                visible: highscoresMouseArea.containsMouse
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Text {
                color: Styles.textColor
                text: presenter.mainPaneScoreMessage

                anchors {
                    left: parent.left
                    leftMargin: parent.width * 0.01
                    verticalCenter: parent.verticalCenter
                }
            }
        }

        Rectangle {
            id: wordPairsRect

            Layout.minimumWidth: (infoRect.width - scoresLayout.spacing) / 2
            Layout.minimumHeight: parent.height / 2

            color: Styles.backgroundColor
            border.color: Styles.borderColor

            MouseArea {
                id: wordPairsMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                text: GameStrings.wordPairsToolTip
                visible: wordPairsMouseArea.containsMouse
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Text {
                text: presenter.mainPaneWordPairsMessage
                color: Styles.textColor

                anchors {
                    left: parent.left
                    leftMargin: parent.width * 0.01
                    verticalCenter: parent.verticalCenter
                }
            }
        }

        Button {
            id: resetBtn

            enabled: presenter.resetEnabled

            Layout.minimumWidth: resultsBtn.width
            Layout.alignment: Qt.AlignRight
            Layout.leftMargin: 0.05 * mainPane.width

            contentItem: Text {
                text: GameStrings.resetButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.resetButtonToolTip
                visible: resetBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.resetButtonShortcut
                enabled: presenter.mainPaneVisible

                onActivated: presenter.handleResetRequest()
            }

            onClicked: presenter.handleResetRequest()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: dataEntryBtn

            enabled: presenter.dataEntryEnabled

            Layout.minimumWidth: quitBtn.width

            contentItem: Text {
                text: GameStrings.dataEntryButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                radius: width * buttonRadiusRatio
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.dataEntryButtonToolTip
                visible: dataEntryBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            onClicked: presenter.switchToPane(GamePresenter.DATA_ENTRY)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }

    RowLayout {
        id: infoLevelsAndStatusLayout

        width: parent.width
        height: infoLevelsAndStatusLayoutHeight

        anchors {
            top: scoresLayout.bottom
            topMargin: parent.height * 0.05
        }

        Rectangle {
            id: infoRect

            Layout.minimumWidth: parent.width * 0.55
            Layout.minimumHeight: parent.height

            color: Styles.backgroundColor
            border.color: Styles.borderColor

            MouseArea {
                id: infoMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                text: GameStrings.gameInstructionsToolTip
                visible: infoMouseArea.containsMouse
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Text {
                text: GameStrings.mainPaneInstructionsMessage
                color: Styles.textColor
                width: parent.width
                wrapMode: Text.WordWrap

                anchors {
                    fill: parent
                    leftMargin: parent.width * 0.01
                    rightMargin: parent.width * 0.01
                }
            }
        }

        Rectangle {
            id: levelBtnsRect

            Layout.minimumWidth: parent.width * 0.15
            Layout.minimumHeight: parent.height * 0.6

            color: Styles.backgroundColor
            border.color: Styles.borderColor

            MouseArea {
                id: levelBtnsMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                text: GameStrings.levelButtonsToolTip
                visible: levelBtnsMouseArea.containsMouse
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            ColumnLayout {
                anchors.fill: parent

                RadioButton {
                    id: easyLvlBtn

                    checked: false
                    text: GameStrings.levelEasyButtonLabel

                    Shortcut {
                        sequence: GameStrings.levelEasyButtonShortcut
                        enabled: presenter.mainPaneVisible

                        onActivated: {
                            if (!easyLvlBtn.checked) {
                                easyLvlBtn.checked = true;
                                presenter.switchToLevel(presenter.levelEasy);
                            }
                        }
                    }

                    onToggled: {
                        checked = true;
                        presenter.switchToLevel(presenter.levelEasy);
                    }
                }

                RadioButton {
                    id: mediumLvlBtn

                    checked: true
                    text: GameStrings.levelMediumButtonLabel

                    Shortcut {
                        sequence: GameStrings.levelMediumButtonShortcut
                        enabled: presenter.mainPaneVisible

                        onActivated: {
                            if (!mediumLvlBtn.checked) {
                                mediumLvlBtn.checked = true;
                                presenter.switchToLevel(presenter.levelMedium);
                            }
                        }
                    }

                    onToggled: {
                        checked = true;
                        presenter.switchToLevel(presenter.levelMedium);
                    }
                }

                RadioButton {
                    id: hardLvlBtn

                    checked: false
                    text: GameStrings.levelHardButtonLabel

                    Shortcut {
                        sequence: GameStrings.levelHardButtonShortcut
                        enabled: presenter.mainPaneVisible

                        onActivated: {
                            if (!hardLvlBtn.checked) {
                                hardLvlBtn.checked = true;
                                presenter.switchToLevel(presenter.levelHard);
                            }
                        }
                    }

                    onToggled: {
                        checked = true;
                        presenter.switchToLevel(presenter.levelHard);
                    }
                }
            }
        }

        Rectangle {
            id: statusRect

            Layout.minimumWidth: parent.width * 0.242
            Layout.minimumHeight: parent.height
            Layout.alignment: Qt.AlignRight

            color: Styles.backgroundColor
            border.color: Styles.borderColor

            MouseArea {
                id: statusMouseArea

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                text: GameStrings.gameStatusToolTip
                visible: statusMouseArea.containsMouse
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Text {
                text: presenter.mainPaneStatusMessage
                color: Styles.textColor

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
    }

    Row {
        id: wordPieces

        height: wordPiecesHeight

        anchors {
            top: infoLevelsAndStatusLayout.bottom
            topMargin: parent.height * 0.025
            left: infoLevelsAndStatusLayout.left
            right: infoLevelsAndStatusLayout.right
        }

        Repeater {
            id: mixedWordsRepeater

            model: presenter.mixedWordsPiecesContent

            Rectangle {
                width: parent.width / mixedWordsRepeater.count
                height: parent.height

                visible: !presenter.mixedWordsPiecesSelections[index]

                color: Styles.backgroundColor
                border.color: Styles.borderColor

                Timer {
                    id: clickTimer
                    interval: 200
                    onTriggered: presenter.selectWordPieceForFirstInputWord(index)
                }

                Text {
                    text: modelData
                    font.pointSize: wordPieces.height * 0.4
                    anchors.centerIn: parent
                    color: presenter.mixedWordsPiecesTextColors[index]
                }

                MouseArea {
                    id: mixedWordsCurrentPieceMouseArea

                    hoverEnabled: true
                    anchors.fill: parent

                    // single click for assigning piece to first word, double click for assigning to second word
                    onClicked: {
                        if (clickTimer.running) {
                            presenter.selectWordPieceForSecondInputWord(index);
                            clickTimer.stop();
                        }
                        else {
                            clickTimer.restart();
                        }
                    }
                }

                ToolTip {
                    text: presenter.mixedWordsPiecesSelections[index] ? GameStrings.wordPieceAlreadySelectedToolTip : GameStrings.selectWordPieceToolTip
                    visible: mixedWordsCurrentPieceMouseArea.containsMouse
                    delay: presenter.toolTipDelay
                    timeout: presenter.toolTipTimeout
                }
            }
        }
    }

    Row {
        id: wordsEntryLayout

        height: wordsEntryLayoutHeight

        anchors {
            top: wordPieces.bottom
            topMargin: parent.width * 0.01
            left: wordPieces.left
            right: wordPieces.right
        }

        Repeater {
            id: firstWordInputRepeater

            model: presenter.firstWordInputPiecesContent

            Rectangle {
                property bool isHoverSelected: presenter.isFirstWordInputHovered && presenter.firstWordInputHoverIndex <= index

                width: parent.width / mixedWordsRepeater.count
                height: parent.height

                opacity: isHoverSelected ? Styles.hoverOpacity : Styles.defaultOpacity

                color: isHoverSelected ? Styles.markedForDeletionColor : Styles.backgroundColor
                border.color: Styles.borderColor

                Text {
                    font.pointSize: wordPieces.height * 0.4
                    anchors.centerIn: parent
                    text: modelData
                    color: presenter.firstWordInputPiecesTextColors[index]
                }

                MouseArea {
                    id: firstWordInputCurrentPieceMouseArea

                    hoverEnabled: true
                    anchors.fill: parent

                    onEntered: presenter.updateFirstWordInputHoverIndex(index)
                    onExited: presenter.clearWordInputHoverIndexes()
                    onClicked: presenter.removeWordPiecesFromFirstInputWord(index)
                }

                ToolTip {
                    text: GameStrings.firstWordInputToolTip
                    visible: firstWordInputCurrentPieceMouseArea.containsMouse
                    delay: presenter.toolTipDelay
                    timeout: presenter.toolTipTimeout
                }
            }
        }

        Shortcut {
            sequence: GameStrings.clearFirstInputWordShortcut
            enabled: presenter.mainPaneVisible

            onActivated: presenter.clearFirstInputWord()
        }

        Repeater {
            id: secondWordInputRepeater

            model: presenter.secondWordInputPiecesContent

            Rectangle {
                property bool isHoverSelected: presenter.isSecondWordInputHovered && presenter.secondWordInputHoverIndex <= index

                width: parent.width / mixedWordsRepeater.count
                height: parent.height

                opacity: isHoverSelected ? Styles.hoverOpacity : Styles.defaultOpacity

                color: isHoverSelected ? Styles.markedForDeletionColor : Styles.backgroundColor
                border.color: Styles.borderColor

                Text {
                    font.pointSize: wordPieces.height * 0.4
                    anchors.centerIn: parent
                    text: modelData
                    color: presenter.secondWordInputPiecesTextColors[index]
                }

                MouseArea {
                    id: secondWordInputCurrentPieceMouseArea

                    hoverEnabled: true
                    anchors.fill: parent

                    onEntered: presenter.updateSecondWordInputHoverIndex(index)
                    onExited: presenter.clearWordInputHoverIndexes()
                    onClicked: presenter.removeWordPiecesFromSecondInputWord(index)
                }

                ToolTip {
                    text: GameStrings.secondWordInputToolTip
                    visible: secondWordInputCurrentPieceMouseArea.containsMouse
                    delay: presenter.toolTipDelay
                    timeout: presenter.toolTipTimeout
                }
            }
        }

        Shortcut {
            sequence: GameStrings.clearSecondInputWordShortcut
            enabled: presenter.mainPaneVisible

            onActivated: presenter.clearSecondInputWord()
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            bottom: parent.bottom
            left: wordsEntryLayout.left
            right: wordsEntryLayout.right
        }

        Button {
            id: submitBtn

            enabled: presenter.submitEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.submitButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.submitButtonToolTip
                visible: submitBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.submitButtonShortcut
                enabled: presenter.mainPaneVisible

                onActivated: presenter.handleSubmitRequest()
            }

            onClicked: presenter.handleSubmitRequest()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: clearInputBtn

            enabled: presenter.clearInputEnabled

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.clearInputButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.clearInputButtonToolTip
                visible: clearInputBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.clearInputButtonShortcut
                enabled: presenter.mainPaneVisible

                onActivated: presenter.clearInput()
            }

            onClicked: presenter.clearInput()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: helpBtn

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.helpButtonLabel
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
                text: GameStrings.helpButtonToolTip
                visible: helpBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            onClicked: presenter.switchToPane(GamePresenter.HELP)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: resultsBtn

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.resultsButtonLabel
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
                text: GameStrings.resultsButtonToolTip
                visible: resultsBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.resultsButtonShortcut
                enabled: presenter.mainPaneVisible

                onActivated: presenter.handleResultsRequest()
            }

            onClicked: presenter.handleResultsRequest()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: quitBtn

            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.2

            contentItem: Text {
                text: GameStrings.quitButtonLabel
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
                text: GameStrings.quitButtonToolTip
                visible: quitBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            onClicked: presenter.quit()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }
}
