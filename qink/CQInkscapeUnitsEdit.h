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

  Q_PROPERTY(bool percent READ isPercent WRITE setPercent)

 public:
  using Units = CScreenUnits::Units;

 public:
  UnitsEdit(QWidget *parent=nullptr);

  const Units &units() const;
  void setUnits(const Units &units);

  bool isPercent() const { return percent_; }
  void setPercent(bool b);

  QSize sizeHint() const override;

 signals:
  void unitsChanged();

 private:
  void updateItems();

  void updateTip();

 private slots:
  void indexChanged();

 private:
  Units units_   { Units::PX };
  bool  percent_ { false };
};

}

#endif
