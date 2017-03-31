import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
  property string text: ""
  property int maxLines: 20
  property int fontPixSize: root.fontPixSize
  
  property alias textItem: recvSigText
  
  id: recvSigContainer
  width: 300
  height: 100

  border.color: "white"
  border.width: 1
  color: "transparent"
  
  Flickable {
    id: recvSigScroll
    width: parent.width
    height: parent.height
    contentWidth: recvSigText.width
    contentHeight: recvSigText.height
    contentY: contentHeight - height
    clip: true
    leftMargin: 10
    ScrollBar.vertical: ScrollBar {
      id: recvSigScrollBar
      parent: recvSigScroll.parent
      anchors.top: recvSigScroll.top
      anchors.left: recvSigScroll.right
      anchors.bottom: recvSigScroll.bottom
    }
    Text {
      id: recvSigText
      text: recvSigContainer.text
      color: "white"
      // font.pointSize: 12
      font.pixelSize: fontPixSize
      font.family: root.fontFamily
    }
  }
}
