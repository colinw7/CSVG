#include <CQInkscapeFillStrokePalette.h>
#include <CQInkscapeWindow.h>

#include <CSVGObject.h>
#include <CSVGPattern.h>
#include <CSVGLinearGradient.h>
#include <CSVGRadialGradient.h>

#include <CQTabWidget.h>
#include <CQImageButton.h>
#include <CQPixmapCache.h>
#include <CQColorChooser.h>
#include <CQLineDash.h>
#include <CQLineCap.h>
#include <CQLineJoin.h>
#include <CQRealSpin.h>
#include <CQUtil.h>

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

namespace CQInkscape {

FillStrokePalette::
FillStrokePalette(Window *window) :
 Palette(window, "Fill/Stroke")
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 2, 2);

  auto *tab = CQUtil::makeWidget<CQTabWidget>(this, "tab");

  layout->addWidget(tab);

  fill_        = new Fill       (window_);
  strokePaint_ = new StrokePaint(window_);
  strokeStyle_ = new StrokeStyle(window_);

  tab->addTab(fill_       , "Fill");
  tab->addTab(strokePaint_, "Stroke paint");
  tab->addTab(strokeStyle_, "Stroke style");
}

void
FillStrokePalette::
setObject(CSVGObject *obj)
{
  fill_       ->setObject(obj);
  strokePaint_->setObject(obj);
  strokeStyle_->setObject(obj);
}

//----

Fill::
Fill(Window *window) :
 window_(window)
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  //-----

  // mode toolbar
  auto *buttonLayout = CQUtil::makeLayout<QHBoxLayout>(2, 8);

  layout->addLayout(buttonLayout);

  // mode panel stack
  stack_ = CQUtil::makeWidget<QStackedWidget>("stack");

  layout->addWidget(stack_);

  //-----

  auto makeCheckButton = [&](const QString &icon, const QString &tip) {
    auto *button = new CQImageButton(CQPixmapCacheInst->getIcon(icon));

    button->setCheckable(true);
    button->setToolTip(tip);

    buttonLayout->addWidget(button);

    return button;
  };

  auto makePanel = [&](const QString &name) {
    auto *panel  = CQUtil::makeWidget<QFrame>(name);
    (void) CQUtil::makeLayout<QGridLayout>(panel, 2, 2);
    stack_->addWidget(panel);
    return panel;
  };

  auto makeModeData = [&](ModeData &modeData, const QString &name,
                          const QString &icon, const QString &tip,
                          const char *slotName) {
    modeData.ind    = stack_->count();
    modeData.button = makeCheckButton(icon, tip);
    modeData.panel  = makePanel(name + "Panel");
    modeData.layout = qobject_cast<QGridLayout *>(modeData.panel->layout());

    connect(modeData.button, SIGNAL(clicked()), this, slotName);

    modeDatas_.push_back(&modeData);
  };

  makeModeData(noneData_   , "none"   , "FILL_NONE"     , "None"           , SLOT(noneSlot()));
  makeModeData(solidData_  , "solid"  , "FILL_SOLID"    , "Solid"          , SLOT(solidSlot()));
  makeModeData(currentData_, "current", "FILL_CURRENT"  , "Current"        , SLOT(currentSlot()));
  makeModeData(inheritData_, "inherit", "FILL_INHERIT"  , "Inherit"        , SLOT(inheritSlot()));
  makeModeData(lgradData_  , "lgrad"  , "FILL_LGRADIENT", "Linear Gradient", SLOT(lgradSlot()));
  makeModeData(rgradData_  , "rgrad"  , "FILL_RGRADIENT", "Radial Gradient", SLOT(rgradSlot()));
  makeModeData(imageData_  , "image"  , "FILL_IMAGE"    , "Image"          , SLOT(imageSlot()));

  buttonLayout->addStretch();

  //-----

  int solidRow = 0;

  solidData_.layout->addWidget(new QLabel("Color"  ), solidRow++, 0);
  solidData_.layout->addWidget(new QLabel("Opacity"), solidRow++, 0);
  solidData_.layout->addWidget(new QLabel("Rule"   ), solidRow++, 0);
  solidData_.layout->addWidget(new QLabel("Clip"   ), solidRow++, 0);

  colorChooser_ = new CQColorChooser;

  colorChooser_->setStyles(CQColorChooser::ColorButton);

  connect(colorChooser_, SIGNAL(colorChanged(const QColor &)),
          this, SLOT(colorSlot(const QColor &)));

  opacityEdit_ = CQUtil::makeWidget<CQRealSpin>("opacityEdit");

  opacityEdit_->setRange(0.0, 1.0);
  opacityEdit_->setSingleStep(0.1);
  opacityEdit_->setDecimals(5);
