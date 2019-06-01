import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


TabView {
    style: TabViewStyle {
        tabOverlap: 16
        frameOverlap: 4
        tabsMovable: true

        frame: Rectangle {
            gradient: Gradient {
                GradientStop { color: "#e5e5e5" ; position: 0 }
                GradientStop { color: "#e0e0e0" ; position: 1 }
            }
            border.color: "#898989"
            Rectangle { anchors.fill: parent ; anchors.margins: 1 ; border.color: "white" ; color: "transparent" }
        }
        tab: Item {
            property int totalOverlap: tabOverlap * (control.count - 1)
            implicitWidth: Math.max(text.width + 4, image.sourceSize.width)
            implicitHeight: image.sourceSize.height

            Component.onCompleted: {
                console.log("implicitWidth:", styleData, "implicitHeight", totalOverlap)
            }

            BorderImage {
                id: image
                anchors.fill: parent
                source: styleData.selected ? "qrc:/images/tab_selected.png" : "qrc:/images/tab.png"
                border.left: 30
                smooth: false
                border.right: 30
            }

            Text {
                id: text
                anchors.centerIn: parent
                text: styleData.title
            }
        }
        leftCorner: Item { implicitWidth: 12 }
    }
}


