#ifndef QCARD_H_
#define QCARD_H_

#include <QObject>
#include "Card.h"

class QCard : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString str READ str NOTIFY cardChanged)

public:
  QCard() {
    QObject(0);
  }

  QCard(const Card &other) { copyFrom(other); }

  void copyFrom(const Card &other) { str_ = QString::fromStdString(other.str()); }
  void copyFromQ(const QCard &other) { str_ = other.str_; }

  QString str() const { return str_; }

signals:
  void cardChanged();

private:
  QString str_;
};

// necessary?
Q_DECLARE_METATYPE(QCard *);
Q_DECLARE_METATYPE(const QCard *);

#endif  // QCARD_H_