//opacityEdit_->setRange(0.00, 1.0, 5);

  connect(opacityEdit_, SIGNAL(valueChanged(double)), this, SLOT(opacitySlot(double)));

  fillRule_ = CQUtil::makeWidget<QComboBox>("fillRule");

  fillRule_->addItems(QStringList() << "Winding" << "Even Odd");

  connect(fillRule_, SIGNAL(currentIndexChanged(const QString &)),
          this, SLOT(fillRuleSlot(const QString &)));

  clipCheck_ = CQUtil::makeLabelWidget<QCheckBox>("Yes", "clipCheck");

  connect(clipCheck_, SIGNAL(clicked(bool)), this, SLOT(clipSlot()));

  solidRow = 0;

  solidData_.layout->addWidget(colorChooser_, solidRow++, 1);
  solidData_.layout->addWidget(opacityEdit_ , solidRow++, 1);
  solidData_.layout->addWidget(fillRule_    , solidRow++, 1);
  solidData_.layout->addWidget(clipCheck_   , solidRow++, 1);

  solidData_.layout->setRowStretch(solidRow++, 1);

  //---

  layout->addStretch();
}

void
Fill::
noneSlot()
{
  stack_->setCurrentIndex(noneData_.ind);

  if (! obj_ || ! obj_->hasFill())
    return;

  auto &fill = obj_->getFill();

  fill.setColor(CSVGFill::Color(CSVGColor::makeNone()));

  window_->redraw(/*update*/true);
}

void
Fill::
solidSlot()
{
  stack_->setCurrentIndex(solidData_.ind);

  if (! obj_ || ! obj_->hasFill())
    return;

  auto &fill = obj_->getFill();

  auto c = colorChooser_->color();

  fill.setColor(CSVGFill::Color(CSVGColor::makeRGBA(CQUtil::colorToRGBA(c))));

  window_->redraw(/*update*/true);
}

void
Fill::
currentSlot()
{
  stack_->setCurrentIndex(currentData_.ind);

  if (! obj_ || ! obj_->hasFill())
    return;

  auto &fill = obj_->getFill();

  fill.setColor(CSVGFill::Color(CSVGColor::makeCurrent()));

  window_->redraw(/*update*/true);
}

void
Fill::
inheritSlot()
{
  stack_->setCurrentIndex(inheritData_.ind);

  if (! obj_ || ! obj_->hasFill())
    return;

  auto &fill = obj_->getFill();

  fill.setColor(CSVGFill::Color::makeInherit());

  window_->redraw(/*update*/true);
}

void
Fill::
lgradSlot()
{
  stack_->setCurrentIndex(lgradData_.ind);
}

void
Fill::
rgradSlot()
{
  stack_->setCurrentIndex(rgradData_.ind);
}

void
Fill::
imageSlot()
{
  stack_->setCurrentIndex(imageData_.ind);
}

void
Fill::
colorSlot(const QColor &c)
{
  if (! obj_ || ! obj_->hasFill())
    return;

  auto &fill = obj_->getFill();

  fill.setColor(CSVGFill::Color(CSVGColor::makeRGBA(CQUtil::colorToRGBA(c))));

  window_->redraw(/*update*/true);
}

void
Fill::
opacitySlot(double o)
{
  if (! obj_ || ! obj_->hasFill())
    return;

  auto &fill = obj_->getFill();

  fill.setOpacity(CSVGFill::Opacity(o));

  window_->redraw(/*update*/true);
}

void
Fill::
fillRuleSlot(const QString &)
{
}

void
Fill::
clipSlot()
{
}

