#include <CQInkscapeModeToolBar.h>
#include <CQInkscapeWindow.h>

#include <CQImageButton.h>
#include <CQPixmapCache.h>
#include <CQUtil.h>

#include <QVBoxLayout>

namespace CQInkscape {

ModeToolBar::
ModeToolBar(Window *window) :
 QFrame(window), window_(window)
{
  setObjectName("modelToolBar");

  setFrameShape(QFrame::StyledPanel);

  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this);

  auto makeButton = [&](const QString &icon, const QString &desc, const char *slotName) {
    auto *button = new CQImageButton(CQPixmapCacheInst->getIcon(icon));

    button->setToolTip(desc);

    connect(button, SIGNAL(clicked()), this, slotName);

    layout->addWidget(button);

    return button;
  };

  makeButton("SELECT_MODE"      , "Select Object", SLOT(selectSlot()));
  makeButton("POINT_SELECT_MODE", "Select Point" , SLOT(pointSelectSlot()));
  makeButton("ZOOM_MODE"        , "Zoom"         , SLOT(zoomSlot()));
  makeButton("RECT_MODE"        , "Create Rect"  , SLOT(createRectSlot()));
  makeButton("PATH_MODE"        , "Create Path"  , SLOT(createPathSlot()));
  makeButton("TEXT_MODE"        , "Create Text"  , SLOT(createTextSlot()));

  layout->addStretch(1);
}

void
ModeToolBar::
selectSlot()
{
  window_->setMode(Window::Mode::SELECT_OBJECT);
}

void
ModeToolBar::
pointSelectSlot()
{
  window_->setMode(Window::Mode::SELECT_POINT);
}

void
ModeToolBar::
zoomSlot()
{
  window_->setMode(Window::Mode::ZOOM);
}

void
ModeToolBar::
createRectSlot()
{
  window_->setMode(Window::Mode::CREATE_RECT);
}

void
ModeToolBar::
createPathSlot()
{
  window_->setMode(Window::Mode::CREATE_PATH);
}

void
ModeToolBar::
createTextSlot()
{
  window_->setMode(Window::Mode::CREATE_TEXT);
}

}
