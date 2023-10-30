import QtQuick 2.15

import PendantLoader 1.0

Item {
    width: 600
    height: current.height + choosePendantList.explicitHeight

    Text {
        id: current

        x: 2
        y: 2

        color: "black"

        font.pixelSize: 15
        text: "Choose pendant"
    }

    property string name: "baum"
    property bool choose: false

    Rectangle {
        id: openButton
        height: parent.height
        width: 17

        x: parent.width - width + 2
        y: 2

        color: "lightgray"

        border { color: "black"; width: 1; }

        Image {
            x: 1
            source: "qrc:/OPEN.png"
        }

        MouseArea {
            anchors.fill: parent

            onClicked: choose = !choose
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: choose = !choose
    }

    List {
        parentWidth: parent.width - 5 - 2
        id: choosePendantList
        x: 5
        y: 20
        visible: choose
    }
}
