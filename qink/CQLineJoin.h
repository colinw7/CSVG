#ifndef CQLINE_JOIN_H
#define CQLINE_JOIN_H

#include <QComboBox>
#include <CLineJoinType.h>

class CQLineJoin : public QComboBox {
  Q_OBJECT

 public:
  CQLineJoin(QWidget *parent=nullptr);

  void setLineJoin(CLineJoinType pen);

  CLineJoinType getLineJoin() const;

 private slots:
  void joinChangedSlot(int join);

 signals:
  void valueChanged(const CLineJoinType &join);
};

#endif
