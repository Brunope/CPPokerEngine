import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
//import poker 1.0

Window {
  id: root
  visible: true
  width: 300
  height: 200
  color: "gray"

  signal startGame(string msg)

  Text {
    id: recvSigText
    text: "received signal text"
    font.pointSize: 20
    anchors.centerIn: parent
  }

  Button {
    id: startGameButton    
    text: "start game"

    anchors.bottom: recvSigText.top
    anchors.horizontalCenter: parent.horizontalCenter

    onClicked: {
      root.startGame("dummy msg")
    }
  }

  Player {
    id: player0
    seat: 0
    anchors.top: parent.top
  }

  Player {
    id: player1
    seat: 1
    anchors.left: player0.right
  }

  Player {
    id: player5
    anchors.left: player1.right
    seat: 5
  }

  Rectangle {
    id: numHandsContainer
    anchors.top: recvSigText.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    width: 100
    height: 20
    Text {
      text: "hand # " + view.numHands
    }
  }

  Connections {
    id: cppConnection
    target: listener
    ignoreUnknownSignals: true
    onGameStart: {
      recvSigText.text = sig_text
    }
    onPlayerJoin: {
      recvSigText.text = sig_player.getName()
    }
    onPlayerLeave: {
      recvSigText.text = sig_text
    }
    onHandStart: {
      recvSigText.text = sig_text
    }
    onDeal: {
      recvSigText.text = sig_text
    }
    onPlayerAction: {
      recvSigText.text = sig_text
    }
    onShowCards: {
      recvSigText.text = sig_text
    }
    onShowdown: {
      recvSigText.text = sig_text
    }
    onPotWin: {
      recvSigText.text = sig_text
    }
  }
}

