#include <CQInkscapeTransformPalette.h>
#include <CQInkscapeWindow.h>
#include <CQInkscapeUnitsEdit.h>

#include <CSVGObject.h>

#include <CQTabWidget.h>
#include <CQRealSpin.h>
#include <CQAngleSpinBox.h>
#include <CQUtil.h>

#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace CQInkscape {

TransformPalette::
TransformPalette(Window *window) :
 Palette(window, "Transform")
{
  setObjectName("transform");

  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 2, 2);

  tab_ = CQUtil::makeWidget<CQTabWidget>(this, "tab");

  layout->addWidget(tab_);

  move_   = new MoveTransform  (window_);
  scale_  = new ScaleTransform (window_);
  rotate_ = new RotateTransform(window_);
  skew_   = new SkewTransform  (window_);
  matrix_ = new MatrixTransform(window_);

  tab_->addTab(move_  , "Move");
  tab_->addTab(scale_ , "Scale");
  tab_->addTab(rotate_, "Rotate");
  tab_->addTab(skew_  , "Skew");
  tab_->addTab(matrix_, "Matrix");

  eachCheck_ = CQUtil::makeLabelWidget<QCheckBox>("Apply to each object separately", "eachCheck");

  layout->addWidget(eachCheck_);

  auto *buttonBar    = CQUtil::makeWidget<QFrame>("buttonBar");
  auto *buttonLayout = CQUtil::makeLayout<QHBoxLayout>(buttonBar, 2, 2);

  layout->addWidget(buttonBar);

  auto *clearButton = CQUtil::makeLabelWidget<QPushButton>("Clear", "clear");
  auto *applyButton = CQUtil::makeLabelWidget<QPushButton>("Apply", "apply");

  buttonLayout->addStretch();
  buttonLayout->addWidget(clearButton);
  buttonLayout->addWidget(applyButton);

  connect(clearButton, SIGNAL(clicked()), this, SLOT(clearSlot()));
  connect(applyButton, SIGNAL(clicked()), this, SLOT(applySlot()));
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

void
TransformPalette::
clearSlot()
{
  int ind = tab_->currentIndex();

  switch (ind) {
    case 0 : move_  ->clear(); break;
    case 1 : scale_ ->clear(); break;
    case 2 : rotate_->clear(); break;
    case 3 : skew_  ->clear(); break;
    case 4 : matrix_->clear(); break;
  }
}

void
TransformPalette::
applySlot()
{
  int ind = tab_->currentIndex();

  switch (ind) {
    case 0 : move_  ->apply(); break;
    case 1 : scale_ ->apply(); break;
    case 2 : rotate_->apply(); break;
    case 3 : skew_  ->apply(); break;
    case 4 : matrix_->apply(); break;
  }
}

//----

TransformFrame::
TransformFrame(Window *window) :
 window_(window)
{
}

//----

MoveTransform::
MoveTransform(Window *window) :
 TransformFrame(window)
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

  connect(relCheck_, SIGNAL(stateChanged(int)), this, SLOT(relSlot(int)));

  layout->addStretch(1);
}

void
MoveTransform::
relSlot(int)
{
  updateState();
}

void
MoveTransform::
setObject(CSVGObject *obj)
{
  object_ = obj;

  updateState();
}

void
MoveTransform::
updateState()
{
  hEdit_->setValue(0.0);
  vEdit_->setValue(0.0);

  if (! relCheck_->isChecked()) {
    CBBox2D bbox;

    if (! object_ || ! object_->getFlatTransformedBBox(bbox))
      return;

    hEdit_->setValue(bbox.getXMin());
    vEdit_->setValue(bbox.getYMin());
  }
}

void
MoveTransform::
clear()
{
}

void
MoveTransform::
apply()
{
  if (! object_)
    return;

  auto x = hEdit_->value();
  auto y = vEdit_->value();

  if (! relCheck_->isChecked()) {
    object_->moveTo(CPoint2D(x, y));
  }
  else {
    object_->moveBy(CVector2D(x, y));
  }

  window_->redraw(/*update*/true);
}

//----

ScaleTransform::
ScaleTransform(Window *window) :
 TransformFrame(window)
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

      units->setPercent(true);

      editLayout->addWidget(units, row, 2);
    }

    ++row;

    return edit;
  };

  wEdit_ = makeRealEdit("Width" , "Width Scale", nullptr, /*addUnits*/true);
  hEdit_ = makeRealEdit("Height", "Height Scale");

  propCheck_ = CQUtil::makeLabelWidget<QCheckBox>("Scale proportionally", "propCheck");

  layout->addWidget(propCheck_);

  layout->addStretch(1);
}

void
ScaleTransform::
setObject(CSVGObject *obj)
{
  object_ = obj;

  updateState();
}

void
ScaleTransform::
updateState()
{
  wEdit_->setValue(0.0);
  hEdit_->setValue(0.0);

  CBBox2D bbox;

  if (! object_ || ! object_->getFlatTransformedBBox(bbox))
    return;

  wEdit_->setValue(bbox.getWidth ());
  hEdit_->setValue(bbox.getHeight());
}

void
ScaleTransform::
clear()
{
}

void
ScaleTransform::
apply()
{
  if (! object_)
    return;

  auto w = wEdit_->value();
  auto h = hEdit_->value();

  if (! propCheck_->isChecked()) {
    object_->resizeTo(CSize2D(w, h));
  }
  else {
    object_->resizeTo(CSize2D(w, h));
  }

  window_->redraw(/*update*/true);
}

//----

RotateTransform::
RotateTransform(Window *window) :
 TransformFrame(window)
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this);

  auto makeAngleEdit = [&](const QString &name, const QString &desc,
                           const char *slotName=nullptr) {
    auto *rframe  = CQUtil::makeWidget<QFrame>("frame");
    auto *rlayout = CQUtil::makeLayout<QHBoxLayout>(rframe, 0, 2);

    auto *label = (name != "" ? CQUtil::makeLabelWidget<QLabel>(name, "label") : nullptr);
    auto *edit  = CQUtil::makeWidget<CQAngleSpinBox>("edit");

    if (label)
      rlayout->addWidget(label);

    rlayout->addWidget(edit);

    edit->setToolTip(desc);

    if (slotName)
      connect(edit, SIGNAL(valueChanged(double)), this, slotName);

    layout->addWidget(rframe);

    return edit;
  };

  aEdit_ = makeAngleEdit("Angle" , "Angle");

  layout->addStretch(1);
}

void
RotateTransform::
setObject(CSVGObject *obj)
{
  object_ = obj;

  //updateState();
}

void
RotateTransform::
clear()
{
}

void
RotateTransform::
apply()
{
  if (! object_)
    return;

  auto a = aEdit_->getAngle().radians();

  object_->rotateBy(a);

  window_->redraw(/*update*/true);
}

//----

SkewTransform::
SkewTransform(Window *window) :
 TransformFrame(window)
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

void
SkewTransform::
clear()
{
}

void
SkewTransform::
apply()
{
}

//----

MatrixTransform::
MatrixTransform(Window *window) :
 TransformFrame(window)
{
}

void
MatrixTransform::
setObject(CSVGObject *)
{
}

void
MatrixTransform::
clear()
{
}

void
MatrixTransform::
apply()
{
}

//---

}
