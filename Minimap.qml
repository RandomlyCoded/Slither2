import QtQuick 2.15
import Slither 1.0

Rectangle {
    id: minimap
    property int r: 1
    width: r * 2 * playground.size
    height: width
    radius: r * playground.size

    color: "#afafaf7f"

    Repeater {
        model: playground.energyPearls
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
        model: playground.snakes
        Image {
            source: ":/assets/assets/Minimap-icon.svg"
            x: modelData.position.x + minimap.width/2
            y: modelData.position.y + minimap.height/2
            width: modelData.size + 100
            height: modelData.size + 100
        }
    }
}
