import QtQuick 2.7
import QtQuick.Controls 2.2
import GameUtils 1.0

Repeater {
    id: wordPiecesInput

    property QtObject gamePresenter
    property string clearShortcutSequence: ""
    property bool clearShortcutEnabled: false
    property double pieceWidth
    property double pieceHeight
    property bool isFirstWord : true

    property bool areInputPiecesHovered: isFirstWord ? gamePresenter.areFirstWordInputPiecesHovered : gamePresenter.areSecondWordInputPiecesHovered
    property int piecesRemovalCursorPosition : isFirstWord ? gamePresenter.piecesRemovalFirstWordCursorPosition : gamePresenter.piecesRemovalSecondWordCursorPosition
    property int hoverIndex: isFirstWord ? gamePresenter.firstWordInputPiecesHoverIndex : gamePresenter.secondWordInputPiecesHoverIndex
    property color wordInputBorderColor : isFirstWord ? Styles.firstWordInputBorderColor : Styles.secondWordInputBorderColor
    property color wordInputBackgroundColor : isFirstWord ? Styles.firstWordInputBackgroundColor : Styles.secondWordInputBackgroundColor
    property string pieceToolTip: isFirstWord ? GameStrings.mainPaneFirstWordInputToolTip : GameStrings.mainPaneSecondWordInputToolTip

    readonly property QtObject shortcut: Shortcut {
        sequence: clearShortcutSequence
        enabled: clearShortcutSequence !== "" && clearShortcutEnabled

        onActivated: {
            if (isFirstWord) {
                gamePresenter.clearMainPaneFirstInputWord();
            } else {
                gamePresenter.clearMainPaneSecondInputWord();
            }
        }
    }

    model: isFirstWord ? gamePresenter.firstWordInputPiecesContent : gamePresenter.secondWordInputPiecesContent

    property Timer clickRemovedPiecesTimer : Timer {
        interval: 100
        onTriggered: {
            if (isFirstWord) {
                gamePresenter.removePiecesFromFirstInputWord(hoverIndex);
            } else {
                gamePresenter.removePiecesFromSecondInputWord(hoverIndex);
            }
        }
    }

    property Timer cursorRemovedPiecesTimer : Timer {
        interval: 100
    }

    Rectangle {
        property bool isHoverSelected: !gamePresenter.persistentModeEnabled ? areInputPiecesHovered && hoverIndex <= index
                                                                       : false
        property bool isKeyboardSelected: gamePresenter.persistentModeEnabled ? piecesRemovalCursorPosition !== -1 &&
                                                                            piecesRemovalCursorPosition <= index
                                                                          : false

        width: pieceWidth
        height: pieceHeight

        visible: !presenter.dataFetchingInProgress

        opacity: ((clickRemovedPiecesTimer.running &&
                   index >= hoverIndex) ||
                  (cursorRemovedPiecesTimer.running &&
                   index >= piecesRemovalCursorPosition)) ? Styles.pressedOpacity
                                                                             : (isHoverSelected ? Styles.hoverOpacity
                                                                                                : (isKeyboardSelected ? Styles.hoverOpacity
                                                                                                                      : Styles.defaultOpacity))

        color: isHoverSelected ? Styles.markedForDeletionColor : (isKeyboardSelected ? Styles.markedForDeletionColor : wordInputBackgroundColor)

        border.color: index === piecesRemovalCursorPosition &&
                      !cursorRemovedPiecesTimer.running ? Styles.selectedBorderColor
                                                                                               : wordInputBorderColor

        border.width: index === piecesRemovalCursorPosition &&
                      !cursorRemovedPiecesTimer.running ? Styles.selectedBorderWidth
                                                                                               : Styles.borderWidth

        Text {
            font.pointSize: pieceHeight * 0.4
            anchors.centerIn: parent
            text: modelData
            color: isFirstWord ? gamePresenter.firstWordInputPiecesTextColors[index] : gamePresenter.secondWordInputPiecesTextColors[index]
        }

        MouseArea {
            id: wordInputCurrentPieceMouseArea

            hoverEnabled: true
            anchors.fill: parent

            onEntered: {
                if (isFirstWord) {
                    gamePresenter.updateFirstWordInputHoverIndex(index);
                } else {
                    gamePresenter.updateSecondWordInputHoverIndex(index);
                }
            }

            onExited: gamePresenter.clearWordInputHoverIndexes()
            onClicked: clickRemovedPiecesTimer.start()
        }

        ToolTip {
            text: pieceToolTip
            visible: !gamePresenter.persistentModeEnabled && wordInputCurrentPieceMouseArea.containsMouse
            delay: gamePresenter.toolTipDelay
            timeout: gamePresenter.toolTipTimeout
        }
    }

    function animatePiecesRemovedByCursor() {
        cursorRemovedPiecesTimer.start();
    }
}
