#include <CQInkscapeFillStroke.h>

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

FillStroke::
FillStroke(Window *window) :
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
FillStroke::
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

  auto *buttonLayout = CQUtil::makeLayout<QHBoxLayout>(2, 8);

  auto makeCheckButton = [&](const QString &icon) {
    auto *button = new CQImageButton(CQPixmapCacheInst->getIcon(icon));

    button->setCheckable(true);

    buttonLayout->addWidget(button);

    return button;
  };

  noneButton_    = makeCheckButton("FILL_NONE"   );
  solidButton_   = makeCheckButton("FILL_SOLID"  );
  currentButton_ = makeCheckButton("FILL_CURRENT");
  inheritButton_ = makeCheckButton("FILL_INHERIT");

  lgradButton_ = makeCheckButton("FILL_LGRADIENT");
  rgradButton_ = makeCheckButton("FILL_RGRADIENT");
  imageButton_ = makeCheckButton("FILL_IMAGE"    );

  buttonLayout->addStretch();

  layout->addLayout(buttonLayout);

  //connect(solidButton_ , SIGNAL(clicked()), this, SLOT(solidSlot()));
  //connect(lgradButton_, SIGNAL(clicked()), this, SLOT(lgradSlot()));
  //connect(rgradButton_, SIGNAL(clicked()), this, SLOT(rgradSlot()));
  //connect(imageButton_, SIGNAL(clicked()), this, SLOT(imageSlot()));

  //-----

  stack_ = CQUtil::makeWidget<QStackedWidget>("stack");

  layout->addWidget(stack_);

  //-----

  // Normal Fill
  auto *flatPanel = CQUtil::makeWidget<QFrame>("flatPanel");

  stack_->addWidget(flatPanel);

  auto *flatLayout = CQUtil::makeLayout<QGridLayout>(flatPanel, 2, 2);

  flatLayout->addWidget(new QLabel("Color"  ), 0, 0);
  flatLayout->addWidget(new QLabel("Opacity"), 1, 0);
  flatLayout->addWidget(new QLabel("Rule"   ), 2, 0);
  flatLayout->addWidget(new QLabel("Clip"   ), 3, 0);

  colorChooser_ = new CQColorChooser;

  colorChooser_->setStyles(CQColorChooser::ColorButton);

  //connect(colorChooser_, SIGNAL(colorChanged(const QColor &)),
  //        this, SLOT(colorSlot(const QColor &)));

  opacityEdit_ = CQUtil::makeWidget<CQRealSpin>("opacityEdit");

  opacityEdit_->setRange(0.0, 1.0);
  opacityEdit_->setSingleStep(0.1);
  opacityEdit_->setDecimals(5);
//opacityEdit_->setRange(0.00, 1.0, 5);

  //connect(opacityEdit_, SIGNAL(valueChanged(double)), this, SLOT(opacitySlot(double)));

  fillRule_ = CQUtil::makeWidget<QComboBox>("fillRule");

  fillRule_->addItems(QStringList() << "Winding" << "Even Odd");

  //connect(fillRule_, SIGNAL(currentIndexChanged(const QString &)),
  //        this, SLOT(fillRuleSlot(const QString &)));

  clipCheck_ = CQUtil::makeLabelWidget<QCheckBox>("Yes", "clipCheck");

  //connect(clipCheck_, SIGNAL(clicked(bool)), this, SLOT(clipSlot()));

  flatLayout->addWidget(colorChooser_, 0, 1);
  flatLayout->addWidget(opacityEdit_ , 1, 1);
  flatLayout->addWidget(fillRule_    , 2, 1);
  flatLayout->addWidget(clipCheck_   , 3, 1);

  flatLayout->setRowStretch(5, 1);

  //---

  layout->addStretch();
}

