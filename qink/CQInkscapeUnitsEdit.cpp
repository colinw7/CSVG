#include <CQInkscapeUnitsEdit.h>

namespace CQInkscape {

UnitsEdit::
UnitsEdit(QWidget *parent) :
 QComboBox(parent)
{
  setObjectName("units");

  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  updateItems();

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged()));
}

const UnitsEdit::Units &
UnitsEdit::
units() const
{
  return units_;
}

void
UnitsEdit::
setUnits(const Units &units)
{
  units_ = units;

  switch (units) {
    case CScreenUnits::Units::CM: setCurrentIndex(0); break;
    case CScreenUnits::Units::IN: setCurrentIndex(1); break;
    case CScreenUnits::Units::PT: setCurrentIndex(2); break;
    case CScreenUnits::Units::PC: setCurrentIndex(3); break;
    case CScreenUnits::Units::MM: setCurrentIndex(4); break;
    case CScreenUnits::Units::PX: setCurrentIndex(5); break;
    default:                      setCurrentIndex(0); break;
  }

  updateTip();
}

void
UnitsEdit::
setPercent(bool b)
{
  if (b != percent_) {
    percent_ = b;

    updateItems();
  }
}

void
UnitsEdit::
updateItems()
{
  QStringList unitNames, tipNames;

  if (isPercent()) {
     unitNames << "%";
     tipNames  << "Percent";
  }

  unitNames << "cm" << "in" << "pt" << "pc" << "mm" << "px";
  tipNames  << "centimeters" << "inches" << "points" << "picsa" << "millimeters" << "pixels";

  disconnect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged()));

  clear();

  addItems(unitNames);

  assert(unitNames.size() == tipNames.size());

  for (int i = 0; i < count(); ++i)
    setItemData(i, tipNames[i], Qt::ToolTipRole);

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged()));

  updateTip();
}

void
UnitsEdit::
indexChanged()
{
  auto ustr = currentText();

  int ind = currentIndex();

  if (isPercent()) {
    if (ind == 0) units_ = CScreenUnits::Units::CM;

    --ind;
  }

  switch (ind) {
    case 0: units_ = CScreenUnits::Units::CM; break;
    case 1: units_ = CScreenUnits::Units::IN; break;
    case 2: units_ = CScreenUnits::Units::PT; break;
    case 3: units_ = CScreenUnits::Units::PC; break;
    case 4: units_ = CScreenUnits::Units::MM; break;
    case 5: units_ = CScreenUnits::Units::PX; break;
  }

  updateTip();

  emit unitsChanged();
}

void
UnitsEdit::
updateTip()
{
  int ind = std::max(currentIndex(), 0);

  setToolTip(itemData(ind, Qt::ToolTipRole).toString());
}

QSize
UnitsEdit::
sizeHint() const
{
  QFontMetrics fm(font());

  return QSize(fm.horizontalAdvance("px") + 8, fm.height() + 4);
}

}
