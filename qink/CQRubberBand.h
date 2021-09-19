#ifndef CQRubberBand_H
#define CQRubberBand_H

#include <QRubberBand>

class CQRubberBandStyle;

class CQRubberBand : public QRubberBand {
  Q_OBJECT

 public:
  CQRubberBand(QWidget *p=nullptr);
  CQRubberBand(QRubberBand::Shape shape, QWidget *p=nullptr);

 ~CQRubberBand();

  const QColor &color() const;
  void setColor(const QColor &c);

  void setBounds(const QPoint &p1, const QPoint &p2);

 private:
  CQRubberBandStyle *style_ { nullptr };
};

#endif
