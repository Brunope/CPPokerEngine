import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
  id: boardContainer

  width: 400
  height: 100
  color: "transparent"

  Card {
    id: c0
    anchors.right: c1.left
    anchors.rightMargin: 10
    visible: view.board.length > 0
    str: cardStr(0)
   }

  Card {
    id: c1
    anchors.right: c2.left
    anchors.rightMargin: 10
    visible: view.board.length > 1
    str: cardStr(1)
  }

  Card {
    id: c2
    anchors.horizontalCenter: parent.horizontalCenter
    str: cardStr(2)
    visible: view.board.length > 2
  }

  Card {
    id: c3
    anchors.left: c2.right
    anchors.leftMargin: 10
    visible: view.board.length > 3
    str: cardStr(3)
  }

  Card {
    id: c4
    anchors.left: c3.right
    anchors.leftMargin: 10
    visible: view.board.length > 4
    str: cardStr(4)
  }

  function cardStr(index) {
    return view.board.length > index ? view.board[index].str : "back"
  }
}