void
Fill::
setObject(CSVGObject *obj)
{
  obj_ = obj;

  for (auto &modeData : modeDatas_)
    modeData->button->setChecked(false);

  if (! obj_ || ! obj_->hasFill())
    return;

  auto setCurrentData = [&](const ModeData &modeData) {
    stack_->setCurrentIndex(modeData.ind);
    modeData.button->setChecked(true);
  };

  auto &fill = obj_->getFill();

  auto *fillObject = fill.calcFillObject();

  if (fillObject) {
    auto *lg = dynamic_cast<CSVGLinearGradient *>(fillObject);
    auto *rg = dynamic_cast<CSVGRadialGradient *>(fillObject);
    auto *pt = dynamic_cast<CSVGPattern        *>(fillObject);

    if      (lg)
      setCurrentData(lgradData_);
    else if (rg)
      setCurrentData(rgradData_);
    else if (pt)
      setCurrentData(imageData_);
    else
      assert(false);
  }
  else {
    if (fill.getColorValid()) {
      auto ifc = fill.getColor();

      if (! ifc.isInherit()) {
        auto fc = ifc.getValue();

        if      (fc.isNone())
          setCurrentData(noneData_);
        else if (fc.isCurrent())
          setCurrentData(currentData_);
        else {
          setCurrentData(solidData_);

          colorChooser_->setColor(CQUtil::rgbaToColor(fc.rgba()));
        }
      }
      else {
        setCurrentData(inheritData_);
      }
    }

    if (fill.getOpacityValid()) {
      auto ifo = fill.getOpacity();

      if (! ifo.isInherit()) {
        auto fo = ifo.getValue();

        opacityEdit_->setValue(fo);
      }
    }
  }
}

//----

StrokePaint::
StrokePaint(Window *window) :
 window_(window)
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  //---

  // mode toolbar
  auto *buttonLayout = CQUtil::makeLayout<QHBoxLayout>(2, 8);

  layout->addLayout(buttonLayout);

  // mode panel stack
  stack_ = CQUtil::makeWidget<QStackedWidget>("stack");

  layout->addWidget(stack_);

  //-----

  auto makeCheckButton = [&](const QString &icon, const QString &tip) {
    auto *button = new CQImageButton(CQPixmapCacheInst->getIcon(icon));

    button->setCheckable(true);
    button->setToolTip(tip);

    buttonLayout->addWidget(button);

    return button;
  };

  auto makePanel = [&](const QString &name) {
    auto *panel  = CQUtil::makeWidget<QFrame>(name);
    (void) CQUtil::makeLayout<QGridLayout>(panel, 2, 2);
    stack_->addWidget(panel);
    return panel;
  };

  auto makeModeData = [&](ModeData &modeData, const QString &name,
                          const QString &icon, const QString &tip,
                          const char *slotName) {
    modeData.ind    = stack_->count();
    modeData.button = makeCheckButton(icon, tip);
    modeData.panel  = makePanel(name + "Panel");
    modeData.layout = qobject_cast<QGridLayout *>(modeData.panel->layout());

    connect(modeData.button, SIGNAL(clicked()), this, slotName);

    modeDatas_.push_back(&modeData);
  };

  makeModeData(noneData_   , "none"   , "FILL_NONE"     , "None"           , SLOT(noneSlot()));
  makeModeData(solidData_  , "solid"  , "FILL_SOLID"    , "Solid"          , SLOT(solidSlot()));
  makeModeData(currentData_, "current", "FILL_CURRENT"  , "Current"        , SLOT(currentSlot()));
  makeModeData(inheritData_, "inherit", "FILL_INHERIT"  , "Inherit"        , SLOT(inheritSlot()));
  makeModeData(lgradData_  , "lgrad"  , "FILL_LGRADIENT", "Linear Gradient", SLOT(lgradSlot()));
  makeModeData(rgradData_  , "rgrad"  , "FILL_RGRADIENT", "Radial Gradient", SLOT(rgradSlot()));
  makeModeData(imageData_  , "image"  , "FILL_IMAGE"    , "Image"          , SLOT(imageSlot()));

  buttonLayout->addStretch();

  //-----

  int solidRow = 0;

  solidData_.layout->addWidget(new QLabel("Color"  ), solidRow++, 0);
  solidData_.layout->addWidget(new QLabel("Opacity"), solidRow++, 0);

  colorChooser_ = new CQColorChooser;

  colorChooser_->setStyles(CQColorChooser::ColorButton);

  connect(colorChooser_, SIGNAL(colorChanged(const QColor &)),
          this, SLOT(colorSlot(const QColor &)));

  opacityEdit_ = CQUtil::makeWidget<CQRealSpin>("opacityEdit");

  opacityEdit_->setRange(0.0, 1.0);
  opacityEdit_->setSingleStep(0.1);
  opacityEdit_->setDecimals(5);
//opacityEdit_->setRange(0.00, 1.0, 5);

  connect(opacityEdit_, SIGNAL(valueChanged(double)), this, SLOT(opacitySlot(double)));

  solidRow = 0;

  solidData_.layout->addWidget(colorChooser_, solidRow++, 1);
  solidData_.layout->addWidget(opacityEdit_ , solidRow++, 1);

  solidData_.layout->setRowStretch(solidRow++, 1);

  //------

