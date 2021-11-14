import QtQuick 2.15
import Slither 1.0

Rectangle {
    id: minimap
    property int r: 100
    width: r * 2
    height: width
    radius: r

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
        Rectangle {
            width: radius * 2
            height: radius * 2
            radius: modelData.size + .5

            x: modelData.position.x + minimap.width/2
            y: modelData.position.y + minimap.height/2
            color: modelData.skinAt(0)
        }
    }
}
