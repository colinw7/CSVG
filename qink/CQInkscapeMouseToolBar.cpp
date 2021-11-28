#include <CQInkscapeMouseToolBar.h>
#include <CQInkscapeWindow.h>

#include <CSVGCircle.h>
#include <CSVGEllipse.h>
#include <CSVGRect.h>

#include <CQImageButton.h>
#include <CQPixmapCache.h>
#include <CQLineEdit.h>
#include <CQRealSpin.h>
#include <CQUtil.h>

#include <QStackedWidget>
#include <QLabel>
#include <QHBoxLayout>

namespace CQInkscape {

MouseToolBar::
MouseToolBar(Window *window) :
 QFrame(window), window_(window)
{
  setObjectName("mouseToolBar");

  setFrameShape(QFrame::StyledPanel);

  auto *layout = CQUtil::makeLayout<QHBoxLayout>(this);

  stack_ = CQUtil::makeWidget<QStackedWidget>("stack");

  layout->addWidget(stack_);

  auto makeFrame = [&](const QString &name) {
    auto *frame = CQUtil::makeWidget<QFrame>(name + "Frame");
    (void) CQUtil::makeLayout<QHBoxLayout>(frame);
    stack_->addWidget(frame);
    return frame;
  };

  selectData_.frame  = makeFrame("select");
  pointSelectFrame_  = makeFrame("pointSelect");
  zoomFrame_         = makeFrame("zoom");
  rectData_.frame    = makeFrame("rect");
  ellipseData_.frame = makeFrame("ellipse");
  pathFrame_         = makeFrame("path");
  textFrame_         = makeFrame("text");

  auto makeButton = [&](QFrame *frame, const QString &icon, const QString &desc,
                        const char *slotName=nullptr) {
    auto *button = new CQImageButton(CQPixmapCacheInst->getIcon(icon));

    button->setToolTip(desc);

    if (slotName)
      connect(button, SIGNAL(clicked()), this, slotName);

    frame->layout()->addWidget(button);

    return button;
  };

  auto makeStringEdit = [&](QFrame *frame, const QString &name, const QString &desc,
                            const char *slotName=nullptr) {
    auto *rframe = CQUtil::makeWidget<QFrame>("frame");
    auto *layout = CQUtil::makeLayout<QHBoxLayout>(rframe, 0, 2);

    auto *label = (name != "" ? CQUtil::makeLabelWidget<QLabel>(name, "label") : nullptr);
    auto *edit  = CQUtil::makeWidget<CQLineEdit>("edit");

    if (label)
      layout->addWidget(label);

    layout->addWidget(edit);

    edit->setToolTip(desc);

    if (slotName)
      connect(edit, SIGNAL(valueChanged(const QString &)), this, slotName);

    frame->layout()->addWidget(rframe);

    return edit;
  };

  auto makeRealEdit = [&](QFrame *frame, const QString &name, const QString &desc,
                          const char *slotName=nullptr) {
    auto *rframe = CQUtil::makeWidget<QFrame>("frame");
    auto *layout = CQUtil::makeLayout<QHBoxLayout>(rframe, 0, 2);

    auto *label = (name != "" ? CQUtil::makeLabelWidget<QLabel>(name, "label") : nullptr);
    auto *edit  = CQUtil::makeWidget<CQRealSpin>("edit");

    if (label)
      layout->addWidget(label);

    layout->addWidget(edit);

    edit->setToolTip(desc);

    if (slotName)
      connect(edit, SIGNAL(valueChanged(double)), this, slotName);

    frame->layout()->addWidget(rframe);

    return edit;
  };

  auto addStretch = [&](QFrame *frame) {
    qobject_cast<QBoxLayout *>(frame->layout())->addStretch(1);
  };

  // select
  makeButton(selectData_.frame, "SELECT_MODE_POINT", "Point Select");
  makeButton(selectData_.frame, "SELECT_MODE_RECT" , "Rect Select");

  selectData_.xEdit = makeRealEdit(selectData_.frame, "X", "X"     , SLOT(selectXSlot()));
  selectData_.yEdit = makeRealEdit(selectData_.frame, "Y", "Y"     , SLOT(selectYSlot()));
  selectData_.wEdit = makeRealEdit(selectData_.frame, "W", "Width" , SLOT(selectWidthSlot()));
  selectData_.hEdit = makeRealEdit(selectData_.frame, "H", "Height", SLOT(selectHeightSlot()));

  addStretch(selectData_.frame);

  // point select (path)
  addStretch(pointSelectFrame_);

  // zoom
  makeButton(zoomFrame_, "MOUSE_ZOOM_IN" , "Zoom In" , SLOT(zoomInSlot()));
  makeButton(zoomFrame_, "MOUSE_ZOOM_OUT", "Zoom Out", SLOT(zoomOutSlot()));
  makeButton(zoomFrame_, "MOUSE_ZOOM_FIT", "Zoom Fit", SLOT(zoomFitSlot()));
  addStretch(zoomFrame_);

  // rect
  rectData_.wEdit  = makeRealEdit(rectData_.frame, "W" , "Width" , SLOT(rectWidthSlot()));
  rectData_.hEdit  = makeRealEdit(rectData_.frame, "H" , "Height", SLOT(rectHeightSlot()));
  rectData_.rxEdit = makeRealEdit(rectData_.frame, "Rx", "Horizontal Corner Radius",
                                  SLOT(rectXRadiusSlot()));
  rectData_.ryEdit = makeRealEdit(rectData_.frame, "Ry", "Vertical Corner Radius",
                                  SLOT(rectYRadiusSlot()));
  addStretch(rectData_.frame);

  // ellipse
  ellipseData_.rxEdit    = makeRealEdit(ellipseData_.frame, "Rx", "Horizontal Radius",
                                        SLOT(ellipseXRadiusSlot()));
  ellipseData_.ryEdit    = makeRealEdit(ellipseData_.frame, "Ry", "Vertical Radius",
                                        SLOT(ellipseYRadiusSlot()));
  ellipseData_.startEdit = makeRealEdit(ellipseData_.frame, "Start", "Start Angle",
                                        SLOT(ellipseStartAngleSlot()));
  ellipseData_.endEdit   = makeRealEdit(ellipseData_.frame, "End"  , "End Angle",
                                        SLOT(ellipseEndAngleSlot()));
  addStretch(ellipseData_.frame);

  // path
  addStretch(pathFrame_);

  // text
  makeStringEdit(textFrame_, "", "Font");
  makeStringEdit(textFrame_, "", "Style");
  makeRealEdit  (textFrame_, "", "Size");

  addStretch(textFrame_);
}

void
MouseToolBar::
updateState()
{
  auto mode = window_->mode();

  int ind = stack_->currentIndex();

  int ind1 = 0;

  switch (mode) {
    case Window::Mode::SELECT_OBJECT : ind1 = 0; break;
    case Window::Mode::SELECT_POINT  : ind1 = 1; break;
    case Window::Mode::ZOOM          : ind1 = 2; break;
    case Window::Mode::CREATE_RECT   : ind1 = 3; break;
    case Window::Mode::CREATE_ELLIPSE: ind1 = 4; break;
    case Window::Mode::CREATE_PATH   : ind1 = 5; break;
    case Window::Mode::CREATE_TEXT   : ind1 = 6; break;
  }

  if (ind1 != ind)
    stack_->setCurrentIndex(ind1);

  //---

  auto *object = window()->currentObject();

  if      (mode == Window::Mode::SELECT_OBJECT) {
    bool enabled = object;

    CBBox2D bbox;

    if (object && object->getFlatTransformedBBox(bbox)) {
      selectData_.xEdit->setValue(bbox.getXMin());
      selectData_.yEdit->setValue(bbox.getYMin());
      selectData_.wEdit->setValue(bbox.getWidth());
      selectData_.hEdit->setValue(bbox.getHeight());
    }

    selectData_.xEdit->setEnabled(enabled);
    selectData_.yEdit->setEnabled(enabled);
    selectData_.wEdit->setEnabled(enabled);
    selectData_.hEdit->setEnabled(enabled);
  }
  else if (mode == Window::Mode::SELECT_POINT) {
  }
  else if (mode == Window::Mode::ZOOM) {
  }
  else if (mode == Window::Mode::CREATE_RECT) {
    auto *rect = dynamic_cast<CSVGRect *>(object);
    if (! rect) return;

    rectData_.wEdit ->setValue(rect->getWidth ().pxValue());
    rectData_.hEdit ->setValue(rect->getHeight().pxValue());
    rectData_.rxEdit->setValue(rect->getRX());
    rectData_.ryEdit->setValue(rect->getRY());
  }
  else if (mode == Window::Mode::CREATE_ELLIPSE) {
    auto *ellipse = dynamic_cast<CSVGEllipse *>(object);
    auto *circle  = dynamic_cast<CSVGCircle  *>(object);

    if      (ellipse) {
      ellipseData_.rxEdit->setValue(ellipse->getRadiusX().pxValue());
      ellipseData_.ryEdit->setValue(ellipse->getRadiusY().pxValue());
    }
    else if (circle) {
      ellipseData_.rxEdit->setValue(circle->getRadius().pxValue());
      ellipseData_.ryEdit->setValue(circle->getRadius().pxValue());
    }
  }
  else if (mode == Window::Mode::CREATE_PATH) {
  }
  else if (mode == Window::Mode::CREATE_TEXT) {
  }
}

void
MouseToolBar::
selectXSlot()
{
}

void
MouseToolBar::
selectYSlot()
{
}

void
MouseToolBar::
selectWidthSlot()
{
}

void
MouseToolBar::
selectHeightSlot()
{
}

void
MouseToolBar::
zoomInSlot()
{
}

void
MouseToolBar::
zoomOutSlot()
{
}

void
MouseToolBar::
zoomFitSlot()
{
}

void
MouseToolBar::
rectWidthSlot()
{
  auto *object = window()->currentObject();

  auto *rect = dynamic_cast<CSVGRect *>(object);
  if (! rect) return;

  rect->setWidth(rectData_.wEdit->value());

  window()->redraw(/*update*/true);
}

void
MouseToolBar::
rectHeightSlot()
{
  auto *object = window()->currentObject();

  auto *rect = dynamic_cast<CSVGRect *>(object);
  if (! rect) return;

  rect->setHeight(rectData_.hEdit->value());

  window()->redraw(/*update*/true);
}

void
MouseToolBar::
rectXRadiusSlot()
{
  auto *object = window()->currentObject();

  auto *rect = dynamic_cast<CSVGRect *>(object);
  if (! rect) return;

  rect->setRX(rectData_.rxEdit->value());

  window()->redraw(/*update*/true);
}

void
MouseToolBar::
rectYRadiusSlot()
{
  auto *object = window()->currentObject();

  auto *rect = dynamic_cast<CSVGRect *>(object);
  if (! rect) return;

  rect->setRY(rectData_.ryEdit->value());

  window()->redraw(/*update*/true);
}

void
MouseToolBar::
ellipseXRadiusSlot()
{
  auto *object = window()->currentObject();

  auto *ellipse = dynamic_cast<CSVGEllipse *>(object);
  if (! ellipse) return;

  ellipse->setRadiusX(ellipseData_.rxEdit->value());

  window()->redraw(/*update*/true);
}

void
MouseToolBar::
ellipseYRadiusSlot()
{
  auto *object = window()->currentObject();

  auto *ellipse = dynamic_cast<CSVGEllipse *>(object);
  if (! ellipse) return;

  ellipse->setRadiusY(ellipseData_.ryEdit->value());

  window()->redraw(/*update*/true);
}

void
MouseToolBar::
ellipseStartAngleSlot()
{
}

void
MouseToolBar::
ellipseEndAngleSlot()
{
}

}
