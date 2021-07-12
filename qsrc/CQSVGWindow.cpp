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

  svg_ = new CQSVG(this);

  //---

  QWidget *frame = new QWidget;
  frame->setObjectName("frame");

  QHBoxLayout *layout = new QHBoxLayout(frame);
  layout->setMargin(2); layout->setSpacing(2);

  canvas_ = new CQSVGCanvas(this, svg_);

#if 0
  QSplitter *splitter = new QSplitter;
  splitter->setObjectName("splitter");

  splitter->addWidget(canvas_);

  QFrame *rframe = new QFrame;
  rframe->setObjectName("rframe");

  QVBoxLayout *rlayout = new QVBoxLayout(rframe);
  rlayout->setMargin(2); rlayout->setSpacing(2);

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

  //------

  QMenu *fileMenu = menuBar()->addMenu("&File");

  //---

  QAction *printAction = new QAction("&Print", this);

  connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

  fileMenu->addAction(printAction);

  //---

  QAction *printFlatAction = new QAction("Print &Flat", this);

  connect(printFlatAction, SIGNAL(triggered()), this, SLOT(printFlat()));

  fileMenu->addAction(printFlatAction);

  //---

  QAction *exitAction = new QAction("&Exit", this);

  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

  fileMenu->addAction(exitAction);

  //------

  QMenu *viewMenu = menuBar()->addMenu("&View");

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

  QAction *propertiesAction = new QAction("&Properties", this);
  propertiesAction->setIcon(CQPixmapCacheInst->getIcon("PROPERTIES"));

  connect(propertiesAction, SIGNAL(triggered()), this, SLOT(showProperties()));

  viewMenu->addAction(propertiesAction);

  //---

  QAction *buffersAction = new QAction("&Buffers", this);
  buffersAction->setIcon(CQPixmapCacheInst->getIcon("BUFFERS"));

  connect(buffersAction, SIGNAL(triggered()), this, SLOT(showBuffers()));

  viewMenu->addAction(buffersAction);

  //---

  QAction *jsAction = new QAction("&JavaScript", this);
  jsAction->setIcon(CQPixmapCacheInst->getIcon("JAVASCRIPT"));

  connect(jsAction, SIGNAL(triggered()), this, SLOT(showJSDialog()));

  viewMenu->addAction(jsAction);

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

  //---

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

  QToolBar *viewToolBar = new QToolBar(this);

  viewToolBar->addAction(prevAction_);
  viewToolBar->addAction(nextAction_);
  viewToolBar->addAction(propertiesAction);
  viewToolBar->addAction(buffersAction);
  viewToolBar->addAction(jsAction);

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

CQSVGWindow::
~CQSVGWindow()
{
  delete svg_;
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

    setRootId(rootIds_[rootInd_]);
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
        fileInd_ = files_.size() - 1;
    }

    loadFile();
  }
  else if (rootIds_.size() > 1) {
    if (rootInd_ < 0)
      rootInd_ = 0;
    else {
      ++rootInd_;

      if (rootInd_ >= int(rootIds_.size()))
        rootInd_ = rootIds_.size() - 1;
    }

    setRootId(rootIds_[rootInd_]);

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

    setRootId(rootIds_[rootInd_]);

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

  std::string filename = files_[fileInd_].toStdString();

  if      (isImage()) {
    svg_->read(filename);

    auto *renderer = new CQSVGRenderer;

    renderer->setSize(svg_->getIWidth(), svg_->getIHeight());

    svg_->setRenderer(renderer);

    //---

    svg_->draw();

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

    svg_->setRenderer(0);
  }
  else if (isPrint()) {
    svg_->print(std::cout, true);
  }
  else {
    svg_->stopTimer();

    svg_->setTime(0);

    svg_->read(filename);

    updateAltRoot();

    addProperties();

    loadCSS();

    if (svg_->hasAnimation())
      svg_->startTimer();

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

  auto filename = files_[fileInd_];

  QString title = filename;

  if (rootId() != "")
    title += " (" + rootId() + ")";

  setWindowTitle(title);
}

void
CQSVGWindow::
updateAltRoot()
{
  if (rootId() != "") {
    svg_->setAltRoot(0);

    CSVGObject::ObjectArray objects;

    svg_->getChildrenOfId(rootId().toStdString(), objects);

    if      (objects.empty())
      std::cerr << "No objects of id '" << rootId().toStdString() << "'\n";
    else {
      if (objects.size() > 1)
        std::cerr << "Multiple objects match id '" << rootId().toStdString() << "'\n";

      svg_->setAltRoot(objects[0]);
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

  if (svg_->hasAnimation()) {
    animateMenu_   ->menuAction()->setVisible(true);
    animateToolBar_->setVisible(true);

    playAction_ ->setEnabled(! svg_->isAnimating());
    pauseAction_->setEnabled(svg_->isAnimating());
    stepAction_ ->setEnabled(! svg_->isAnimating());
    bstepAction_->setEnabled(! svg_->isAnimating());
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

  propTree_->clear();

  propTree_->addProperty("", svg_, "background");
  propTree_->addProperty("", svg_, "animating");
  propTree_->addProperty("", svg_, "checkerboard");
  propTree_->addProperty("", svg_, "showGradient");
  propTree_->addProperty("", svg_, "showFilterBox");
  propTree_->addProperty("", svg_, "ignoreFilter");

  auto *block = svg_->getRoot();

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
  //QObject *qobj = dynamic_cast<QObject *>(obj);

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
  QString ptext = QString("%1,%2").arg(ppos.x()).arg(ppos.y());
  QString wtext = QString("%1,%2").arg(wpos.x()).arg(wpos.y());

  posLabel_->setText(QString("P(%1) W(%2)").arg(ptext).arg(wtext));

  zoomLabel_->setText(QString("%1").arg(canvas_->scale()));
}

void
CQSVGWindow::
deselectAllObjects()
{
  auto *block = svg_->getRoot();

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
  QString cdir = QDir::currentPath();

  QString file = QFileDialog::getSaveFileName(this, "Specify Output File", cdir, "*.svg");

  std::ofstream os(file.toStdString());

  svg_->print(os, true);
}

void
CQSVGWindow::
printFlat()
{
  QString cdir = QDir::currentPath();

  QString file = QFileDialog::getSaveFileName(this, "Specify Output File", cdir, "*.svg");

  std::ofstream os(file.toStdString());

  svg_->printFlat(os, /*force*/true);
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
  if (! bufferView_)
    bufferView_ = new CQSVGBufferView(svg_);

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
  svg_->startTimer();

  updateState();
}

void
CQSVGWindow::
pauseSlot()
{
  svg_->stopTimer();

  updateState();
}

void
CQSVGWindow::
stepSlot()
{
  svg_->stepTimer();
}

void
CQSVGWindow::
bstepSlot()
{
  svg_->bstepTimer();
}

void
CQSVGWindow::
timeSlot()
{
  bool ok;

  double t = timeEdit_->text().toDouble(&ok);

  if (ok)
    svg_->setTime(t);
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
    return QSize(0.50*w, 0.75*h);
  else
    return QSize(0.50*w, 0.50*h);
}
