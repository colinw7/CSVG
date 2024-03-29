#include <CQInkscapeWindow.h>
#include <CQInkscapeCanvas.h>
#include <CQInkscapeMouseToolBar.h>
#include <CQInkscapeModeToolBar.h>
#include <CQInkscapePaletteArea.h>
#include <CQInkscapeStatusBar.h>
#include <CQInkscapeColorBar.h>
#include <CQInkscapeFillStrokePalette.h>
#include <CQInkscapeObjectProperties.h>
#include <CQInkscapeTransformPalette.h>
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

#include <fstream>

#include <svg/fill_current_svg.h>
#include <svg/fill_image_svg.h>
#include <svg/fill_inherit_svg.h>
#include <svg/fill_lgradient_svg.h>
#include <svg/fill_none_svg.h>
#include <svg/fill_rgradient_svg.h>
#include <svg/fill_solid_svg.h>

#include <svg/select_mode_svg.h>
#include <svg/point_select_mode_svg.h>
#include <svg/zoom_mode_svg.h>
#include <svg/rect_mode_svg.h>
#include <svg/ellipse_mode_svg.h>
#include <svg/path_mode_svg.h>
#include <svg/text_mode_svg.h>

#include <svg/select_mode_point_svg.h>
#include <svg/select_mode_rect_svg.h>

#include <svg/mouse_zoom_in_svg.h>
#include <svg/mouse_zoom_out_svg.h>
#include <svg/mouse_zoom_fit_svg.h>

