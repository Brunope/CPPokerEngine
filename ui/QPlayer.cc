#include <QObject>
#include <iostream>
#include "QPlayer.h"

QPlayer::QPlayer() {
  QObject(0);
  name_ = "default";
  live_ = false;
  exists_ = false;
}

QPlayer::QPlayer(const Player &other) {
  QObject(0);
  name_ = QString::fromStdString(other.getName());
  chips_ = other.getChips();
  chips_in_play_ = other.getChipsInPlay();
  seat_ = other.getSeat();
  live_ = other.isLive();
  exists_ = true;
}

QPlayer::~QPlayer() {

}

void
QPlayer::copyFrom(const QPlayer &other) {
  name_ = other.name_;
  chips_ = other.chips_;
  chips_in_play_ = other.chips_in_play_;
  seat_ = other.seat_;
  live_ = other.live_;
  exists_ = other.exists_;

  emit nameChanged();
  emit seatChanged();
  emit chipsChanged();
  emit chipsInPlayChanged();
  emit liveChanged();
  emit existsChanged();
}
