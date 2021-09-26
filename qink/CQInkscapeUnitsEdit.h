#ifndef CQInkscapeUnitsEdit_H
#define CQInkscapeUnitsEdit_H

#include <CScreenUnits.h>
#include <QComboBox>

namespace CQInkscape {

/*!
 * \brief units edit
 * \ingroup Charts
 */
class UnitsEdit : public QComboBox {
  Q_OBJECT

 public:
  using Units = CScreenUnits::Units;

 public:
  UnitsEdit(QWidget *parent=nullptr);

  const Units &units() const;
  void setUnits(const Units &units);

  QSize sizeHint() const override;

 signals:
  void unitsChanged();

 private:
  void updateTip();

 private slots:
  void indexChanged();

 private:
  Units units_ { Units::PX };
};

}

#endif
