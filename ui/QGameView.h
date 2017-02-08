#ifndef QGAMEVIEW_H_
#define QGAMEVIEW_H_

#include <QObject>
#include <QList>
#include <QVariant>
#include <QQmlListProperty>
#include "GameView.h"
#include "QPlayer.h"
#include <iostream>

class QGameView : public QObject {
  Q_OBJECT
  Q_PROPERTY(QQmlListProperty<QPlayer> players READ players NOTIFY playersChanged)
  Q_PROPERTY(int numHands READ numHands NOTIFY numHandsChanged)

public:
  QGameView();
  ~QGameView();

  // copy data from view to this, caller retains ownership of view
  void copyFrom(const GameView *view);
  void copyFromQ(const QGameView *view);
  
  QQmlListProperty<QPlayer> players();
  QPlayer *player(int index) const;
  int playerCount() const;
  void setPlayer(size_t seat, QPlayer player);

  int numHands() const;

signals:
  void playersChanged();
  void numHandsChanged();

private:
  void initQPlayers();
  QList<QPlayer *> players_;
  int num_hands_;
};

//Q_DECLARE_METATYPE(QGameView);

#endif  // QGAMEVIEW_H_
