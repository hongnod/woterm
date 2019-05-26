import QtQuick 2.2
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Imagine 2.4
import "layout"

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    minimumWidth: 800
    minimumHeight: 600
    Imagine.path: skinPath

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("&New...") }
            Action { text: qsTr("&Open...") }
            Action { text: qsTr("&Save") }
            Action { text: qsTr("Save &As...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
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
