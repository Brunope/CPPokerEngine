#ifndef QEVENTLISTENER_H_
#define QEVENTLISTENER_H_

#include <QObject>
#include <QString>
#include <QMap>
#include <memory>
#include "QPlayer.h"
#include "QGameView.h"
#include "IEventListener.h"

/**
 * Once registered in a Game via Game::addEventListener, QtEventListener
 * emits signals as the Game's state changes.
 */
class QEventListener : public QObject, public IEventListener {
  Q_OBJECT

public:
  QEventListener();
  ~QEventListener();
  void onViewChanged(std::shared_ptr<const GameView> view);
  void onGameStart(std::shared_ptr<const GameView> view);
  void onGameEnd();
  void onPlayerJoin(Player player);
  void onPlayerLeave(Player player);
  void onHandStart(long handNum, std::shared_ptr<const GameView> view);
  void onDeal(STREET street);
  void onPlayerAction(Action action);
  void onShowCards(std::pair<Card, Card> cards, Player player);
  void onShowdown(Hand best_hand, Player player);
  void onPotWin(uint32_t pot, Player player);

signals:
  void gameStart(const QString &sig_text);
  void gameEnd(const QString &sig_text);
  void playerJoin(const QString &sig_text);
  void playerLeave(const QString &sig_text);
  void handStart(const QString &sig_text);
  void deal(const QString &sig_text);
  void playerAction(const QString &sig_text);
  void showCards(const QString &sig_text, QPlayer *player,
                 QCard *card0, QCard *card1);
  void showdown(const QString &sig_text);
  void potWin(const QString &sig_text, QPlayer *player, int amount);

  void viewChanged(const QGameView &sig_view);

private:
  QCard shown_cards_[MAX_NUM_PLAYERS * 2];
  void updateView();
  std::shared_ptr<const GameView> view_;
  QGameView updateableView_;
};

#endif  // QTEVENTLISTENER_H_
