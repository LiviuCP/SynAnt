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

    readonly property double bottomBtnsMinWidth: (infoLevelsAndStatusLayout.width - 3 * bottomBtnsLayout.spacing) * 0.25

    MouseArea {
        id: mainPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: mainPaneToolTip
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
        text: GameStrings.mainPaneToolTip
        visible: mainPaneMouseArea.containsMouse
    }

    RowLayout {
        id: scoresLayout

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: scoresLayoutHeight

        Rectangle {
            id: highscoresRect

            Layout.minimumWidth: parent.width / 3
            Layout.minimumHeight: parent.height / 2

            color: Styles.backgroundColor

            border {
                color: Styles.borderColor
            }

            MouseArea {
                id: highscoresMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: highscoresToolTip
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
                text: GameStrings.highscoresToolTip
                visible: highscoresMouseArea.containsMouse
            }

            Text {
                color: Styles.textColor
                text: presenter.mainPaneScoreMessage

                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.01
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            id: wordPairsRect

            Layout.minimumWidth: parent.width / 3
            Layout.minimumHeight: parent.height / 2
            Layout.leftMargin: parent.width * (-0.04)

            color: Styles.backgroundColor

            border {
                color: Styles.borderColor
            }

            MouseArea {
                id: wordPairsMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: wordPairsToolTip
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
                text: GameStrings.wordPairsToolTip
                visible: wordPairsMouseArea.containsMouse
            }

            Text {
                color: Styles.textColor
                text: presenter.mainPaneWordPairsMessage

                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.01
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Button {
            id: resetBtn

            contentItem: Text {
                text: GameStrings.resetButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.01
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            Layout.minimumWidth: quitBtn.width
            Layout.alignment: Qt.AlignRight

            ToolTip.text: GameStrings.resetButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            enabled: presenter.resetEnabled

            Shortcut {
                sequence: GameStrings.resetButtonShortcut
                onActivated: {
                    presenter.handleResetRequest();
                }
            }

            onClicked: {
                presenter.handleResetRequest();
            }

            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }

    RowLayout {
        id: infoLevelsAndStatusLayout

        anchors.top: scoresLayout.bottom
        anchors.topMargin: parent.height * 0.05

        width: parent.width
        height: infoLevelsAndStatusLayoutHeight

        Rectangle {
            id: infoRect

            Layout.minimumWidth: parent.width * 0.55
            Layout.minimumHeight: parent.height

            color: Styles.backgroundColor

            border {
                color: Styles.borderColor
            }

            MouseArea {
                id: infoMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: infoToolTip
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
                text: GameStrings.gameInstructionsToolTip
                visible: infoMouseArea.containsMouse
            }

            Text {
                color: Styles.textColor
                text: GameStrings.mainPaneInstructionsMessage

                anchors.fill: parent
                anchors.leftMargin: parent.width * 0.01
                anchors.rightMargin: parent.width * 0.01

                width: parent.width
                wrapMode: Text.WordWrap
            }
        }

        Rectangle {
            id: levelBtnsRect

            Layout.minimumWidth: parent.width * 0.15
            Layout.minimumHeight: parent.height * 0.6

            color: Styles.backgroundColor

            border {
                color: Styles.borderColor
            }

            MouseArea {
                id: levelBtnsMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: levelBtnsToolTip
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
                text: GameStrings.levelButtonsToolTip
                visible: levelBtnsMouseArea.containsMouse
            }

            ColumnLayout {
                anchors.fill: parent

                RadioButton {
                    id: easyLvlBtn
                    text: GameStrings.levelEasyButtonLabel
                    checked: false

                    Shortcut {
                        sequence: GameStrings.levelEasyButtonShortcut
                        onActivated: {
                            if (!easyLvlBtn.checked) {
                                easyLvlBtn.checked = true;
                                presenter.switchToLevel(presenter.levelEasy);
                            }
                        }
                    }

                    onToggled: {
                        checked = true
                        presenter.switchToLevel(presenter.levelEasy);
                    }
                }

                RadioButton {
                    id: mediumLvlBtn
                    text: GameStrings.levelMediumButtonLabel
                    checked: true

                    Shortcut {
                        sequence: GameStrings.levelMediumButtonShortcut
                        onActivated: {
                            if (!mediumLvlBtn.checked) {
                                mediumLvlBtn.checked = true;
                                presenter.switchToLevel(presenter.levelMedium);
                            }
                        }
                    }

                    onToggled: {
                        checked = true
                        presenter.switchToLevel(presenter.levelMedium);
                    }
                }

                RadioButton {
                    id: hardLvlBtn
                    text: GameStrings.levelHardButtonLabel
                    checked: false

                    Shortcut {
                        sequence: GameStrings.levelHardButtonShortcut
                        onActivated: {
                            if (!hardLvlBtn.checked) {
                                hardLvlBtn.checked = true;
                                presenter.switchToLevel(presenter.levelHard);
                            }
                        }
                    }

                    onToggled: {
                        checked = true
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

            border {
                color: Styles.borderColor
            }

            MouseArea {
                id: statusMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip {
                id: statusToolTip
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
                text: GameStrings.gameStatusToolTip
                visible: statusMouseArea.containsMouse
            }

            Text {
                color: Styles.textColor

                text: presenter.mainPaneStatusMessage

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
    }

    Row {
        id: wordPieces

        anchors.top: infoLevelsAndStatusLayout.bottom
        anchors.left: infoLevelsAndStatusLayout.left
        anchors.right: infoLevelsAndStatusLayout.right
        anchors.topMargin: parent.height * 0.025

        height: wordPiecesHeight

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
                    onTriggered: {
                        presenter.selectWordPieceForFirstInputWord(index);
                    }
                }

                Text {
                    font.pointSize: wordPieces.height * 0.4
                    anchors.centerIn: parent
                    text: modelData
                    color: presenter.mixedWordsPiecesTextColors[index]
                }

                MouseArea {
                    id: mixedWordsCurrentPieceMouseArea
                    anchors.fill: parent
                    hoverEnabled: true

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
                    id: mixedWordsCurrentPieceToolTip
                    delay: presenter.toolTipDelay
                    timeout: presenter.toolTipTimeout
                    text: presenter.mixedWordsPiecesSelections[index] ? GameStrings.wordPieceAlreadySelectedToolTip : GameStrings.selectWordPieceToolTip
                    visible: mixedWordsCurrentPieceMouseArea.containsMouse
                }
            }
        }
    }

    Row {
        id: wordsEntryLayout

        anchors.top: wordPieces.bottom
        anchors.left: wordPieces.left
        anchors.right: wordPieces.right
        anchors.topMargin: parent.width * 0.01

        height: wordsEntryLayoutHeight

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
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: presenter.updateFirstWordInputHoverIndex(index)
                    onExited: presenter.clearWordInputHoverIndexes()

                    onClicked: {
                        presenter.removeWordPiecesFromFirstInputWord(index);
                    }
                }

                ToolTip {
                    id: firstWordInputCurrentPieceToolTip
                    delay: presenter.toolTipDelay
                    timeout: presenter.toolTipTimeout
                    text: GameStrings.firstWordInputToolTip
                    visible: firstWordInputCurrentPieceMouseArea.containsMouse
                }
            }
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
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: presenter.updateSecondWordInputHoverIndex(index)
                    onExited: presenter.clearWordInputHoverIndexes()

                    onClicked: {
                        presenter.removeWordPiecesFromSecondInputWord(index);
                    }
                }

                ToolTip {
                    id: secondWordInputCurrentPieceToolTip
                    delay: presenter.toolTipDelay
                    timeout: presenter.toolTipTimeout
                    text: GameStrings.secondWordInputToolTip
                    visible: secondWordInputCurrentPieceMouseArea.containsMouse
                }
            }
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors.bottom: parent.bottom
        anchors.left: wordsEntryLayout.left
        anchors.right: wordsEntryLayout.right

        height: bottomBtnsLayoutHeight



        Button {
            id: submitBtn
            enabled: presenter.submitEnabled

            contentItem: Text {
                text: GameStrings.submitButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.005
                radius: width * 0.01
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.submitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: GameStrings.submitButtonShortcut
                onActivated: {
                    presenter.handleSubmitRequest();
                }
            }

            onClicked: presenter.handleSubmitRequest()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: helpBtn

            contentItem: Text {
                text: GameStrings.helpButtonLabel
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

            ToolTip.text: GameStrings.helpButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            onClicked: presenter.switchToPane(GamePresenter.HELP)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: resultsBtn

            contentItem: Text {
                text: GameStrings.resultsButtonLabel
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

            ToolTip.text: GameStrings.resultsButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: GameStrings.resultsButtonShortcut
                onActivated: {
                    presenter.handleResultsRequest()
                }
            }

            onClicked: presenter.handleResultsRequest()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: quitBtn

            contentItem: Text {
                text: GameStrings.quitButtonLabel
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

            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.25

            ToolTip.text: GameStrings.quitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            onClicked: Qt.quit()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }
}
