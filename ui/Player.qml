import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
  property int seat
  property string name: view.players[seat].name
  property int chips: view.players[seat].chips
  property int chipsInPlay: view.players[seat].chipsInPlay
  property bool exists: view.players[seat].exists

  id: playerContainer
  visible: exists

  width: 50
  height: 50
  
  Text {
    id: playerName
    text: name
  }
  Text {
    id: playerChips
    text: chips
    anchors.top: playerName.bottom
  }
  Text {
    id: playerChipsInPlay
    text: chipsInPlay
    anchors.top: playerChips.bottom
  }
}