namespace CQInkscape {

Window::
Window()
{
  setObjectName("window");

  //---

  // add menu bar
  menuBar_ = CQUtil::makeWidget<QMenuBar>(this, "menubar");

  //---

  auto *fileMenu   = menuBar_->addMenu("File");
  auto *editMenu   = menuBar_->addMenu("Edit");
  auto *objectMenu = menuBar_->addMenu("Object");
  auto *viewMenu   = menuBar_->addMenu("View");
  auto *helpMenu   = menuBar_->addMenu("Help");

  CQUtil::addAction(fileMenu, "Load...", this, SLOT(loadSlot()));
  CQUtil::addAction(fileMenu, "Save As...", this, SLOT(saveAsSlot()));
  CQUtil::addAction(fileMenu, "Print...", this, SLOT(printSlot()));
  CQUtil::addAction(fileMenu, "Quit", this, SLOT(quitSlot()));

  CQUtil::addAction(editMenu, "Delete");
  CQUtil::addAction(editMenu, "Select All", this, SLOT(selectAllSlot()));
  CQUtil::addAction(editMenu, "Select None", this, SLOT(selectNoneSlot()));
  CQUtil::addAction(editMenu, "Select Parent", this, SLOT(selectParentSlot()));

  CQUtil::addAction(objectMenu, "Ungroup", this, SLOT(ungroupSlot()));

  auto *viewZoomMenu = viewMenu->addMenu("Zoom");

  CQUtil::addAction(viewZoomMenu, "Zoom In"   , this, SLOT(zoomInSlot()));
  CQUtil::addAction(viewZoomMenu, "Zoom Out"  , this, SLOT(zoomOutSlot()));
  CQUtil::addAction(viewZoomMenu, "Zoom Reset", this, SLOT(zoomResetSlot()));

  CQUtil::addAction(viewMenu, "Settings", this, SLOT(settingsSlot()));
  CQUtil::addAction(viewMenu, "Console" , this, SLOT(consoleSlot()));

  CQUtil::addAction(helpMenu, "Help", this, SLOT(helpSlot()));

  //---

  // top mouse toolbar
  mouseToolBar_ = new MouseToolBar(this);

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
  paletteArea_ = new PaletteArea(this);

  fillStrokePalette_       = new FillStrokePalette(this);
  objectPropertiesPalette_ = new ObjectProperties(this);
  transformPalette_        = new TransformPalette(this);

  paletteArea_->addPalette(fillStrokePalette_);
  paletteArea_->addPalette(objectPropertiesPalette_);
  paletteArea_->addPalette(transformPalette_);
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
  int palettesWidth = paletteArea_->sizeHint().width();

  //---

  // place menu bar at top
  int menuBarHeight = fm.height() + 8;

  menuBar_->move(0, 0);
  menuBar_->resize(width(), menuBarHeight);

  menuBar_->show();

  //---

  // place top mouse toolbar underneath menu bar
  int mouseToolBarHeight = fm.height() + 24;

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

  paletteArea_->move(canvasWidth + modeToolBarWidth, topHeight);

  paletteArea_->resize(paletteSize);
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
updateCurrentObject()
{
  CSVG::ObjectList objects;

  svg()->getSelectedObjects(objects);

  for (auto &obj : objects) {
    setCurrentObject(obj);
    return;
  }

  setCurrentObject(nullptr);
}

void
Window::
setCurrentObject(CSVGObject *obj)
{
  currentObj_ = obj;

  mouseToolBar_->updateState();

  fillStrokePalette_      ->setObject(currentObj_);
  objectPropertiesPalette_->setObject(currentObj_);
  transformPalette_       ->setObject(currentObj_);

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
  auto title  = QString("Load SVG File");
  auto cwd    = QString(COSFile::getCurrentDir().c_str());
  auto filter = QString("SVG, (*.svg)");

  auto filenames = QFileDialog::getOpenFileNames(this, title, cwd, filter);
  if (filenames.size() == 0) return;

  QStringListIterator fi(filenames);

  while (fi.hasNext())
    svg()->read(fi.next().toStdString());
}

void
Window::
saveAsSlot()
{
  auto title  = QString("Load SVG File");
  auto cwd    = QString(COSFile::getCurrentDir().c_str());
  auto filter = QString("SVG, (*.svg)");

  auto filename = QFileDialog::getSaveFileName(this, title, cwd, filter);
  if (filename == "") return;

  std::ofstream os(filename.toStdString());

  svg()->print(os, /*hier*/true);
}

void
Window::
printSlot()
{
  auto title  = QString("Print Image");
  auto cwd    = QString(COSFile::getCurrentDir().c_str());
  auto filter = QString("PNG, (*.png)");

  auto filename = QFileDialog::getSaveFileName(this, title, cwd, filter);
  if (filename == "") return;

  canvas_->print(filename);
}

void
Window::
zoomInSlot()
{
  canvas_->zoomIn();
}

void
Window::
zoomOutSlot()
{
  canvas_->zoomOut();
}

void
Window::
zoomResetSlot()
{
  canvas_->zoomReset();
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
consoleSlot()
{
  setConsoleVisible(true);
}

void
Window::
quitSlot()
{
  exit(0);
}

void
Window::
selectAllSlot()
{
  selectAll();
}

void
Window::
selectAll()
{
  CSVG::ObjectList objects;

  svg()->getAllChildren(objects);

  for (auto &obj : objects)
    obj->setSelected(true);

  updateCurrentObject();

  redraw(true);
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

  updateCurrentObject();

  redraw(true);
}

void
Window::
selectParentSlot()
{
  selectParent();
}

void
Window::
selectParent()
{
  CSVG::ObjectList objects;

  svg()->getSelectedObjects(objects);

  CSVG::ObjectList parentObjects;

  for (auto &obj : objects) {
    if (obj->getParent())
      parentObjects.push_back(obj->getParent());

    obj->setSelected(false);
  }

  for (auto &obj : parentObjects)
    obj->setSelected(true);

  updateCurrentObject();

  redraw(true);
}

void
Window::
ungroupSlot()
{
  CSVG::ObjectList objects;

  svg()->getSelectedObjects(objects);

  for (auto &obj : objects)
    obj->ungroupObject();

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

void
Window::
setMode(const Mode &v)
{
  mode_ = v;

  mouseToolBar_->updateState();
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