#if 0
  auto *defButton = CQUtil::makeLabelWidget<QPushButton>("Set Default", "defButton");

  //connect(defButton, SIGNAL(clicked()), this, SLOT(setDefaultSlot()));

  layout->addWidget(defButton);
#endif

  //---

  layout->addStretch();
}

void
StrokePaint::
noneSlot()
{
  stack_->setCurrentIndex(noneData_.ind);

  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  stroke.setColor(CSVGStroke::Color(CSVGColor::makeNone()));

  window_->redraw(/*update*/true);
}

void
StrokePaint::
solidSlot()
{
  stack_->setCurrentIndex(solidData_.ind);

  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  auto c = colorChooser_->color();

  stroke.setColor(CSVGStroke::Color(CSVGColor::makeRGBA(CQUtil::colorToRGBA(c))));

  window_->redraw(/*update*/true);
}

void
StrokePaint::
currentSlot()
{
  stack_->setCurrentIndex(currentData_.ind);

  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  stroke.setColor(CSVGStroke::Color(CSVGColor::makeCurrent()));

  window_->redraw(/*update*/true);
}

void
StrokePaint::
inheritSlot()
{
  stack_->setCurrentIndex(inheritData_.ind);

  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  stroke.setColor(CSVGStroke::Color::makeInherit());

  window_->redraw(/*update*/true);
}

void
StrokePaint::
lgradSlot()
{
  stack_->setCurrentIndex(lgradData_.ind);
}

void
StrokePaint::
rgradSlot()
{
  stack_->setCurrentIndex(rgradData_.ind);
}

void
StrokePaint::
imageSlot()
{
  stack_->setCurrentIndex(imageData_.ind);
}

void
StrokePaint::
colorSlot(const QColor &c)
{
  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  stroke.setColor(CSVGStroke::Color(CSVGColor::makeRGBA(CQUtil::colorToRGBA(c))));

  window_->redraw(/*update*/true);
}

void
StrokePaint::
opacitySlot(double o)
{
  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  stroke.setOpacity(CSVGStroke::Opacity(o));

  window_->redraw(/*update*/true);
}

void
StrokePaint::
setObject(CSVGObject *obj)
{
  obj_ = obj;

  for (auto &modeData : modeDatas_)
    modeData->button->setChecked(false);

  if (! obj_ || ! obj_->hasStroke())
    return;

  auto setCurrentData = [&](const ModeData &modeData) {
    stack_->setCurrentIndex(modeData.ind);
    modeData.button->setChecked(true);
  };

  auto &stroke = obj_->getStroke();

  auto *fillObject = stroke.getFillObject();

  if (fillObject) {
    auto *lg = dynamic_cast<CSVGLinearGradient *>(fillObject);
    auto *rg = dynamic_cast<CSVGRadialGradient *>(fillObject);
    auto *pt = dynamic_cast<CSVGPattern        *>(fillObject);

    if      (lg)
      setCurrentData(lgradData_);
    else if (rg)
      setCurrentData(rgradData_);
    else if (pt)
      setCurrentData(imageData_);
    else
      assert(false);
  }
  else {
    if (stroke.getColorValid()) {
      auto ifc = stroke.getColor();

      if (! ifc.isInherit()) {
        auto fc = ifc.getValue();

        if      (fc.isNone())
          setCurrentData(noneData_);
        else if (fc.isCurrent())
          setCurrentData(currentData_);
        else {
          setCurrentData(solidData_);

          colorChooser_->setColor(CQUtil::rgbaToColor(fc.rgba()));
        }
      }
      else
        setCurrentData(inheritData_);
    }

    if (stroke.getOpacityValid()) {
      auto ifo = stroke.getOpacity();

      if (! ifo.isInherit()) {
        auto fo = ifo.getValue();

        opacityEdit_->setValue(fo);
      }
    }
  }
}

//----

StrokeStyle::
StrokeStyle(Window *window) :
 window_(window)
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  //---

  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  gridLayout->addWidget(new QLabel("Width"      ), 0, 0);
  gridLayout->addWidget(new QLabel("Dash"       ), 1, 0);
  gridLayout->addWidget(new QLabel("Line Join"  ), 2, 0);
  gridLayout->addWidget(new QLabel("Line Cap"   ), 3, 0);
  gridLayout->addWidget(new QLabel("Mitre Limit"), 4, 0);

  widthEdit_ = new CQRealSpin;
  dashEdit_  = new CQLineDash;
  joinEdit_  = new CQLineJoin;
  capEdit_   = new CQLineCap;
  mitreEdit_ = new CQRealSpin;

  widthEdit_->setRange(0.01, 1000);
  widthEdit_->setSingleStep(1.0);
  widthEdit_->setDecimals(3);
