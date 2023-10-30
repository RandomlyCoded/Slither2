import QtQuick 2.15

import Editor 1.0

Circle {
    signal activated
    property string label
    property int textSize

    rad: 100
    exactly: true
    radX: 100
    radY: 50

    color: "green"

    Text {
        anchors.centerIn: parent
        font { pixelSize: textSize; }
        text: label
    }

    MouseArea {
        anchors.fill: parent
        onClicked: activated()
    }
}
