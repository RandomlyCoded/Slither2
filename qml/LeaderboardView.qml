import QtQuick

import Slither

Rectangle {
    id: leaderboardBorder
    visible: showLeaderboard

    function reload() { rep.model = Leaderboard.leaderboard; }

    width: 200
    height: topLine.height * 12 // its 12 lines: top, bottom and 10 snakes.

    border.color: "black"
    color: Qt.rgba(0, 0, 0, .15)

    Column {
        width: parent.width

        Text {
            id: topLine
            anchors.horizontalCenter: parent.horizontalCenter
            text: "LEADERBOARD"
        }

        Repeater {
            id: rep
//            model: Leaderboard.leaderboard

            Text {
                color: modelData.skinAt(0)

                font.bold: !index
                font.family: "Source Code Pro"
                text: " #" + (index + 1) + ": " + (index < 9 ? " " : "") + modelData.name + "\t" + modelData.length
            }
        }

        Text {
            id: bottomLine
            text: " "
        }
    }
}