//widthEdit_->setValidator(new QDoubleValidator(0.01, 1000.0, 3, widthEdit_));

  mitreEdit_->setRange(0.01, 100.0);
  mitreEdit_->setSingleStep(1.0);
  mitreEdit_->setDecimals(3);
//mitreEdit_->setValidator(new QDoubleValidator(0.01,  100.0, 3, mitreEdit_));

  connect(widthEdit_, SIGNAL(valueChanged(double)),
          this, SLOT(widthSlot(double)));
  connect(dashEdit_ , SIGNAL(valueChanged(const CLineDash &)),
          this, SLOT(dashSlot(const CLineDash &)));
  connect(capEdit_  , SIGNAL(valueChanged(CLineCapType)),
          this, SLOT(capSlot(CLineCapType)));
  connect(joinEdit_ , SIGNAL(valueChanged(CLineJoinType)),
          this, SLOT(joinSlot(CLineJoinType)));
  connect(mitreEdit_, SIGNAL(valueChanged(double)),
          this, SLOT(mitreSlot(double)));

  gridLayout->addWidget(widthEdit_, 0, 1);
  gridLayout->addWidget(dashEdit_ , 1, 1);
  gridLayout->addWidget(joinEdit_ , 2, 1);
  gridLayout->addWidget(capEdit_  , 3, 1);
  gridLayout->addWidget(mitreEdit_, 4, 1);

  layout->addLayout(gridLayout);

  //------

#if 0
  auto *defButton = CQUtil::makeLabelWidget<QPushButton>("Set Default", "defButton");

  //connect(defButton, SIGNAL(clicked()), this, SLOT(setDefaultSlot()));

  layout->addWidget(defButton);
#endif

  //---

  layout->addStretch();
}

void
StrokeStyle::
widthSlot(double w)
{
  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  stroke.setWidth(CSVGStroke::Width(w));

  window_->redraw(/*update*/true);
}

void
StrokeStyle::
dashSlot(const CLineDash &d)
{
  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  CSVGLineDash d1;

  if (d.isSolid())
    d1.setSolid();
  else {
    CSVGLineDash::Dashes dashes;

    for (int i = 0; i < int(d.getNumLengths()); ++i)
      dashes.push_back(d.getLength(i));

    d1.setDashes(dashes);
  }

  stroke.setDashArray(CSVGStroke::DashArray(d1));

  window_->redraw(/*update*/true);
}

void
StrokeStyle::
capSlot(CLineCapType c)
{
  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  stroke.setLineCap(CSVGStroke::LineCap(c));

  window_->redraw(/*update*/true);
}

void
StrokeStyle::
joinSlot(CLineJoinType j)
{
  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  stroke.setLineJoin(CSVGStroke::LineJoin(j));

  window_->redraw(/*update*/true);
}

void
StrokeStyle::
mitreSlot(double l)
{
  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  stroke.setMiterLimit(CSVGStroke::MiterLimit(l));

  window_->redraw(/*update*/true);
}

void
StrokeStyle::
setObject(CSVGObject *obj)
{
  obj_ = obj;

  if (! obj_ || ! obj_->hasStroke())
    return;

  auto &stroke = obj_->getStroke();

  if (stroke.getWidthValid()) {
    auto isw = stroke.getWidth();

    if (! isw.isInherit()) {
      auto sw = isw.getValue();

      widthEdit_->setValue(sw);
    }
  }

  if (stroke.getDashArrayValid()) {
    auto isd = stroke.getDashArray();

    if (! isd.isInherit()) {
      //auto sd = isd.getValue();

      //dashEdit_->setValue(sd);
    }
  }

  if (stroke.getDashOffsetValid()) {
    auto iso = stroke.getDashOffset();

    if (! iso.isInherit()) {
      //auto so = iso.getValue();

      //dashEdit_->setValue(so);
    }
  }

  if (stroke.getLineCapValid()) {
    auto isc = stroke.getLineCap();

    if (! isc.isInherit()) {
      auto sc = isc.getValue();

      capEdit_->setLineCap(sc);
    }
  }

  if (stroke.getLineJoinValid()) {
    auto isj = stroke.getLineJoin();

    if (! isj.isInherit()) {
      auto sj = isj.getValue();

      joinEdit_->setLineJoin(sj);
    }
  }

  if (stroke.getMiterLimitValid()) {
    auto isl = stroke.getMiterLimit();

    if (! isl.isInherit()) {
      auto sl = isl.getValue();

      mitreEdit_->setValue(sl);
    }
  }
}

}
