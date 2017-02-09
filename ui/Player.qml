import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
  property int seat
  property string name: view.players[seat].name
  property int chips: view.players[seat].chips
  property int chipsInPlay: view.players[seat].chipsInPlay
  property bool exists: view.players[seat].exists
  property bool acting: view.actingPlayerSeat == seat
  property string textColor: acting ? "red" : "white"

  id: playerContainer

  
  width: 50
  height: 50

  visible: exists
  color: "transparent"
  
  Text {
    id: playerName
    text: name
    color: textColor
  }
  Text {
    id: playerChips
    text: chips
    anchors.top: playerName.bottom
    color: textColor
  }
  Text {
    id: playerChipsInPlay
    text: chipsInPlay
    anchors.top: playerChips.bottom
    color: textColor
  }
}
