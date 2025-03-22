#include <CQSVGWindow.h>
#include <CQSVG.h>
#include <CQSVGCanvas.h>
#include <CQSVGPropertiesDlg.h>
#include <CQSVGBufferView.h>
#include <CQSVGJSDialog.h>

#include <CQSVGAnchor.h>
#include <CQSVGAnimate.h>
#include <CQSVGAnimateColor.h>
#include <CQSVGAnimateMotion.h>
#include <CQSVGAnimateTransform.h>
#include <CQSVGBlock.h>
#include <CQSVGCircle.h>
#include <CQSVGClipPath.h>
#include <CQSVGDefs.h>
#include <CQSVGDesc.h>
#include <CQSVGEllipse.h>
#include <CQSVGFeBlend.h>
#include <CQSVGFeColorMatrix.h>
#include <CQSVGFeComposite.h>
#include <CQSVGFeDiffuseLighting.h>
#include <CQSVGFeDisplacementMap.h>
#include <CQSVGFeDistantLight.h>
#include <CQSVGFeFlood.h>
#include <CQSVGFeGaussianBlur.h>
#include <CQSVGFeImage.h>
#include <CQSVGFeMerge.h>
#include <CQSVGFeMergeNode.h>
#include <CQSVGFeMorphology.h>
#include <CQSVGFeOffset.h>
#include <CQSVGFePointLight.h>
#include <CQSVGFeSpecularLighting.h>
#include <CQSVGFeSpotLight.h>
#include <CQSVGFeTile.h>
#include <CQSVGFeTurbulence.h>
#include <CQSVGFilter.h>
#include <CQSVGGroup.h>
#include <CQSVGImage.h>
#include <CQSVGLine.h>
#include <CQSVGLinearGradient.h>
#include <CQSVGMarker.h>
#include <CQSVGMask.h>
#include <CQSVGPath.h>
#include <CQSVGPattern.h>
#include <CQSVGPolygon.h>
#include <CQSVGPolyLine.h>
#include <CQSVGRadialGradient.h>
#include <CQSVGRect.h>
#include <CQSVGStop.h>
#include <CQSVGText.h>
#include <CQSVGTextPath.h>
#include <CQSVGTitle.h>
#include <CQSVGTSpan.h>
#include <CQSVGUse.h>
#include <CSVGImageData.h>

#include <CQSVGPathPart.h>
#include <CQSVGRenderer.h>

#include <CQPropertyTree.h>
#include <CQPropertyItem.h>
#include <CQPixmapCache.h>

#include <QApplication>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QFileDialog>

#include <fstream>
#include <iostream>

#include <svg/next_svg.h>
#include <svg/prev_svg.h>
#include <svg/properties_svg.h>
#include <svg/buffers_svg.h>
#include <svg/javascript_svg.h>
#include <svg/play_svg.h>
#include <svg/pause_svg.h>
#include <svg/play_one_svg.h>
#include <svg/rewind_one_svg.h>
#include <svg/busy_svg.h>
#include <svg/ready_svg.h>

