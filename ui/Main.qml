import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import poker 1.0

Window {
  id: root
  visible: true
  width: 640
  height: 480
  Image {
    id: bgImage
    anchors.fill: parent
    source: "background.png"
    fillMode: Image.Tile
  }

  signal startGame(string msg)
  signal sendAction(int type, int amount)

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

  Board {
    id: board
    anchors.verticalCenter: playerRing.verticalCenter
    anchors.horizontalCenter: playerRing.horizontalCenter
    anchors.verticalCenterOffset: 15
  }

  Text {
    id: pot
    anchors.horizontalCenter: board.horizontalCenter
    anchors.bottom: board.top
    anchors.margins: 5
    visible: view.pot > 0
    font.pointSize: 16
    text: view.pot
    color: "white"

  }
  
  PlayerRing {
    id: playerRing
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 10
    //border.color: "red"
  }

  ChatBox {
    id: chat
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.top: playerRing.bottom
    anchors.topMargin: 2
  }
  
  Rectangle {
    id: numHandsContainer
    anchors.left: parent.left
    anchors.bottom: chat.top
    height: 20
    Text {
      text: "hand # " + view.numHands
      color: "white"
      leftPadding: 10
    }
  }

  ActionInterface {
    id: actionInterface
    anchors.bottom: parent.bottom
    anchors.top: playerRing.bottom
    anchors.left: chat.right
    anchors.right: parent.right
  }

  Connections {
    id: humanConnection
    target: human
    ignoreUnknownSignals: true
    onHoleCardsChanged: {
      // gross
      eval("playerRing.player" + human.seat).card0str = human.card0.str
      eval("playerRing.player" + human.seat).card1str = human.card1.str
    }
  }

  Connections {
    id: cppConnection
    target: listener
    ignoreUnknownSignals: true
    onGameStart: {
      appendChatText(sig_text);
    }
    onPlayerJoin: {
      appendChatText(sig_text + " joined")
    }
    onPlayerLeave: {
      appendChatText(sig_text)
    }
    onHandStart: {
      appendChatText(sig_text)
    }
    onDeal: {
      appendChatText(sig_text)
    }
    onPlayerAction: {
      appendChatText(sig_text);
    }
    onShowCards: {
      appendChatText(sig_text)
    }
    onShowdown: {
      appendChatText(sig_text)
    }
    onPotWin: {
      appendChatText(sig_text)
      pot.anchors.left = root.left
      pot.anchors.bottom = root.bottom
      console.log("new pot win coords " + pot.x + ", " + pot.y);
    }
  }

  function appendChatText(text) {
    if (chat.textItem.lineCount > chat.maxLines) {
      chat.text = chat.text.substring(chat.text.indexOf("\n") + 1) + text + "\n"
    }
    else {
      chat.text = chat.text + text + "\n"
    }
  }

}

