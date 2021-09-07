#include <CQInkscapeWindow.h>
#include <CQInkscapeCanvas.h>
#include <CQInkscapeModeToolBar.h>
#include <CQInkscapeStatusBar.h>
#include <CQInkscapeColorBar.h>
#include <CQInkscapeFillStroke.h>
#include <CQInkscapeObjectProperties.h>
#include <CQInkscapeConsole.h>
#include <CQInkscapeSettings.h>

#include <CQUtil.h>
#include <CQSVGRenderer.h>
#include <CQSVGBufferView.h>
#include <CSVG.h>
#include <COSFile.h>

#include <QHBoxLayout>
#include <QMenuBar>
#include <QLabel>
#include <QFileDialog>

#include <svg/fill_current_svg.h>
#include <svg/fill_image_svg.h>
#include <svg/fill_inherit_svg.h>
#include <svg/fill_lgradient_svg.h>
#include <svg/fill_none_svg.h>
#include <svg/fill_rgradient_svg.h>
#include <svg/fill_solid_svg.h>
#include <svg/flat_svg.h>
#include <svg/path_svg.h>
#include <svg/point_select_svg.h>
#include <svg/rect_svg.h>
#include <svg/select_svg.h>
#include <svg/text_svg.h>
#include <svg/zoom_svg.h>