void
Fill::
setObject(CSVGObject *obj)
{
  noneButton_   ->setChecked(false);
  solidButton_  ->setChecked(false);
  currentButton_->setChecked(false);
  inheritButton_->setChecked(false);

  lgradButton_->setChecked(false);
  rgradButton_->setChecked(false);
  imageButton_->setChecked(false);

  if (! obj || ! obj->hasFill())
    return;

  auto &fill = obj->getFill();

  auto *fillObject = fill.calcFillObject();

  if (fillObject) {
    auto *lg = dynamic_cast<CSVGLinearGradient *>(fillObject);
    auto *rg = dynamic_cast<CSVGRadialGradient *>(fillObject);
    auto *pt = dynamic_cast<CSVGPattern        *>(fillObject);

    if      (lg) {
      lgradButton_->setChecked(true);
    }
    else if (rg) {
      rgradButton_->setChecked(true);
    }
    else if (pt) {
      imageButton_->setChecked(true);
    }
    else
      assert(false);
  }
  else {
    if (! fill.getColorValid())
      return;

    auto ifc = fill.getColor();

    if (! ifc.isInherit()) {
      auto fc = ifc.getValue();

      if      (fc.isNone())
        noneButton_->setChecked(true);
      else if (fc.isCurrent())
        currentButton_->setChecked(true);
      else {
        solidButton_->setChecked(true);

        colorChooser_->setColor(CQUtil::rgbaToColor(fc.rgba()));
      }
    }
    else
      inheritButton_->setChecked(true);
  }
}

//----

StrokePaint::
StrokePaint(Window *window) :
 window_(window)
{
  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 0);

  //---

  auto *gridLayout = CQUtil::makeLayout<QGridLayout>(2, 2);

  gridLayout->addWidget(new QLabel("Shown"  ), 0, 0);
  gridLayout->addWidget(new QLabel("Color"  ), 1, 0);
  gridLayout->addWidget(new QLabel("Opacity"), 2, 0);

  shownCheck_   = new QCheckBox;
  colorChooser_ = new CQColorChooser;
  opacityEdit_  = new CQRealSpin;

  colorChooser_->setStyles(CQColorChooser::ColorButton);

  opacityEdit_->setRange(0.0, 1.0);
  opacityEdit_->setSingleStep(0.1);
  opacityEdit_->setDecimals(5);
//opacityEdit_->setValidator(new QDoubleValidator(0.00,    1.0, 5, opacityEdit_));

#if 0
  connect(shownCheck_  , SIGNAL(stateChanged(int)),
          this, SLOT(shownSlot(int)));
  connect(colorChooser_, SIGNAL(colorChanged(const QColor &)),
          this, SLOT(colorSlot(const QColor &)));
  connect(opacityEdit_ , SIGNAL(valueChanged(double)),
          this, SLOT(opacitySlot(double)));
#endif

  gridLayout->addWidget(shownCheck_  , 0, 1);
  gridLayout->addWidget(colorChooser_, 1, 1);
  gridLayout->addWidget(opacityEdit_ , 2, 1);

  layout->addLayout(gridLayout);

  //------

  auto *defButton = CQUtil::makeLabelWidget<QPushButton>("Set Default", "defButton");

  //connect(defButton, SIGNAL(clicked()), this, SLOT(setDefaultSlot()));

  layout->addWidget(defButton);

  //---

  layout->addStretch();
}

void
StrokePaint::
setObject(CSVGObject *)
{
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

#if 0
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
#endif

  gridLayout->addWidget(widthEdit_, 0, 1);
  gridLayout->addWidget(dashEdit_ , 1, 1);
  gridLayout->addWidget(joinEdit_ , 2, 1);
  gridLayout->addWidget(capEdit_  , 3, 1);
  gridLayout->addWidget(mitreEdit_, 4, 1);

  layout->addLayout(gridLayout);

  //------

  auto *defButton = CQUtil::makeLabelWidget<QPushButton>("Set Default", "defButton");

  //connect(defButton, SIGNAL(clicked()), this, SLOT(setDefaultSlot()));

  layout->addWidget(defButton);

  //---

  layout->addStretch();
}

void
StrokeStyle::
setObject(CSVGObject *)
{
}

}
