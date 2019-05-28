import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Imagine 2.4
import "layout"
import "control"
import WoItem 1.0
import WoSetting 1.0


ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    minimumWidth: 800
    minimumHeight: 600

    Component.onCompleted: {
    }

    WoTheme {
        id:g_theme
    }

    menuBar: WoMenuBar {
        Menu {
            title: qsTr("Help")
            Action { text: qsTr("About") }
        }
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: qsTr("New")
                onClicked: {

                }
            }
            ToolButton {
                text: qsTr("Connect")
                implicitWidth: 60
                onClicked: {

                }
                Button{
                    width: 20
                    anchors.right: parent.right
                    text : "Drop"
                }
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }
    WoItem {
        anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            border.width: 3
        }
    }
}
