#ifndef QHUMANAGENT_H_
#define QHUMANAGENT_H_

#include <QObject>
#include "QCard.h"
#include "Agent.h"

class QHumanAgent : public QObject, public Agent {
  Q_OBJECT
  Q_PROPERTY(int seat READ seat NOTIFY seatChanged)
  Q_PROPERTY(QCard* card0 READ card0 NOTIFY holeCardsChanged)
  Q_PROPERTY(QCard* card1 READ card1 NOTIFY holeCardsChanged)
  

public:
  QHumanAgent();
  ~QHumanAgent() { std::cout << "destructed qhumanagent" << std::endl; }
  void receiveHoleCards(const std::pair<Card, Card> hc);
  Action act(const GameView &view);
  void receiveHandHistory(const HandHistory &history);

  int seat() const;
  QCard* card0();
  QCard* card1();
  
signals:
  void needAction();
  void seatChanged();
  void holeCardsChanged();

public slots:
  void doAction(int action_type, int amount);

private:
  std::atomic<bool> got_action_;
  std::atomic<uint8_t> to_do_action_type_;
  std::atomic<uint32_t> to_do_action_amount_;

  int seat_;
  QCard hc0_;
  QCard hc1_;
};

#endif  // QHUMANAGENT_H_
