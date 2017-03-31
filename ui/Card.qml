import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
  id: cardContainer

  property string str: "null"
  property real scale: 1.0
  property real textSize: scale * 25 + 2
  property int fontPixSize: width / 1.5

  color: "white"
  width: 55 * scale
  height: 75 * scale
  radius: 3
  border.color: "black"
  anchors.margins: 1
  
  Image {
    id: cardImg
    anchors.fill: parent
    // TODO: conditionally load this when str != null
    // instead of looking for a 'null.png' that's not needed
    source: "img/back.png"
    mipmap: true
    fillMode: Image.Stretch
    visible: str == "back"
  }

  Text {
    id: cardText
    text: getCardText(str)
    color: getCardColor(str)
    // font.pointSize: textSize
    font.pixelSize: fontPixSize
    font.family: root.fontFamily
    anchors.centerIn: parent
    visible: str != "back"
  }

  function getCardText(cardStr) {
    var rank = cardStr.charAt(0);
    var suit = cardStr.charAt(1);
    var newSuit = "";
    if (suit == "c") {
      newSuit = "\u2663";
    } else if (suit == "h") {
      newSuit = "\u2665";
    } else if (suit == "s") {
      newSuit = "\u2660";
    } else if (suit  == "d") {
      newSuit = "\u2666";
    }
    return rank + newSuit;
  }

  function getCardColor(cardStr) {
    var suit = cardStr.charAt(1);
    if (suit == "c" || suit == "s") {
      return "black";
    }
    return "red";
  }
}
