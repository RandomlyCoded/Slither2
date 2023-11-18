import QtQuick 2.15
import Slither 1.0

Rectangle {
    id: minimap

    property int r: 1

    visible: showMinimap

    width: r * 2 * Playground.size
    height: width
    radius: r * Playground.size

    color: "#afafaf7f"

    Repeater {
        model: Playground.energyPearls
        Rectangle {
            width: 2
            height: 2
            radius: 1
            x: modelData.position.x + minimap.width/2
            y: modelData.position.y + minimap.height/2
            color: modelData.color
        }
    }

    Repeater {
        model: Playground.snakes
        Image {
            source: "qrc:/assets/Minimap-icon.svg"
            x: modelData.position.x + minimap.width/2
            y: modelData.position.y + minimap.height/2
            width: modelData.size + 3
            height: modelData.size + 3
        }
    }
}
