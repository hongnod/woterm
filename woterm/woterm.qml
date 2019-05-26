import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Imagine 2.4
import "layout"
import "control"

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    minimumWidth: 800
    minimumHeight: 600

    WoTheme {
        id:g_theme
    }

    menuBar: WoMenuBar {
        Menu {
            id: menu
            title: "sdfdsfdf"

            Action { text: qsTr("Tool Bar"); checkable: true }
            Action { text: qsTr("Side Bar"); checkable: true; checked: true }
            Action { text: qsTr("Status Bar"); checkable: true; checked: true }
            Menu {
                title: qsTr("Advanced")
                // ...
            }
        }
        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("Cu&t") }
            Action { text: qsTr("&Copy") }
            Action { text: qsTr("&Paste") }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About") }
        }
    }
}
