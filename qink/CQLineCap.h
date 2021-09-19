#ifndef CQLINE_CAP_H
#define CQLINE_CAP_H

#include <QComboBox>
#include <CLineCapType.h>

class CQLineCap : public QComboBox {
  Q_OBJECT

 public:
  CQLineCap(QWidget *parent=nullptr);

  void setLineCap(CLineCapType cap);

  CLineCapType getLineCap() const;

 private slots:
  void capChangedSlot(int value);

 signals:
  void valueChanged(const CLineCapType &cap);
};

#endif
