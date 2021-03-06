#ifndef QPLAYER_H_
#define QPLAYER_H_

#include <QObject>
#include <QQuickItem>
#include <QMetaType>
#include <iostream>
#include "Player.h"

class QPlayer : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString name READ name NOTIFY nameChanged)
  Q_PROPERTY(unsigned int seat READ seat NOTIFY seatChanged)
  Q_PROPERTY(unsigned int chips READ chips NOTIFY chipsChanged)
  Q_PROPERTY(unsigned int chipsInPlay READ chipsInPlay NOTIFY chipsInPlayChanged)
  Q_PROPERTY(bool live READ live NOTIFY liveChanged)
  Q_PROPERTY(bool exists READ exists NOTIFY existsChanged)
  
public:
  QPlayer();
  QPlayer(const Player &other);
  ~QPlayer();

  // QObjects aren't supposed to be copied, because of the parent
  // pointer thing, but QPlayers never have a parent, and we need
  // a way to update the model, and this is better than manipulating
  // private members
  void copyFrom(const QPlayer &other);
  
  inline QString name() const { return name_; }
  inline unsigned int seat() const { return seat_; }
  inline unsigned int chips() const { return chips_; }
  inline unsigned int chipsInPlay() const { return chips_in_play_; }
  inline bool live() const { return live_; }
  inline bool exists() const { return exists_; }

  // sorry
  void unexist() { exists_ = false; }

signals:
  void nameChanged();
  void seatChanged();
  void chipsChanged();
  void chipsInPlayChanged();
  void liveChanged();
  void existsChanged();
  
private:
  QString name_;
  unsigned int chips_;
  unsigned int chips_in_play_;
  unsigned int seat_;
  bool live_;
  bool exists_;
};

#endif  // QPLAYER_H_
