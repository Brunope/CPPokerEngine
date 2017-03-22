#ifndef QGAMEVIEW_H_
#define QGAMEVIEW_H_

#include <QObject>
#include <QList>
#include <QVariant>
#include <QQmlListProperty>
#include <memory>
#include "GameView.h"
#include "QPlayer.h"
#include "QCard.h"
#include <iostream>

class QGameView : public QObject {
  Q_OBJECT
  Q_PROPERTY(QQmlListProperty<QPlayer> players READ players NOTIFY playersChanged)
  Q_PROPERTY(QQmlListProperty<QCard> board READ board NOTIFY boardChanged)
  Q_PROPERTY(int numHands READ numHands NOTIFY numHandsChanged)
  Q_PROPERTY(int actingPlayerSeat READ actingPlayerSeat NOTIFY actingPlayerSeatChanged)
  Q_PROPERTY(int pot READ pot NOTIFY potChanged)
  Q_PROPERTY(int currentBet READ currentBet NOTIFY currentBetChanged)
  Q_PROPERTY(int currentRaiseBy READ currentRaiseBy NOTIFY currentRaiseByChanged)
    

public:
  QGameView();
  ~QGameView();

  void copyFrom(std::shared_ptr<const GameView> view);
  void copyFromQ(const QGameView *view);
  
  QQmlListProperty<QPlayer> players();
  QPlayer *player(int index) const;
  int playerCount() const;
  void setPlayer(size_t seat, QPlayer player);

  QQmlListProperty<QCard> board();
  QCard *card(int index) const;
  int cardCount() const;
  //  void setCard() const;
  
  int numHands() const;
  int actingPlayerSeat() const;
  int pot() const;
  int currentBet() const;
  int currentRaiseBy() const;

signals:
  void playersChanged();
  void boardChanged();
  void numHandsChanged();
  void actingPlayerSeatChanged();
  void potChanged();
  void currentBetChanged();
  void currentRaiseByChanged();

private:
  void initQPlayers();
  QList<QPlayer *> players_;
  QList<QCard *> board_;
  QCard card_mem_[5];
  int num_hands_;
  int acting_player_seat_;
  int pot_;
  int current_bet_;
  int current_raise_by_;
};

//Q_DECLARE_METATYPE(QGameView);

#endif  // QGAMEVIEW_H_