CQSVGWindow::
CQSVGWindow() :
 QMainWindow()
{
  setObjectName("window");

  //---

  auto *frame  = CQUtil::makeWidget<QFrame>("frame");
  auto *layout = CQUtil::makeLayout<QHBoxLayout>(frame, 2, 2);

  //---

  canvas_ = new CQSVGCanvas(this);

  //---

#if 0
  auto *splitter = CQWidgetUtil::makeWidget<QSplitter>("spliiter");

  splitter->addWidget(canvas_);

  auto *rframe  = CQWidgetUtil::makeWidget<QFrame>("rframe");
  aito *rlayout = CQWidgetUtil::makeLayout<QVBoxLayout>(rframe, 2, 2);

  propTree_ = new CQPropertyTree;

  connect(propTree_, SIGNAL(valueChanged(QObject *, const QString &)),
          this, SLOT(redraw()));

  connect(propTree_, SIGNAL(itemSelected(QObject *, const QString &)),
          this, SLOT(itemSelectedSlot(QObject *, const QString &)));

  //connect(propTree_, SIGNAL(menuExec(QObject *, const QPoint &)),
  //        this, SLOT(treeMenuExec(QObject *, const QPoint &)));

  rframe->setMaximumWidth(400);

  rlayout->addWidget(propTree_);

  splitter->addWidget(rframe);

  layout->addWidget(splitter);
#else
  layout->addWidget(canvas_);
#endif

  setCentralWidget(frame);

  //---

  // add menus
  addMenus();

  //---

  // add status bar widgets
  posLabel_ = new QLabel;
  posLabel_->setObjectName("posLabel");

  statusBar()->addPermanentWidget(posLabel_);

  zoomLabel_ = new QLabel;
  zoomLabel_->setObjectName("zoomLabel");

  statusBar()->addPermanentWidget(zoomLabel_);

  busyButton_ = new QToolButton;

  busyButton_->setIcon(CQPixmapCacheInst->getIcon("READY"));

  statusBar()->addPermanentWidget(busyButton_);

  showPos(QPoint(0, 0), QPointF(0, 0));

  //---

  // add toolbars
  auto *viewToolBar = new QToolBar(this);

  viewToolBar->addAction(prevAction_);
  viewToolBar->addAction(nextAction_);
  viewToolBar->addAction(propertiesAction_);
  viewToolBar->addAction(buffersAction_);
  viewToolBar->addAction(jsAction_);

  addToolBar(viewToolBar);

  //---

  timeEdit_ = new QLineEdit;

  connect(timeEdit_, SIGNAL(editingFinished()), this, SLOT(timeSlot()));

  animateToolBar_ = new QToolBar(this);

  animateToolBar_->addAction(playAction_);
  animateToolBar_->addAction(pauseAction_);
  animateToolBar_->addAction(stepAction_);
  animateToolBar_->addAction(bstepAction_);
  animateToolBar_->addWidget(timeEdit_);

  addToolBar(animateToolBar_);

  //---

  updateState();
}

void
CQSVGWindow::
addMenus()
{
  auto *fileMenu = menuBar()->addMenu("&File");

  //---

  auto *printAction = new QAction("&Print", this);

  connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

  fileMenu->addAction(printAction);

  //---

  auto *printFlatAction = new QAction("Print &Flat", this);

  connect(printFlatAction, SIGNAL(triggered()), this, SLOT(printFlat()));

  fileMenu->addAction(printFlatAction);

  //---

  auto *exitAction = new QAction("&Exit", this);

  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

  fileMenu->addAction(exitAction);

  //------

  auto *viewMenu = menuBar()->addMenu("&View");

  //---

  prevAction_ = new QAction("&Prev", this);
  prevAction_->setIcon(CQPixmapCacheInst->getIcon("PREV"));

  connect(prevAction_, SIGNAL(triggered()), this, SLOT(prevFile()));

  viewMenu->addAction(prevAction_);

  //---

  nextAction_ = new QAction("&Next", this);
  nextAction_->setIcon(CQPixmapCacheInst->getIcon("NEXT"));

  connect(nextAction_, SIGNAL(triggered()), this, SLOT(nextFile()));

  viewMenu->addAction(nextAction_);

  //---

  propertiesAction_ = new QAction("&Properties", this);
  propertiesAction_->setIcon(CQPixmapCacheInst->getIcon("PROPERTIES"));

  connect(propertiesAction_, SIGNAL(triggered()), this, SLOT(showProperties()));

  viewMenu->addAction(propertiesAction_);

  //---

  buffersAction_ = new QAction("&Buffers", this);
  buffersAction_->setIcon(CQPixmapCacheInst->getIcon("BUFFERS"));

  connect(buffersAction_, SIGNAL(triggered()), this, SLOT(showBuffers()));

  viewMenu->addAction(buffersAction_);

  //---

  jsAction_ = new QAction("&JavaScript", this);
  jsAction_->setIcon(CQPixmapCacheInst->getIcon("JAVASCRIPT"));

  connect(jsAction_, SIGNAL(triggered()), this, SLOT(showJSDialog()));

  viewMenu->addAction(jsAction_);

  //---

  animateMenu_ = menuBar()->addMenu("&Animate");

  playAction_ = new QAction("&Play", this);
  playAction_->setIcon(CQPixmapCacheInst->getIcon("PLAY"));

  connect(playAction_, SIGNAL(triggered()), this, SLOT(playSlot()));

  animateMenu_->addAction(playAction_);

  pauseAction_ = new QAction("Pa&use", this);
  pauseAction_->setIcon(CQPixmapCacheInst->getIcon("PAUSE"));

  connect(pauseAction_, SIGNAL(triggered()), this, SLOT(pauseSlot()));

  animateMenu_->addAction(pauseAction_);

  stepAction_ = new QAction("Step &Forward", this);
  stepAction_->setIcon(CQPixmapCacheInst->getIcon("PLAY_ONE"));

  connect(stepAction_, SIGNAL(triggered()), this, SLOT(stepSlot()));

  animateMenu_->addAction(stepAction_);

  bstepAction_ = new QAction("Step &Backward", this);
  bstepAction_->setIcon(CQPixmapCacheInst->getIcon("REWIND_ONE"));

  connect(bstepAction_, SIGNAL(triggered()), this, SLOT(bstepSlot()));

  animateMenu_->addAction(bstepAction_);
}

