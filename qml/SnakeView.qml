import QtQuick 2.15

import QtQuick.Shapes 1.12

import Slither

Item {
    property Snake currentSnake

    x: playgroundView.center.x - (width /2 * zoom)
    y: playgroundView.center.y - height/2

    Shape {
        antialiasing: true
        width: parent.width
        height: parent.height
        ShapePath {
            strokeWidth: currentSnake.size * zoom * 2
            strokeColor: currentSnake.boosting ? "#80ffffff" : "#80000000"
            fillColor: "transparent"

            startX: currentSnake.position.x * zoom
            startY: currentSnake.position.y * zoom
            PathPolyline {
                path: Playground.scaleToZoom(currentSnake.segments, zoom)
            }
        }
    }

    Rectangle {
        id: head
        x: currentSnake.position.x * zoom - radius/2
        y: currentSnake.position.y * zoom - radius/2
        z: 2
        width: currentSnake.size * zoom * 2
        height: width
        radius: height
        color: currentSnake.skinAt(0)

        Eyes {
            id: eyes
            anchors.centerIn: parent
            rotation: Math.atan2(currentSnake.direction.y, currentSnake.direction.x) / Math.PI * 180 + 90
//            onRotationChanged: console.info(rotation, x, y)
            radius: zoom/2.25 * currentSnake.size
        }
    }

    Rectangle {
        id: end

        x: currentSnake.lastSegment.x * zoom - radius/2
        y: currentSnake.lastSegment.y * zoom - radius/2
        z: 1

        width: currentSnake.size * zoom * 2
        height: width
        radius: height

        color: currentSnake.skinLast()
    }

    Rectangle {
        id: dest

        visible: showDestinations

        width: zoom
        height: width
        radius: width/2

        color: "limegreen"

        x: currentSnake.destination.x * zoom - radius
        y: currentSnake.destination.y * zoom - radius
        z: 100
    }
}
