import QtQuick 2.7
import QtQuick.Controls 2.2
import Animations 1.0

ComboBox {
    id: dropdown

    property var dataModel: null

    property string dropdownToolTip: ""
    property string noItemSelectedText: ""

    property int selectedIndex: -1 // don't modify this property manually, it is only meant to be used by the onItemChanged() slot

    property bool dropdownEnabled: false    // use this property and not enabled to ensure the dropdown can be altered

    signal itemChanged

    enabled: dropdownEnabled && dataModel !== null && count > 0

    property bool dropdownOpened: dropdown.popup.opened // don't modify manually, it is used to ensure that the language is restored if popup is closed without actually selecting

    Text {
        z:1
        text: noItemSelectedText

        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        visible: currentIndex === -1
    }

    delegate: ItemDelegate {
        id: dropdownDelegate

        width: parent.width
        height: enabled ? implicitHeight : 0

        text: dataModel[index]
        font.bold: index === selectedIndex

        onClicked: {
            if (index !== currentIndex) {
                selectedIndex = index;
                itemChanged();
            }
        }
    }

    Keys.onUpPressed: {
        if (selectedIndex == 0) {
            selectedIndex = dataModel.length-1;
        } else {
            --selectedIndex;
        }
    }

    Keys.onDownPressed: {
        if (selectedIndex == dataModel.length-1) {
            selectedIndex = 0;
        } else {
            ++selectedIndex;
        }
    }

    Keys.onReturnPressed: itemChanged()
    Keys.onEnterPressed: itemChanged()

    ToolTip.visible: hovered && dropdownToolTip !== ""
    ToolTip.text: dropdownToolTip
    ToolTip.delay: Animations.toolTipDelay
    ToolTip.timeout: Animations.toolTipTimeout

    editable: false
    model: dataModel

    onCurrentIndexChanged: selectedIndex = currentIndex

    onDropdownOpenedChanged: {
        if (!dropdownOpened && selectedIndex !== currentIndex) {
            selectedIndex = currentIndex;
        }
    }
}
