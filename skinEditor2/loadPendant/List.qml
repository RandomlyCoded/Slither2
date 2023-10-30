import QtQuick 2.15

import PendantLoader 1.0

Column {
    property int parentWidth
    Repeater {
        model: mainLoader.count()

        Rectangle {
            width: parentWidth + 4
            height: text.height + 4

            border.width: 1

            Text {
                id: text
                x: 2
                y: 2

                font.pixelSize: 12
                text: mainLoader.getPossiblePendant(index)
            }

            MouseArea {
                anchors.fill: parent
                onClicked: mainLoader.getPendant(text.text)
            }
        }
    }
    Text {
        id: txt
        font.pixelSize: 12
    }

    property int explicitHeight: mainLoader.count() * (txt.height + 4)* visible
}
