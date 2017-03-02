import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
  property int seat
  property string name: view.players[seat].name
  property int chips: view.players[seat].chips
  property int chipsInPlay: view.players[seat].chipsInPlay
  property bool live: view.players[seat].live
  property bool exists: view.players[seat].exists
  property bool acting: view.actingPlayerSeat == seat
  property string textColor: getColor()
  property string facing: "top"
  property string card0str: "back"
  property string card1str: "back"

  id: playerContainer
  
  width: 75
  height: 75

  visible: exists
  color: "transparent"

  //border.color: "red"

  Text {
    id: playerName
    anchors.top: cardContainer.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    text: name
    color: textColor
  }

  Rectangle {
    id: cardContainer
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter

    width: c0.width + c1.width
    height: c0.height
    color: "transparent"

    Card {
      id: c0
      anchors.left: parent.left
      str: card0str
      scale: 0.5
      visible: live
    }
    Card {
      id: c1
      anchors.left: c0.right
      str: card1str
      scale: 0.5
      visible: live
    }
  }

  Text {
    id: playerChips
    text: chips
    color: textColor
    anchors.top: playerName.bottom
    anchors.horizontalCenter: parent.horizontalCenter
  }

  Rectangle {
    id: betContainer

  }
  
  Text {
    id: playerChipsInPlay
    anchors.top: playerChips.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    text: chipsInPlay
    color: textColor
  }

  function getColor() {
    if (live) {
      if (acting) {
        return "red"
      } else {
        return "white"
      }
    }
    return "gray"
  }
}
