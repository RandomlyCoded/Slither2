import QtQuick 2.15

Rectangle {
    anchors.fill: parent
    color: "#7f7f7f7f"

    Column {
        x: 10
        y: 40
        spacing: 2

        Text {
            id: pearlAmount
            text: "amount of energyPearls: " + playground.pearlAmount
        }

        Text {
            id: snakeAmount
            text: "amount of snakes: "+ playground.snakeCount
        }

        Text {
            id: playgroundSizeInfo
            text: "size of Playground: " + playground.size
        }

        Text {
            id: zoomInfo
            text: "zoom: " + zoom
        }

        Text {
            id: colorInfo
            text: "Playground color: " + playgroundView.color
        }

        Text {
            id: totalSnakesSize
            text: "total size of all snakes: " + playground.totalSnakesSize
        }

        Text {
            id: nowPlayerInformation
            text: "-------------------\t infos about you"
        }

        Text {
            id: playerSizeInfo
            text: "your size: " + snake.size
        }

        Text {
            id: positionInfo
            text: "your position: " + snake.positionInfo
        }

        Text {
            id: speedInfo
            text: "your speed: " + snake.speed
        }

        Text {
            id: isSnakeLiving
            text: "your snake lives: " + snake.isAlive
        }

        Text {
            id: positionInSnakesList
            text: "index of your snake: " + playground.snakes.indexOf(snake)
        }
    }
}
