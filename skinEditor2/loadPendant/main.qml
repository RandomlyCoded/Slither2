import QtQuick 2.15
import QtQuick.Window 2.15

import PendantLoader 1.0

Window {
    id: mainWnd
    width: 640
    height: 480
    visible: true
    flags: Qt.CustomContextMenu | Qt.FramelessWindowHint
    color: "#1f1f1f"

    Loader {
        id: mainLoader
    }

    Column {
        spacing: 5

        Rectangle {
            width: chooser.width + 4
            height: chooser.height + 4
            color: "gray"

            border { color: "black"; width: 1 }

            Chooser {
                id: chooser
            }
        }

        Rectangle {
            x: mainWnd.width/2 - width/2
            width: 200
            height: 200
            color: "gray"

            border { color: "black"; width: 1 }

            Image {
                id: previewImg
                anchors.centerIn: parent
                source: mainLoader.preview
                sourceSize: Qt.size(parent.width, parent.height)
            }
        }
    }
}
