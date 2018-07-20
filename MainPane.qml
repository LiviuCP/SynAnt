import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: mainPane

    property QtObject presenter;

    readonly property double scoresLayoutHeight: height * 0.1;
    readonly property double infoLevelsAndStatusLayoutHeight: height * 0.55
    readonly property double wordLabelsLayoutHeight: height * 0.05
    readonly property double wordsEntryLayoutHeight: height * 0.05
    readonly property double bottomBtnsLayoutHeight: height * 0.1

    readonly property double bottomBtnsMinWidth: (infoLevelsAndStatusLayout.width - 3 * bottomBtnsLayout.spacing) * 0.25

    RowLayout {
        id: scoresLayout
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: scoresLayoutHeight

        Rectangle {
            id: highScoresRect
            Layout.minimumWidth: parent.width / 3
            Layout.minimumHeight: parent.height / 2
            color: "grey"

            border {
                color: "white"
            }

            Text {
                color: "white"
                text: "High-score: 0/0"
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.01
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Rectangle {
            id: wordPairsRect
            Layout.minimumWidth: parent.width / 3
            Layout.minimumHeight: parent.height / 2
            color: "grey"
            Layout.leftMargin: parent.width * (-0.04)

            border {
                color: "white"
            }

            Text {
                color: "white"
                text: "Word pairs: 0/0"
                anchors.left: parent.left
                anchors.leftMargin: parent.width * 0.01
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Button {
            id: resetBtn
            text: "Reset"
            Layout.minimumWidth: quitBtn.width
            Layout.alignment: Qt.AlignRight
            onClicked: console.log("Button Reset works!!!")
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
            color: "grey"

            border {
                color: "white"
            }

            Text {
                color: "white"
                text: presenter.mainPaneInstructionsMessage
                anchors.fill: parent
                anchors.leftMargin: parent.width * 0.01
                anchors.rightMargin: parent.width * 0.01
                width: parent.width
                wrapMode: Text.WordWrap
            }
        }

        Rectangle {
            id: radioBtnsRect
            Layout.minimumWidth: parent.width * 0.15
            Layout.minimumHeight: parent.height * 0.6
            color: "grey"

            border {
                color: "white"
            }

            ColumnLayout {
                anchors.fill: parent

                RadioButton {
                    id: easyLvlBtn
                    text: "Easy"
                    checked: false
                    onToggled: {
                        presenter.switchToEasyLevel();
                    }
                }

                RadioButton {
                    id: mediumLvlBtn
                    text: "Medium"
                    checked: true
                    onToggled: {
                        presenter.switchToMediumLevel();
                    }
                }

                RadioButton {
                    id: hardLvlBtn
                    text: "Hard"
                    checked: false
                    onToggled: {
                        presenter.switchToHardLevel();
                    }
                }
            }
        }

        Rectangle {
            id: statusRect
            Layout.minimumWidth: parent.width * 0.242
            Layout.minimumHeight: parent.height
            Layout.alignment: Qt.AlignRight
            color: "grey"

            border {
                color: "white"
            }

            Text {
                color: "white"

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

    RowLayout {
        id: wordLabelsLayout
        anchors.top: infoLevelsAndStatusLayout.bottom
        anchors.left: infoLevelsAndStatusLayout.left
        anchors.right: infoLevelsAndStatusLayout.right
        anchors.topMargin: parent.height * 0.025
        height: wordLabelsLayoutHeight

        Label {
            id: label1
            Layout.minimumWidth: parent.width * 0.25
            Layout.minimumHeight: parent.height
            color: "white"
            text: "Label "
        }

        Label {
            id: label2
            Layout.minimumWidth: parent.width * 0.25
            Layout.minimumHeight: parent.height
            color: "white"
            text: "area."
        }

        Label {
            id: label3
            Layout.minimumWidth: parent.width * 0.25
            Layout.minimumHeight: parent.height
            color: "white"
            text: "Under"
        }

        Label {
            id: label4
            Layout.minimumWidth: parent.width * 0.25
            Layout.minimumHeight: parent.height
            color: "white"
            text: "construction! "
        }
    }

    RowLayout {
        id: wordsEntryLayout
        anchors.top: wordLabelsLayout.bottom
        anchors.left: wordLabelsLayout.left
        anchors.right: wordLabelsLayout.right
        anchors.topMargin: parent.width * 0.01
        height: wordsEntryLayoutHeight

        TextField {
            id: firstWordTextField
            Layout.minimumWidth: parent.width * 0.49
            Layout.minimumHeight: parent.height
        }

        TextField {
            id: secondWordTextField
            Layout.minimumWidth: parent.width * 0.49
            Layout.minimumHeight: parent.height
            Layout.alignment: Qt.AlignRight
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
            text: "Submit"
            Layout.minimumWidth: bottomBtnsMinWidth
            onClicked: {
                var clearTextFields = presenter.handleSubmitRequest(firstWordTextField.text, secondWordTextField.text);
                if (clearTextFields) {
                    firstWordTextField.clear();
                    secondWordTextField.clear();
                }
            }
        }

        Button {
            id: helpBtn
            text: "Help"
            Layout.minimumWidth: bottomBtnsMinWidth

            onClicked: {
                presenter.switchToHelpPane();
            }
        }

        Button {
            id: resultsBtn
            text: "Show results"
            Layout.minimumWidth: bottomBtnsMinWidth
            onClicked: presenter.handleResultsRequest()
        }

        Button {
            id: quitBtn
            text: "Quit"
            Layout.minimumWidth: bottomBtnsMinWidth - parent.spacing * 0.25
            onClicked: Qt.quit()
        }
    }
}