CQSVGWindow::
~CQSVGWindow()
{
}

CQSVG *
CQSVGWindow::
svg() const
{
  return canvas()->svg();
}

void
CQSVGWindow::
addFiles(const Files &files)
{
  for (const auto &file : files)
    addFile(file);
}

void
CQSVGWindow::
addFile(const QString &file)
{
  if (! CFile::isRegular(file.toStdString())) {
    std::cerr << "Invalid file \"" << file.toStdString() << "\"" << std::endl;
    return;
  }

  files_.push_back(file);
}

void
CQSVGWindow::
addRootIds(const Ids &ids)
{
  rootIds_  = ids;

  if (! rootIds_.empty()) {
    rootInd_ = 0;

    setRootId(rootIds_[size_t(rootInd_)]);
  }
  else {
    setRootId("");

    rootInd_ = -1;
  }
}

void
CQSVGWindow::
nextFile()
{
  if      (files_.size() > 1) {
    if (fileInd_ < 0)
      fileInd_ = 0;
    else {
      ++fileInd_;

      if (fileInd_ >= int(files_.size()))
        fileInd_ = int(files_.size() - 1);
    }

    loadFile();
  }
  else if (rootIds_.size() > 1) {
    if (rootInd_ < 0)
      rootInd_ = 0;
    else {
      ++rootInd_;

      if (rootInd_ >= int(rootIds_.size()))
        rootInd_ = int(rootIds_.size() - 1);
    }

    setRootId(rootIds_[size_t(rootInd_)]);

    updateAltRoot();

    updateTitle();

    updateState();

    redraw();
  }
}

void
CQSVGWindow::
prevFile()
{
  if      (files_.size() > 1) {
    if (fileInd_ < 0)
      fileInd_ = 0;
    else {
      --fileInd_;

      if (fileInd_ < 0)
        fileInd_ = 0;
    }

    loadFile();
  }
  else if (rootIds_.size() > 1) {
    if (rootInd_ < 0)
      rootInd_ = 0;
    else {
      --rootInd_;

      if (rootInd_ < 0)
        rootInd_ = 0;
    }

    setRootId(rootIds_[size_t(rootInd_)]);

    updateAltRoot();

    updateTitle();

    updateState();

    redraw();
  }
}

void
CQSVGWindow::
loadFile()
{
  if (! isInitialized())
    setInitialized(true);

  //---

  if (fileInd_ < 0)
    fileInd_ = 0;

  updateTitle();

  updateState();

  if (fileInd_ < 0 || fileInd_ >= int(files_.size()))
    return;

  auto *svg = this->svg();

  auto filename = files_[size_t(fileInd_)].toStdString();

  if      (isImage()) {
    svg->read(filename);

    auto *renderer = new CQSVGRenderer;

    renderer->setSize(svg->getIWidth(), svg->getIHeight());

    svg->setRenderer(renderer);

    //---

    svg->draw();

    //---

    CFile file(filename);

    std::string base = file.getBase();

    std::string name;

    if (imageDir() != "")
      name = CStrUtil::strprintf("%s/svg_%s.png", imageDir().toLatin1().constData(), base.c_str());
    else
      name = CStrUtil::strprintf("svg_%s.png", base.c_str());

    renderer->getImage()->write(name, CFILE_TYPE_IMAGE_PNG);

    delete renderer;

    svg->setRenderer(nullptr);
  }
  else if (isPrint()) {
    svg->print(std::cout, true);
  }
  else {
    svg->stopTimer();

    svg->setTime(0);

    svg->read(filename);

    updateAltRoot();

    addProperties();

    loadCSS();

    if (svg->hasAnimation())
      svg->startTimer();

    updateState();
  }

  redraw();
}

