import QtQuick 2.15
import QtQuick.Controls 2.15

import Slither

Rectangle {
    anchors.fill: parent
    color: "#7f7f7f7f"

    Column {
        x: 10
        y: 40
        spacing: 2

        function slidingAvg(buffer, delay) {
            buffer.push(delay);

            while(buffer.length > (fps * 2))
                buffer.shift();

            const sum = buffer.reduce((a, b) => {
                return a + b;
            });

            const avg = sum / buffer.length;

            return avg;
        }
        Text {

            id: fpsInfo
            //           ---get the ms---
            property var fpsBuffer: []
            property var frameRateBuffer: []

            text: `${(1000/parent.slidingAvg(fpsBuffer, frameDelay * 1000)).toFixed(1)}/${fps} (${parent.slidingAvg(frameRateBuffer, frameDelay * 1000).toFixed(1)} ms per frame)`
        }

        Text {
            id: tickInfo
            property var tickDelayBuffer: []
            text: `${parent.slidingAvg(tickDelayBuffer, Playground.tickDelay).toFixed(1)} ms per tick`
        }

        Text {
            text: "-------------------\tgeneral info"
        }

        Text {
            id: pearlAmount
            text: "amount of energyPearls: " + Playground.pearlAmount
        }

        Text {
            id: snakeAmount
            text: "amount of snakes: "+ Playground.snakeCount
        }

        Text {
            id: playgroundSizeInfo
            text: "size of Playground: " + Playground.size
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
            text: "total size of all snakes: " + Playground.totalSnakesSize
        }

        Text {
            id: nowPlayerInformation
            text: "-------------------\t info about you"
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
            text: "index of your snake: " + Playground.snakes.indexOf(snake)
        }
    }

    Column {
        id: settings
        x: parent.width - fpsSlider.width - 40
        y: 10

        spacing: 5

        Column {
            Text {
                text: "FPS: " + fps
            }

            Slider {
                id: fpsSlider

                stepSize: 1
                from: 8
                to: 120
                value: 30

                onMoved: fps = value
            }
        }

        Column { // idk doesn't work :(
            Text {
                text: "background animation\nspeed: " + backgroundAnimationSpeed
            }

            Slider {
                id: backgroundAnimationSpeedSlider // ik a very short name XD

                stepSize: 100
                from: 200
                to: 200000 // 200k
                value: 20000 // 20k

                onMoved: { backgroundAnimationSpeed = value; bgAnimation.durationChanged(value); }
            }
        }

        Row {
            Text {
                text: "show leaderboard: "
            }

            CheckBox {
                checked: showLeaderboard
                onToggled: showLeaderboard ^= 1
            }
        }

        Row {
            Text {
                text: "show minimap: "
            }

            CheckBox {
                checked: showMinimap
                onToggled: showMinimap ^= 1
            }
        }

        Row {
            Text {
                text: "show destinations: "
            }

            CheckBox {
                checked: showDestinations
                onToggled: showDestinations ^= 1
            }
        }
    }
}
