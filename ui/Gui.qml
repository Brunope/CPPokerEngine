import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0

Window {
  id: root
  visible: true
  width: 300
  height: 200
  color: "gray"

  signal startGame(string msg)

  Text {
    id: recvSigText
    text: qsTr("received signal text")
    font.pointSize: 20
    anchors.centerIn: parent
  }

  Button {
    id: startGameButton    
    text: qsTr("start game")

    anchors.bottom: recvSigText.top
    anchors.horizontalCenter: parent.horizontalCenter

    onClicked: {
      root.startGame("dummy msg")
    }
  }

  Connections {
    id: cppConnection
    target:listener
    ignoreUnknownSignals: true
    onGameStart: {
      recvSigText.text = sig_text
    }
    onPlayerJoin: {
      recvSigText.text = sig_text
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

