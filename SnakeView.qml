import QtQuick 2.15

import QtQuick.Shapes 1.12

import Slither 1.0

Item {
    property Snake currentSnake

    x: playgroundView.center.x - (width /2 * zoom)
    y: playgroundView.center.y - height/2

    /*   Repeater {
        id: snakeView

        model: snake.segments

        Rectangle {
            id: segmentView

            readonly property int index: model.index
            readonly property point segment: modelData

            x: playgroundView.center.x + segment.x * zoom - width/2
            y: playgroundView.center.y + segment.y * zoom - height/2
            z: snakeView.count - model.index

            border { width: 1; color: "black" }

            property int speeding: mouseArea.pressed
            property color normalColor: ["red", "#ff7f00", "yellow", "limegreen", "cyan", "blue", "#6a006a"][model.index % 7] // snake.skin[model.index]
            property color speedColor: ["#ff5555", "orange", "#ffff66", "#55ff55", "#00ffff", "#5555ff", "#7f007f"][model.index % 7] // // snake.speedSkin[model.index]
            // [["red", "#ff5555"], ["#ff7f00", "orange"], ["yellow", "#ffff66"], ["limegreen", "#55ff55"], ["cyan", "#00ffff"], ["blue", "#5555ff"], ["#6a006a", "#7f007f"]][model.index % 7][speeding]
            // REGENBOGENSCHLANGE

            color: [currentSnake.skinAt(index), speedColor][speeding]

            property bool isLast: segmentView.index === snakeView.count - 1
            Behavior on radius {
                animation: NumberAnimation {
                    duration: 1000
                    from: 0
                    to: 0
                }
            }

            radius: {
                let r = snake.size * zoom; //snake.size;

                if (isLast) {
                    r *= Math.min(1 + snake.load, 1);
                }


                return r;
            }

            width: radius * 2
            height: radius * 2

            Eyes {
    //                    rotation: Math.atan2(snake.direction.y, snake.direction.x)
                anchors.horizontalCenter: parent.horizontalCenter
                viewAngle: Math.atan2(snake.direction.y, snake.direction.x)
                radius: zoom/2 * currentSnake.size
                visible: segmentView.index === 0
            }
        }
    }*/

    Shape {
        width: parent.width
        height: parent.height
        ShapePath {
            strokeWidth: currentSnake.size * zoom
            strokeColor: "black"
            fillColor: "transparent"

            startX: currentSnake.position.x * zoom
            startY: currentSnake.position.y * zoom
            PathPolyline {
                path: playground.zoomOut(currentSnake.segments, zoom)
            }
        }
    }

    Rectangle {
        id: head
        x: currentSnake.position.x * zoom - radius
        y: currentSnake.position.y * zoom - radius
        z: 2
        width: currentSnake.size * zoom
        height: width
        radius: height/2
        color: currentSnake.skinAt(0)

        Eyes {
            id: eyes
            anchors.horizontalCenter: parent.horizontalCenter
            viewAngle: Math.atan2(snake.direction.y, snake.direction.x)
            radius: (zoom/2 * currentSnake.size)/2
        }
    }

    Rectangle {
        id: end
        x: currentSnake.lastSegment.x * zoom - radius
        y: currentSnake.lastSegment.y * zoom - radius
        z: 1
        width: currentSnake.size * zoom
        height: width
        radius: height/2
        color: currentSnake.skinLast()
    }
}