namespace CQInkscape {

Window::
Window()
{
  setObjectName("window");

  //---

  // add menu bar
  menuBar_ = CQUtil::makeWidget<QMenuBar>(this, "menubar");

  //---

  auto addAction = [&](QMenu *menu, const QString &name, const char *slotName) {
    auto *action = new QAction(name, menu);

    connect(action, SIGNAL(triggered()), this, slotName);

    menu->addAction(action);

    return action;
  };

  auto *fileMenu   = menuBar_->addMenu("File");
  auto *selectMenu = menuBar_->addMenu("Select");
  auto *helpMenu   = menuBar_->addMenu("Help");

  addAction(fileMenu, "Load"    , SLOT(loadSlot()));
  addAction(fileMenu, "Settings", SLOT(settingsSlot()));
  addAction(fileMenu, "Quit"    , SLOT(quitSlot()));

  addAction(selectMenu, "Select None", SLOT(selectNoneSlot()));

  addAction(helpMenu, "Help", SLOT(helpSlot()));

  //---

  // top mouse toolbar
  mouseToolBar_ = CQUtil::makeWidget<QFrame>(this, "mouseToolBar");

  // left mode toolbar
  modeToolBar_ = new ModeToolBar(this);

  // bottom color bar
  colorBar_ = new ColorBar(this);

  // bottom status bar
  statusBar_ = new StatusBar(this);

  //---

  // central canvas widget
  canvas_ = new Canvas(this);

  //---

  // palettes
  palettesArea_   = CQUtil::makeWidget<QFrame>(this, "palettesArea");
  palettesLayout_ = CQUtil::makeLayout<QVBoxLayout>(palettesArea_);

  fillStrokePalette_       = new FillStroke(this);
  objectPropertiesPalette_ = new ObjectProperties(this);

  palettes_.push_back(fillStrokePalette_);
  palettes_.push_back(objectPropertiesPalette_);

  palettesLayout_->addWidget(fillStrokePalette_);
  palettesLayout_->addWidget(objectPropertiesPalette_);
}

void
Window::
resizeEvent(QResizeEvent *)
{
  updatePlacement();
}

void
Window::
updatePlacement()
{
  QFontMetrics fm(font());

  //---

  // get width of palettes (TODO: palette sides)
  int palettesWidth = 0;

  for (const auto &palette : palettes_)
    palettesWidth += palette->sizeHint().width();

  //---

  // place menu bar at top
  int menuBarHeight = fm.height() + 8;

  menuBar_->move(0, 0);
  menuBar_->resize(width(), menuBarHeight);

  menuBar_->show();

  //---

  // place top mouse toolbar underneath menu bar
  int mouseToolBarHeight = fm.height() + 8;

  mouseToolBar_->move(0, menuBarHeight);
  mouseToolBar_->resize(width(), mouseToolBarHeight);

  mouseToolBar_->show();

  //---

  // place status bar on bottom
  int statusBarHeight = 2*fm.height();

  statusBar_->move(0, height() - statusBarHeight);
  statusBar_->resize(width(), statusBarHeight);

  statusBar_->show();

  //---

  // place color bar above status bar
  int colorBarHeight = fm.height() + 8;

  colorBar_->move(0, height() - statusBarHeight - colorBarHeight);
  colorBar_->resize(width(), colorBarHeight);

  colorBar_->show();

  //---

  int topHeight    = menuBarHeight + mouseToolBarHeight;
  int bottomHeight = statusBarHeight + colorBarHeight;

  int centerHeight = height() - topHeight - bottomHeight;

  //---

  // place toolbar on left
  int modeToolBarWidth = fm.height() + 8;

  modeToolBar_->move(0, topHeight);
  modeToolBar_->resize(modeToolBarWidth, centerHeight);

  modeToolBar_->show();

  //---

  int canvasWidth = width() - palettesWidth - modeToolBarWidth;

  canvas_->move(modeToolBarWidth, topHeight);

  canvas_->resize(canvasWidth, centerHeight);

  canvas_->show();

  //---

  auto paletteSize = QSize(palettesWidth, centerHeight);

  palettesArea_->move(canvasWidth, topHeight);

  palettesArea_->resize(paletteSize);

  for (const auto &palette : palettes_)
    palette->show();
}

CSVG *
Window::
svg() const
{
  return canvas_->svg();
}

void
Window::
addFiles(const QStringList &files)
{
  if (! files.length())
    return;

  currentFile_ = files[0];
  currentObj_  = nullptr;

  svg()->read(currentFile_.toStdString());

  updateStatus();
}

void
Window::
setCurrentObject(CSVGObject *obj)
{
  currentObj_ = obj;

  fillStrokePalette_->setObject(currentObj_);

  updateStatus();
}

void
Window::
setMousePos(const CPoint2D &p)
{
  statusBar_->setMousePos(p);
}

void
Window::
updateStatus()
{
  statusBar_->updateCurrent();
}

void
Window::
redraw(bool update)
{
  canvas_->redraw(update);
}

void
Window::
loadSlot()
{
  QString title  = "Load SVG File";
  QString cwd    = QString(COSFile::getCurrentDir().c_str());
  QString filter = "SVG, (*.svg)";

  auto filenames = QFileDialog::getOpenFileNames(this, title, cwd, filter);

  if (filenames.size() == 0)
    return;

  QStringListIterator fi(filenames);

  while (fi.hasNext())
    svg()->read(fi.next().toStdString());
}

void
Window::
settingsSlot()
{
  if (! settings_)
    settings_ = new Settings(this);

  settings_->updateState();

  settings_->show();
}

void
Window::
quitSlot()
{
  exit(0);
}

void
Window::
selectNoneSlot()
{
  selectNone();
}

void
Window::
selectNone()
{
  CSVG::ObjectList objects;

  svg()->getSelectedObjects(objects);

  for (auto &obj : objects)
    obj->setSelected(false);

  redraw(true);
}

void
Window::
helpSlot()
{
}

void
Window::
setCheckerboard(bool b)
{
  checkerboard_ = b;

  redraw(/*update*/true);
}

void
Window::
setShowViewBox(bool b)
{
  showViewBox_ = b;

  redraw(/*update*/true);
}

//---

bool
Window::
isConsoleVisible() const
{
  return (console_ && console_->isVisible());
}

void
Window::
setConsoleVisible(bool visible)
{
  if (! console_) {
    if (! visible)
      return;

    console_ = new Console(this);
  }

  console_->setVisible(visible);
}

//---

bool
Window::
isBufferViewVisible() const
{
  return (bufferView_ && bufferView_->isVisible());
}

void
Window::
setBufferViewVisible(bool visible)
{
  if (! bufferView_) {
    if (! visible)
      return;

    bufferView_ = new CQSVGBufferView(svg());
  }

  bufferView_->setVisible(visible);
}

//---

void
Window::
loadTclFile(const QString &file)
{
  if (! console_)
    console_ = new Console(this);

  console_->loadTclFile(file);
}

}