void
CQSVGWindow::
updateTitle()
{
  if (fileInd_ < 0 || fileInd_ >= int(files_.size()))
    return;

  auto filename = files_[size_t(fileInd_)];

  auto title = filename;

  if (rootId() != "")
    title += " (" + rootId() + ")";

  setWindowTitle(title);
}

void
CQSVGWindow::
updateAltRoot()
{
  if (rootId() != "") {
    auto *svg = this->svg();

    svg->setAltRoot(nullptr);

    CSVGObject::ObjectArray objects;

    svg->getChildrenOfId(rootId().toStdString(), objects);

    if      (objects.empty())
      std::cerr << "No objects of id '" << rootId().toStdString() << "'\n";
    else {
      if (objects.size() > 1)
        std::cerr << "Multiple objects match id '" << rootId().toStdString() << "'\n";

      svg->setAltRoot(objects[0]);
    }
  }
}

void
CQSVGWindow::
updateState()
{
  if      (files_.size() > 1) {
    prevAction_->setEnabled(fileInd_ > 0);
    nextAction_->setEnabled(fileInd_ < int(files_.size()) - 1);
  }
  else if (rootIds_.size() > 1) {
    prevAction_->setEnabled(rootInd_ > 0);
    nextAction_->setEnabled(rootInd_ < int(rootIds_.size()) - 1);
  }

  auto *svg = this->svg();

  if (svg->hasAnimation()) {
    animateMenu_   ->menuAction()->setVisible(true);
    animateToolBar_->setVisible(true);

    playAction_ ->setEnabled(! svg->isAnimating());
    pauseAction_->setEnabled(svg->isAnimating());
    stepAction_ ->setEnabled(! svg->isAnimating());
    bstepAction_->setEnabled(! svg->isAnimating());
  }
  else {
    animateMenu_   ->menuAction()->setVisible(false);
    animateToolBar_->setVisible(false);
  }
}

void
CQSVGWindow::
loadCSS()
{
  if (propertiesDlg_)
    propertiesDlg_->loadCSS();
}

void
CQSVGWindow::
addProperties()
{
  if (propertiesDlg_)
    propTree_ = propertiesDlg_->propertiesTree();

  if (! propTree_)
    return;

  auto *svg = this->svg();

  propTree_->clear();

  propTree_->addProperty("", svg, "background");
  propTree_->addProperty("", svg, "animating");
  propTree_->addProperty("", svg, "checkerboard");
  propTree_->addProperty("", svg, "showGradient");
  propTree_->addProperty("", svg, "showFilterBox");
  propTree_->addProperty("", svg, "ignoreFilter");

  auto *block = svg->getRoot();

  std::string id = block->getId();

  if (id == "")
    id = block->getUniqueName();
  else
    id += "(" + block->getObjName() + ")";

  addObjectToTree(id, block);
}

void
CQSVGWindow::
addObjectToTree(const std::string &name, CSVGObject *obj)
{
  //auto *qobj = dynamic_cast<QObject *>(obj);

  auto *qobj = dynamic_cast<CQSVGObject *>(obj);

  if (qobj) {
    auto *qtitle = dynamic_cast<CQSVGTitle *>(qobj);

    if (qtitle)
      setWindowTitle(qtitle->getQText());

    //---

    qobj->addProperties(propTree_, name);
  }

  for (const auto &o : obj->children()) {
    std::string id = o->getId();

    if (id == "")
      id = o->getUniqueName();
    else
      id += "(" + o->getObjName() + ")";

    std::string name1 = name + "/" + id;

    addObjectToTree(name1, o);
  }

  for (const auto &o : obj->getAnimation().objects()) {
    std::string id = o->getId();

    if (id == "")
      id = o->getUniqueName();
    else
      id += "(" + o->getObjName() + ")";

    std::string name1 = name + "/" + id;

    addObjectToTree(name1, o);
  }
}

void
CQSVGWindow::
itemSelectedSlot(QObject *obj, const QString & /*path*/)
{
  deselectAllObjects();

  //std::cerr << "Object \'" << qobject->objectName().toStdString() << "\'" << std::endl;

  auto *qobj = dynamic_cast<CQSVGObject *>(obj);

  if (qobj)
    qobj->setSelected(true);

  //CQUtil::setProperty(obj, "selected", QVariant(true));

  canvas_->drawSelected();
}

