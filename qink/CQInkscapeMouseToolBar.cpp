#include <CQInkscapeMouseToolBar.h>
#include <CQInkscapeWindow.h>

#include <CQImageButton.h>
#include <CQPixmapCache.h>
#include <CQUtil.h>

#include <QStackedWidget>
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

  selectFrame_      = makeFrame("select");
  pointSelectFrame_ = makeFrame("pointSelect");
  zoomFrame_        = makeFrame("zoom");
  rectFrame_        = makeFrame("rect");
  pathFrame_        = makeFrame("path");
  textFrame_        = makeFrame("text");

  auto makeButton = [&](QFrame *frame, const QString &icon, const QString &desc,
                        const char *slotName) {
    auto *button = new CQImageButton(CQPixmapCacheInst->getIcon(icon));

    button->setToolTip(desc);

    connect(button, SIGNAL(clicked()), this, slotName);

    frame->layout()->addWidget(button);

    return button;
  };

  auto addStretch = [&](QFrame *frame) {
    qobject_cast<QBoxLayout *>(frame->layout())->addStretch(1);
  };

  makeButton(zoomFrame_, "MOUSE_ZOOM_IN" , "Zoom In" , SLOT(zoomInSlot()));
  makeButton(zoomFrame_, "MOUSE_ZOOM_OUT", "Zoom Out", SLOT(zoomOutSlot()));
  makeButton(zoomFrame_, "MOUSE_ZOOM_FIT", "Zoom Fit", SLOT(zoomFitSlot()));

  addStretch(selectFrame_);
  addStretch(pointSelectFrame_);
  addStretch(zoomFrame_);
  addStretch(rectFrame_);
  addStretch(pathFrame_);
  addStretch(textFrame_);
}

void
MouseToolBar::
updateState()
{
  auto mode = window_->mode();

  int ind = 0;

  switch (mode) {
    case Window::Mode::SELECT_OBJECT: ind = 0; break;
    case Window::Mode::SELECT_POINT : ind = 1; break;
    case Window::Mode::ZOOM         : ind = 2; break;
    case Window::Mode::CREATE_RECT  : ind = 3; break;
    case Window::Mode::CREATE_PATH  : ind = 4; break;
    case Window::Mode::CREATE_TEXT  : ind = 5; break;
  }

  stack_->setCurrentIndex(ind);
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

}
