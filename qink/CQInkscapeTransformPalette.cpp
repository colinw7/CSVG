#include <CQInkscapeTransformPalette.h>
#include <CQInkscapeWindow.h>
#include <CQInkscapeUnitsEdit.h>

#include <CSVGObject.h>

#include <CQTabWidget.h>
#include <CQRealSpin.h>
#include <CQUtil.h>

#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>

namespace CQInkscape {

TransformPalette::
TransformPalette(Window *window) :
 Palette(window, "Fill/Stroke")
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 2, 2);

  auto *tab = CQUtil::makeWidget<CQTabWidget>(this, "tab");

  layout->addWidget(tab);

  move_   = new MoveTransform  (window_);
  scale_  = new ScaleTransform (window_);
  rotate_ = new RotateTransform(window_);
  skew_   = new SkewTransform  (window_);
  matrix_ = new MatrixTransform(window_);

  tab->addTab(move_  , "Move");
  tab->addTab(scale_ , "Scale");
  tab->addTab(rotate_, "Rotate");
  tab->addTab(skew_  , "Skew");
  tab->addTab(matrix_, "Matrix");
}

void
TransformPalette::
setObject(CSVGObject *obj)
{
  move_  ->setObject(obj);
  scale_ ->setObject(obj);
  rotate_->setObject(obj);
  skew_  ->setObject(obj);
  matrix_->setObject(obj);
}

//----

MoveTransform::
MoveTransform(Window *window) :
 window_(window)
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this);

  auto *editLayout = CQUtil::makeLayout<QGridLayout>();

  layout->addLayout(editLayout);

  int row = 0;

  auto makeRealEdit = [&](const QString &name, const QString &desc,
                          const char *slotName=nullptr, bool addUnits=false) {
    auto *label = (name != "" ? CQUtil::makeLabelWidget<QLabel>(name, "label") : nullptr);
    auto *edit  = CQUtil::makeWidget<CQRealSpin>("edit");

    if (label)
      editLayout->addWidget(label, row, 0);

    editLayout->addWidget(edit, row, 1);

    edit->setToolTip(desc);

    if (slotName)
      connect(edit, SIGNAL(valueChanged(double)), this, slotName);

    if (addUnits) {
      auto *units = new UnitsEdit;

      editLayout->addWidget(units, row, 2);
    }

    ++row;

    return edit;
  };

  hEdit_ = makeRealEdit("Horizontal", "Horizontal Delta", nullptr, /*addUnits*/true);
  vEdit_ = makeRealEdit("Vertical"  , "Vertical Delta");

  relCheck_ = CQUtil::makeLabelWidget<QCheckBox>("Relative Move", "relative");

  layout->addWidget(relCheck_);

  layout->addStretch(1);
}

void
MoveTransform::
setObject(CSVGObject *obj)
{
  hEdit_->setValue(0.0);
  vEdit_->setValue(0.0);

  if (! relCheck_->isChecked()) {
    CBBox2D bbox;

    if (! obj->getFlatTransformedBBox(bbox))
      return;

    hEdit_->setValue(bbox.getXMin());
    vEdit_->setValue(bbox.getYMin());
  }
}

//----

ScaleTransform::
ScaleTransform(Window *window) :
 window_(window)
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this);

  auto makeRealEdit = [&](const QString &name, const QString &desc,
                          const char *slotName=nullptr) {
    auto *rframe  = CQUtil::makeWidget<QFrame>("frame");
    auto *rlayout = CQUtil::makeLayout<QHBoxLayout>(rframe, 0, 2);

    auto *label = (name != "" ? CQUtil::makeLabelWidget<QLabel>(name, "label") : nullptr);
    auto *edit  = CQUtil::makeWidget<CQRealSpin>("edit");

    if (label)
      rlayout->addWidget(label);

    rlayout->addWidget(edit);

    edit->setToolTip(desc);

    if (slotName)
      connect(edit, SIGNAL(valueChanged(double)), this, slotName);

    layout->addWidget(rframe);

    return edit;
  };

  wEdit_ = makeRealEdit("Width" , "Width Scale");
  hEdit_ = makeRealEdit("Height", "Height Scale");

  propCheck_ = CQUtil::makeLabelWidget<QCheckBox>("Scale proportionally", "propCheck");

  layout->addWidget(propCheck_);

  layout->addStretch(1);
}

void
ScaleTransform::
setObject(CSVGObject *)
{
}

//----

RotateTransform::
RotateTransform(Window *window) :
 window_(window)
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this);

  auto makeRealEdit = [&](const QString &name, const QString &desc,
                          const char *slotName=nullptr) {
    auto *rframe  = CQUtil::makeWidget<QFrame>("frame");
    auto *rlayout = CQUtil::makeLayout<QHBoxLayout>(rframe, 0, 2);

    auto *label = (name != "" ? CQUtil::makeLabelWidget<QLabel>(name, "label") : nullptr);
    auto *edit  = CQUtil::makeWidget<CQRealSpin>("edit");

    if (label)
      rlayout->addWidget(label);

    rlayout->addWidget(edit);

    edit->setToolTip(desc);

    if (slotName)
      connect(edit, SIGNAL(valueChanged(double)), this, slotName);

    layout->addWidget(rframe);

    return edit;
  };

  aEdit_ = makeRealEdit("Angle" , "Angle");

  layout->addStretch(1);
}

void
RotateTransform::
setObject(CSVGObject *)
{
}

//----

SkewTransform::
SkewTransform(Window *window) :
 window_(window)
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this);

  auto makeRealEdit = [&](const QString &name, const QString &desc,
                          const char *slotName=nullptr) {
    auto *rframe  = CQUtil::makeWidget<QFrame>("frame");
    auto *rlayout = CQUtil::makeLayout<QHBoxLayout>(rframe, 0, 2);

    auto *label = (name != "" ? CQUtil::makeLabelWidget<QLabel>(name, "label") : nullptr);
    auto *edit  = CQUtil::makeWidget<CQRealSpin>("edit");

    if (label)
      rlayout->addWidget(label);

    rlayout->addWidget(edit);

    edit->setToolTip(desc);

    if (slotName)
      connect(edit, SIGNAL(valueChanged(double)), this, slotName);

    layout->addWidget(rframe);

    return edit;
  };

  hEdit_ = makeRealEdit("Horizontal", "Horizontal Delta");
  vEdit_ = makeRealEdit("Vertical"  , "Vertical Delta");

  layout->addStretch(1);
}

void
SkewTransform::
setObject(CSVGObject *)
{
}

//----

MatrixTransform::
MatrixTransform(Window *window) :
 window_(window)
{
}

void
MatrixTransform::
setObject(CSVGObject *)
{
}

//---

}