void
CQSVGWindow::
showPos(const QPoint &ppos, const QPointF &wpos)
{
  auto ptext = QString("%1,%2").arg(ppos.x()).arg(ppos.y());
  auto wtext = QString("%1,%2").arg(wpos.x()).arg(wpos.y());

  posLabel_->setText(QString("P(%1) W(%2)").arg(ptext).arg(wtext));

  zoomLabel_->setText(QString("%1").arg(canvas_->scale()));
}

void
CQSVGWindow::
deselectAllObjects()
{
  auto *svg = this->svg();

  auto *block = svg->getRoot();

  deselectObjects(block);
}

void
CQSVGWindow::
deselectObjects(CSVGObject *obj)
{
  auto *qobj = dynamic_cast<CQSVGObject *>(obj);

  if (qobj)
    qobj->setSelected(false);

  for (auto &c : obj->children())
    deselectObjects(c);
}

void
CQSVGWindow::
redraw()
{
  canvas_->redraw();

  canvas_->update();
}

void
CQSVGWindow::
print()
{
  auto *svg = this->svg();

  auto cdir = QDir::currentPath();

  auto file = QFileDialog::getSaveFileName(this, "Specify Output File", cdir, "*.svg");

  std::ofstream os(file.toStdString());

  svg->print(os, true);
}

void
CQSVGWindow::
printFlat()
{
  auto *svg = this->svg();

  auto cdir = QDir::currentPath();

  auto file = QFileDialog::getSaveFileName(this, "Specify Output File", cdir, "*.svg");

  std::ofstream os(file.toStdString());

  svg->printFlat(os, /*force*/true);
}

void
CQSVGWindow::
close()
{
  delete this;
}

void
CQSVGWindow::
showProperties()
{
  if (! propertiesDlg_) {
    propertiesDlg_ = new CQSVGPropertiesDlg(this);

    addProperties();

    loadCSS();
  }

  propertiesDlg_->show();

  propertiesDlg_->raise();
}

void
CQSVGWindow::
showBuffers()
{
  auto *svg = this->svg();

  if (! bufferView_)
    bufferView_ = new CQSVGBufferView(svg);

  bufferView_->show();

  bufferView_->raise();
}

void
CQSVGWindow::
showJSDialog()
{
  if (! jsDlg_) {
    jsDlg_ = new CQSVGJSDialog(this);
  }

  jsDlg_->show();

  jsDlg_->raise();
}

void
CQSVGWindow::
playSlot()
{
  auto *svg = this->svg();

  svg->startTimer();

  updateState();
}

void
CQSVGWindow::
pauseSlot()
{
  auto *svg = this->svg();

  svg->stopTimer();

  updateState();
}

void
CQSVGWindow::
stepSlot()
{
  auto *svg = this->svg();

  svg->stepTimer();
}

void
CQSVGWindow::
bstepSlot()
{
  auto *svg = this->svg();

  svg->bstepTimer();
}

void
CQSVGWindow::
timeSlot()
{
  auto *svg = this->svg();

  bool ok;

  double t = timeEdit_->text().toDouble(&ok);

  if (ok)
    svg->setTime(t);
}

void
CQSVGWindow::
setTime(double t)
{
  timeEdit_->setText(QString("%1").arg(t));
}

void
CQSVGWindow::
startBusy()
{
  startTime_ = CHRTime::getTime();
  busyTime_  = startTime_;

  busyButton_->setIcon(CQPixmapCacheInst->getIcon("BUSY"));
}

void
CQSVGWindow::
endBusy()
{
  endTime_ = CHRTime::getTime();

  busyButton_->setIcon(CQPixmapCacheInst->getIcon("READY"));

  auto dtime = CHRTime::diffTime(startTime_, endTime_);

  std::stringstream sstr;

  sstr << dtime;

  busyButton_->setToolTip(QString("Draw Time: %1").arg(sstr.str().c_str()));
}

void
CQSVGWindow::
updateBusy()
{
  busyButton_->update();

  auto currentTime = CHRTime::getTime();

  auto dtime = CHRTime::diffTime(busyTime_, currentTime);

  if (dtime.getMSecs() > 100) {
    busyTime_ = currentTime;

    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
  }
}

QSize
CQSVGWindow::
sizeHint() const
{
  double w = CScreenUnitsMgrInst->screenWidth ();
  double h = CScreenUnitsMgrInst->screenHeight();

  if (w > h)
    return QSize(int(0.50*w), int(0.75*h));
  else
    return QSize(int(0.50*w), int(0.50*h));
}
