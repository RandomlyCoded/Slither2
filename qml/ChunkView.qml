import QtQuick

import Slither

Rectangle {
    property Chunk chunk

//    visible: chunk.coords === Qt.point(0, 0)

    width: chunk.chunkSize() * zoom
    height: chunk.chunkSize() * zoom

    x: middle.x + (chunk.coords.x * chunk.chunkSize() * zoom)
    y: middle.y + (chunk.coords.y * chunk.chunkSize() * zoom)

    color: "transparent"
    border.color: "white"
    border.width: 2

    Connections {
        target: chunk

        function onEnergyPearlsChanged() {
            onModelChanged: console.log("pearls updated for chunk", chunk.coords)

            energyPearlView.model = chunk.energyPearls
        }
    }

    Repeater {
        id: energyPearlView
        model: chunk.energyPearls

        onModelChanged: console.log("pearls updated for chunk", chunk.coords)

        Rectangle {
            readonly property var pearl: model.data

            color: pearl.color
            radius: zoom * pearl.amount
            width: radius * 2
            height: radius * 2

            x: playgroundView.center.x + zoom * pearl.position.x - width/2 - parent.x
            y: playgroundView.center.y + zoom * pearl.position.y - height/2 - parent.y
        }
    }
}
