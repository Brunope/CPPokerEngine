import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import poker 1.0

Window {
  id: root
  visible: true
  width: 640
  height: 480
  color: "gray"

  signal startGame(string msg)

  Rectangle {
    id: recvSigContainer
    width: 202
    height: 102
    border.color: "black"
    border.width: 1

    anchors.bottom: parent.bottom
    anchors.left: parent.left

    Flickable {
      id: recvSigScroll
      width: 200
      height: 100
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
        text: ""
        font.pointSize: 12
      }
    }
  }

  Button {
    id: startGameButton    
    visible: true
    text: "start game"
    anchors.verticalCenter: parent.verticalCenter
    anchors.horizontalCenter: parent.horizontalCenter

    onClicked: {
      root.startGame("dummy msg")
      visible = false
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
    anchors.left: parent.left
    anchors.bottom: recvSigContainer.top
    height: 20
    border.color: "black"
    border.width: 1
    Text {
      text: "hand # " + view.numHands
      leftPadding: 10
    }
  }

  Connections {
    id: cppConnection
    target: listener
    ignoreUnknownSignals: true
    onGameStart: {
      recvSigText.text = recvSigText.text + "\n" + sig_text
    }
    onPlayerJoin: {
      recvSigText.text = recvSigText.text + "\n" + sig_text.getName()
    }
    onPlayerLeave: {
      recvSigText.text = recvSigText.text + "\n" + sig_text
    }
    onHandStart: {
      recvSigText.text = recvSigText.text + "\n" + sig_text
    }
    onDeal: {
      recvSigText.text = recvSigText.text + "\n" + sig_text
    }
    onPlayerAction: {
      recvSigText.text = recvSigText.text + "\n" + sig_text
    }
    onShowCards: {
      recvSigText.text = recvSigText.text + "\n" + sig_text
    }
    onShowdown: {
      recvSigText.text = recvSigText.text + "\n" + sig_text
    }
    onPotWin: {
      recvSigText.text = recvSigText.text + "\n" + sig_text
    }
  }
}

