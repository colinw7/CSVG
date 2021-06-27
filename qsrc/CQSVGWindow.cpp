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

  std::string filename = files_[fileInd_].toStdString();

  QString title = filename.c_str();

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
    auto *qblock    = dynamic_cast<CQSVGBlock              *>(obj);
    auto *qanchor   = dynamic_cast<CQSVGAnchor             *>(obj);
    auto *qanim     = dynamic_cast<CQSVGAnimate            *>(obj);
    auto *qanimcol  = dynamic_cast<CQSVGAnimateColor       *>(obj);
    auto *qanimmot  = dynamic_cast<CQSVGAnimateMotion      *>(obj);
    auto *qanimtran = dynamic_cast<CQSVGAnimateTransform   *>(obj);
    auto *qcircle   = dynamic_cast<CQSVGCircle             *>(obj);
    auto *qclippath = dynamic_cast<CQSVGClipPath           *>(obj);
    auto *qdefs     = dynamic_cast<CQSVGDefs               *>(obj);
    auto *qdesc     = dynamic_cast<CQSVGDesc               *>(obj);
    auto *qellipse  = dynamic_cast<CQSVGEllipse            *>(obj);
    auto *qfeblend  = dynamic_cast<CQSVGFeBlend            *>(obj);
    auto *qfecmtrx  = dynamic_cast<CQSVGFeColorMatrix      *>(obj);
    auto *qfecomp   = dynamic_cast<CQSVGFeComposite        *>(obj);
    auto *qfediffl  = dynamic_cast<CQSVGFeDiffuseLighting  *>(obj);
    auto *qfedmap   = dynamic_cast<CQSVGFeDisplacementMap  *>(obj);
    auto *qfedlight = dynamic_cast<CQSVGFeDistantLight     *>(obj);
    auto *qfeflood  = dynamic_cast<CQSVGFeFlood            *>(obj);
    auto *qfegauss  = dynamic_cast<CQSVGFeGaussianBlur     *>(obj);
    auto *qfeimage  = dynamic_cast<CQSVGFeImage            *>(obj);
    auto *femerge   = dynamic_cast<CQSVGFeMerge            *>(obj);
    auto *femnode   = dynamic_cast<CQSVGFeMergeNode        *>(obj);
    auto *femorph   = dynamic_cast<CQSVGFeMorphology       *>(obj);
    auto *qfeoff    = dynamic_cast<CQSVGFeOffset           *>(obj);
    auto *qfepl     = dynamic_cast<CQSVGFePointLight       *>(obj);
    auto *qfespec   = dynamic_cast<CQSVGFeSpecularLighting *>(obj);
    auto *qfespot   = dynamic_cast<CQSVGFeSpotLight        *>(obj);
    auto *qfetile   = dynamic_cast<CQSVGFeTile             *>(obj);
    auto *qfeturb   = dynamic_cast<CQSVGFeTurbulence       *>(obj);
    auto *qfilter   = dynamic_cast<CQSVGFilter             *>(obj);
    auto *qgroup    = dynamic_cast<CQSVGGroup              *>(obj);
    auto *qimage    = dynamic_cast<CQSVGImage              *>(obj);
    auto *qline     = dynamic_cast<CQSVGLine               *>(obj);
    auto *qlgrad    = dynamic_cast<CQSVGLinearGradient     *>(obj);
    auto *qmarker   = dynamic_cast<CQSVGMarker             *>(obj);
    auto *qmask     = dynamic_cast<CQSVGMask               *>(obj);
    auto *qpath     = dynamic_cast<CQSVGPath               *>(obj);
    auto *qpattern  = dynamic_cast<CQSVGPattern            *>(obj);
    auto *qpolygon  = dynamic_cast<CQSVGPolygon            *>(obj);
    auto *qpolyline = dynamic_cast<CQSVGPolyLine           *>(obj);
    auto *qrgrad    = dynamic_cast<CQSVGRadialGradient     *>(obj);
    auto *qrect     = dynamic_cast<CQSVGRect               *>(obj);
    auto *qstop     = dynamic_cast<CQSVGStop               *>(obj);
    auto *qtext     = dynamic_cast<CQSVGText               *>(obj);
    auto *qtextpath = dynamic_cast<CQSVGTextPath           *>(obj);
    auto *qtitle    = dynamic_cast<CQSVGTitle              *>(obj);
    auto *qtspan    = dynamic_cast<CQSVGTSpan              *>(obj);
    auto *quse      = dynamic_cast<CQSVGUse                *>(obj);

    //---

    bool hasChildren = qobj->object()->hasChildren();

    //---

    QString objName = name.c_str();

    propTree_->addProperty(objName, qobj, "id");

    if (qobj->object()->isDrawable() || hasChildren)
      propTree_->addProperty(objName, qobj, "visible");

    if (qobj->object()->getFilter())
      propTree_->addProperty(objName, qobj, "filtered");

    if (qobj->object()->getClipPath())
      propTree_->addProperty(objName, qobj, "clipped");

    if (qobj->object()->getMask())
      propTree_->addProperty(objName, qobj, "masked");

    //---

    if      (qblock) {
      propTree_->addProperty(objName, qblock, "x"     );
      propTree_->addProperty(objName, qblock, "y"     );
      propTree_->addProperty(objName, qblock, "width" );
      propTree_->addProperty(objName, qblock, "height");
    }
    else if (qanchor) {
      propTree_->addProperty(objName, qanchor, "xlink");
    }
    else if (qanim) {
      propTree_->addProperty(objName, qanim, "attributeName");
      propTree_->addProperty(objName, qanim, "attributeType");
      propTree_->addProperty(objName, qanim, "begin");
      propTree_->addProperty(objName, qanim, "end");
      propTree_->addProperty(objName, qanim, "dur");
      propTree_->addProperty(objName, qanim, "from");
      propTree_->addProperty(objName, qanim, "to");
      propTree_->addProperty(objName, qanim, "repeatCount");
      propTree_->addProperty(objName, qanim, "repeatDur");
      propTree_->addProperty(objName, qanim, "fill");
    }
    else if (qanimcol) {
      propTree_->addProperty(objName, qanimcol, "attributeName");
      propTree_->addProperty(objName, qanimcol, "attributeType");
      propTree_->addProperty(objName, qanimcol, "begin");
      propTree_->addProperty(objName, qanimcol, "end");
      propTree_->addProperty(objName, qanimcol, "dur");
      propTree_->addProperty(objName, qanimcol, "from");
      propTree_->addProperty(objName, qanimcol, "to");
      propTree_->addProperty(objName, qanimcol, "repeatCount");
      propTree_->addProperty(objName, qanimcol, "repeatDur");
      propTree_->addProperty(objName, qanimcol, "fill");
      propTree_->addProperty(objName, qanimcol, "type");
      propTree_->addProperty(objName, qanimcol, "additive");
    }
    else if (qanimmot) {
      propTree_->addProperty(objName, qanimmot, "attributeName");
      propTree_->addProperty(objName, qanimmot, "attributeType");
      propTree_->addProperty(objName, qanimmot, "begin");
      propTree_->addProperty(objName, qanimmot, "end");
      propTree_->addProperty(objName, qanimmot, "dur");
      propTree_->addProperty(objName, qanimmot, "from");
      propTree_->addProperty(objName, qanimmot, "to");
      propTree_->addProperty(objName, qanimmot, "repeatCount");
      propTree_->addProperty(objName, qanimmot, "repeatDur");
      propTree_->addProperty(objName, qanimmot, "fill");
      propTree_->addProperty(objName, qanimmot, "pathStr");
      propTree_->addProperty(objName, qanimmot, "keyPoints");
      propTree_->addProperty(objName, qanimmot, "rotate");
      propTree_->addProperty(objName, qanimmot, "origin");
    }
    else if (qanimtran) {
      propTree_->addProperty(objName, qanimtran, "attributeName");
      propTree_->addProperty(objName, qanimtran, "attributeType");
      propTree_->addProperty(objName, qanimtran, "begin");
      propTree_->addProperty(objName, qanimtran, "end");
      propTree_->addProperty(objName, qanimtran, "dur");
      propTree_->addProperty(objName, qanimtran, "from");
      propTree_->addProperty(objName, qanimtran, "to");
      propTree_->addProperty(objName, qanimtran, "repeatCount");
      propTree_->addProperty(objName, qanimtran, "repeatDur");
      propTree_->addProperty(objName, qanimtran, "fill");
    }
    else if (qdefs) {
    }
    else if (qdesc) {
      propTree_->addProperty(objName, qdesc, "text");
    }
    else if (qcircle) {
      propTree_->addProperty(objName, qcircle, "cx");
      propTree_->addProperty(objName, qcircle, "cy");
      propTree_->addProperty(objName, qcircle, "r" );
    }
    else if (qclippath) {
    }
    else if (qellipse) {
      propTree_->addProperty(objName, qellipse, "cx");
      propTree_->addProperty(objName, qellipse, "cy");
      propTree_->addProperty(objName, qellipse, "rx");
      propTree_->addProperty(objName, qellipse, "ry");
    }
    else if (qfeblend) {
    }
    else if (qfecmtrx) {
      propTree_->addProperty(objName, qfecmtrx, "type");
    }
    else if (qfecomp) {
      propTree_->addProperty(objName, qfecomp, "k1");
      propTree_->addProperty(objName, qfecomp, "k2");
      propTree_->addProperty(objName, qfecomp, "k3");
      propTree_->addProperty(objName, qfecomp, "k4");
    }
    else if (qfediffl) {
      propTree_->addProperty(objName, qfediffl, "lightingColor");
      propTree_->addProperty(objName, qfediffl, "surfaceScale");
      propTree_->addProperty(objName, qfediffl, "diffuseConstant");
    }
    else if (qfedmap) {
      propTree_->addProperty(objName, qfedmap, "scale");
    }
    else if (qfedlight) {
      propTree_->addProperty(objName, qfedlight, "elevation");
      propTree_->addProperty(objName, qfedlight, "azimuth"  );
    }
    else if (qfeflood) {
      propTree_->addProperty(objName, qfeflood, "opacity");
    }
    else if (qfegauss) {
      propTree_->addProperty(objName, qfegauss, "stdDevX");
      propTree_->addProperty(objName, qfegauss, "stdDevY");
    }
    else if (qfeimage) {
      propTree_->addProperty(objName, qfeimage, "width" );
      propTree_->addProperty(objName, qfeimage, "height");
    }
    else if (femerge) {
    }
    else if (femnode) {
    }
    else if (femorph) {
    }
    else if (qfilter) {
      propTree_->addProperty(objName, qfilter, "x");
      propTree_->addProperty(objName, qfilter, "y");
      propTree_->addProperty(objName, qfilter, "width");
      propTree_->addProperty(objName, qfilter, "height");
    }
    else if (qfeoff) {
      propTree_->addProperty(objName, qfeoff, "dx");
      propTree_->addProperty(objName, qfeoff, "dy");
    }
    else if (qfepl) {
      propTree_->addProperty(objName, qfepl, "x");
      propTree_->addProperty(objName, qfepl, "y");
      propTree_->addProperty(objName, qfepl, "z");
    }
    else if (qfespec) {
      propTree_->addProperty(objName, qfespec, "lightingColor");
      propTree_->addProperty(objName, qfespec, "specularConstant");
      propTree_->addProperty(objName, qfespec, "specularExponent");
      propTree_->addProperty(objName, qfespec, "surfaceScale");
    }
    else if (qfespot) {
      propTree_->addProperty(objName, qfespot, "x");
      propTree_->addProperty(objName, qfespot, "y");
      propTree_->addProperty(objName, qfespot, "z");
      propTree_->addProperty(objName, qfespot, "pointsAtX");
      propTree_->addProperty(objName, qfespot, "pointsAtY");
      propTree_->addProperty(objName, qfespot, "pointsAtZ");
      propTree_->addProperty(objName, qfespot, "specularExponent");
      propTree_->addProperty(objName, qfespot, "limitingConeAngle");
    }
    else if (qfetile) {
    }
    else if (qfeturb) {
      propTree_->addProperty(objName, qfeturb, "fractalNoise");
      propTree_->addProperty(objName, qfeturb, "baseFreq");
      propTree_->addProperty(objName, qfeturb, "numOctaves");
      propTree_->addProperty(objName, qfeturb, "seed");
    }
    else if (qgroup) {
    }
    else if (qimage) {
      propTree_->addProperty(objName, qimage, "x"     );
      propTree_->addProperty(objName, qimage, "y"     );
      propTree_->addProperty(objName, qimage, "width" );
      propTree_->addProperty(objName, qimage, "height");
    }
    else if (qline) {
      propTree_->addProperty(objName, qline, "x1");
      propTree_->addProperty(objName, qline, "y1");
      propTree_->addProperty(objName, qline, "x2");
      propTree_->addProperty(objName, qline, "y2");
    }
    else if (qlgrad) {
      propTree_->addProperty(objName, qlgrad, "x1");
      propTree_->addProperty(objName, qlgrad, "y1");
      propTree_->addProperty(objName, qlgrad, "x2");
      propTree_->addProperty(objName, qlgrad, "y2");
    }
    else if (qmarker) {
      propTree_->addProperty(objName, qmarker, "refX");
      propTree_->addProperty(objName, qmarker, "refY");
    }
    else if (qmask) {
      propTree_->addProperty(objName, qmask, "x"     );
      propTree_->addProperty(objName, qmask, "y"     );
      propTree_->addProperty(objName, qmask, "width" );
      propTree_->addProperty(objName, qmask, "height");
    }
    else if (qpath) {
      propTree_->addProperty(objName, qpath, "pathString");

      QString partsName = objName + "/parts";

      int ind = 0;

      for (const auto &part : qpath->getPartList().parts()) {
        auto *qpathpart = dynamic_cast<CQSVGPathPart *>(part);

        auto *qmoveto   = dynamic_cast<CQSVGPathMoveTo      *>(qpathpart);
        auto *qrmoveto  = dynamic_cast<CQSVGPathRMoveTo     *>(qpathpart);
        auto *qlineto   = dynamic_cast<CQSVGPathLineTo      *>(qpathpart);
        auto *qrlineto  = dynamic_cast<CQSVGPathRLineTo     *>(qpathpart);
        auto *qhlineto  = dynamic_cast<CQSVGPathHLineTo     *>(qpathpart);
        auto *qrhlineto = dynamic_cast<CQSVGPathRHLineTo    *>(qpathpart);
        auto *qvlineto  = dynamic_cast<CQSVGPathVLineTo     *>(qpathpart);
        auto *qrvlineto = dynamic_cast<CQSVGPathRVLineTo    *>(qpathpart);
        auto *qarcto    = dynamic_cast<CQSVGPathArcTo       *>(qpathpart);
        auto *qrarcto   = dynamic_cast<CQSVGPathRArcTo      *>(qpathpart);
        auto *qbez2to   = dynamic_cast<CQSVGPathBezier2To   *>(qpathpart);
        auto *qmbez2to  = dynamic_cast<CQSVGPathMBezier2To  *>(qpathpart);
        auto *qrbez2to  = dynamic_cast<CQSVGPathRBezier2To  *>(qpathpart);
        auto *qmrbez2to = dynamic_cast<CQSVGPathMRBezier2To *>(qpathpart);
        auto *qbez3to   = dynamic_cast<CQSVGPathBezier3To   *>(qpathpart);
        auto *qmbez3to  = dynamic_cast<CQSVGPathMBezier3To  *>(qpathpart);
        auto *qrbez3to  = dynamic_cast<CQSVGPathRBezier3To  *>(qpathpart);
        auto *qmrbez3to = dynamic_cast<CQSVGPathMRBezier3To *>(qpathpart);

        QString typeName = CSVGPathPart::partTypeName(qpathpart->getType()).c_str();
        QString partName = partsName + "/" + typeName + ":" + QString("%1").arg(ind + 1);

        if      (qmoveto) {
          propTree_->addProperty(partName, qmoveto, "x");
          propTree_->addProperty(partName, qmoveto, "y");
        }
        else if (qrmoveto) {
          propTree_->addProperty(partName, qrmoveto, "dx");
          propTree_->addProperty(partName, qrmoveto, "dy");
        }
        else if (qlineto) {
          propTree_->addProperty(partName, qlineto, "x");
          propTree_->addProperty(partName, qlineto, "y");
        }
        else if (qrlineto) {
          propTree_->addProperty(partName, qrlineto, "dx");
          propTree_->addProperty(partName, qrlineto, "dy");
        }
        else if (qhlineto) {
          propTree_->addProperty(partName, qhlineto, "x");
        }
        else if (qrhlineto) {
          propTree_->addProperty(partName, qrhlineto, "dx");
        }
        else if (qvlineto) {
          propTree_->addProperty(partName, qvlineto, "y");
        }
        else if (qrvlineto) {
          propTree_->addProperty(partName, qrvlineto, "dy");
        }
        else if (qarcto) {
          propTree_->addProperty(partName, qarcto, "rx");
          propTree_->addProperty(partName, qarcto, "ry");
          propTree_->addProperty(partName, qarcto, "xa");
          propTree_->addProperty(partName, qarcto, "fa");
          propTree_->addProperty(partName, qarcto, "fs");
          propTree_->addProperty(partName, qarcto, "x2");
          propTree_->addProperty(partName, qarcto, "y2");
        }
        else if (qrarcto) {
          propTree_->addProperty(partName, qrarcto, "rx");
          propTree_->addProperty(partName, qrarcto, "ry");
          propTree_->addProperty(partName, qrarcto, "xa");
          propTree_->addProperty(partName, qrarcto, "fa");
          propTree_->addProperty(partName, qrarcto, "fs");
          propTree_->addProperty(partName, qrarcto, "x2");
          propTree_->addProperty(partName, qrarcto, "y2");
        }
        else if (qbez2to) {
          propTree_->addProperty(partName, qbez2to, "x1");
          propTree_->addProperty(partName, qbez2to, "y1");
          propTree_->addProperty(partName, qbez2to, "x2");
          propTree_->addProperty(partName, qbez2to, "y2");
        }
        else if (qmbez2to) {
          propTree_->addProperty(partName, qmbez2to, "x2");
          propTree_->addProperty(partName, qmbez2to, "y2");
        }
        else if (qrbez2to) {
          propTree_->addProperty(partName, qrbez2to, "x1");
          propTree_->addProperty(partName, qrbez2to, "y1");
          propTree_->addProperty(partName, qrbez2to, "x2");
          propTree_->addProperty(partName, qrbez2to, "y2");
        }
        else if (qmrbez2to) {
          propTree_->addProperty(partName, qmrbez2to, "x2");
          propTree_->addProperty(partName, qmrbez2to, "y2");
        }
        else if (qbez3to) {
          propTree_->addProperty(partName, qbez3to, "x1");
          propTree_->addProperty(partName, qbez3to, "y1");
          propTree_->addProperty(partName, qbez3to, "x2");
          propTree_->addProperty(partName, qbez3to, "y2");
          propTree_->addProperty(partName, qbez3to, "x3");
          propTree_->addProperty(partName, qbez3to, "y3");
        }
        else if (qmbez3to) {
          propTree_->addProperty(partName, qmbez3to, "x2");
          propTree_->addProperty(partName, qmbez3to, "y2");
          propTree_->addProperty(partName, qmbez3to, "x3");
          propTree_->addProperty(partName, qmbez3to, "y3");
        }
        else if (qrbez3to) {
          propTree_->addProperty(partName, qrbez3to, "x1");
          propTree_->addProperty(partName, qrbez3to, "y1");
          propTree_->addProperty(partName, qrbez3to, "x2");
          propTree_->addProperty(partName, qrbez3to, "y2");
          propTree_->addProperty(partName, qrbez3to, "x3");
          propTree_->addProperty(partName, qrbez3to, "y3");
        }
        else if (qmrbez3to) {
          propTree_->addProperty(partName, qmrbez3to, "x2");
          propTree_->addProperty(partName, qmrbez3to, "y2");
          propTree_->addProperty(partName, qmrbez3to, "x3");
          propTree_->addProperty(partName, qmrbez3to, "y3");
        }
        else {
          propTree_->addProperty(partName, qpathpart, "type");
        }

        ++ind;
      }
    }
    else if (qpattern) {
      propTree_->addProperty(objName, qpattern, "x"           );
      propTree_->addProperty(objName, qpattern, "y"           );
      propTree_->addProperty(objName, qpattern, "width"       );
      propTree_->addProperty(objName, qpattern, "height"      );
      propTree_->addProperty(objName, qpattern, "units"       );
      propTree_->addProperty(objName, qpattern, "contentUnits");
    }
    else if (qpolygon) {
    }
    else if (qpolyline) {
    }
    else if (qrgrad) {
      propTree_->addProperty(objName, qrgrad, "cx");
      propTree_->addProperty(objName, qrgrad, "cy");
      propTree_->addProperty(objName, qrgrad, "r" );
      propTree_->addProperty(objName, qrgrad, "fx");
      propTree_->addProperty(objName, qrgrad, "fy");
    }
    else if (qrect) {
      propTree_->addProperty(objName, qrect, "x" );
      propTree_->addProperty(objName, qrect, "y" );
      propTree_->addProperty(objName, qrect, "w" );
      propTree_->addProperty(objName, qrect, "h" );
      propTree_->addProperty(objName, qrect, "rx");
      propTree_->addProperty(objName, qrect, "ry");
    }
    else if (qstop) {
      propTree_->addProperty(objName, qstop, "offset" );
      propTree_->addProperty(objName, qstop, "opacity");
    }
    else if (qtext) {
      propTree_->addProperty(objName, qtext, "x"           );
      propTree_->addProperty(objName, qtext, "y"           );
      propTree_->addProperty(objName, qtext, "dx"          );
      propTree_->addProperty(objName, qtext, "dy"          );
      propTree_->addProperty(objName, qtext, "text"        );
      propTree_->addProperty(objName, qtext, "rotate"      );
      propTree_->addProperty(objName, qtext, "textLength"  );
      propTree_->addProperty(objName, qtext, "lengthAdjust");
    }
    else if (qtextpath) {
      propTree_->addProperty(objName, qtextpath, "text"       );
      propTree_->addProperty(objName, qtextpath, "xlink"      );
      propTree_->addProperty(objName, qtextpath, "startOffset");
    }
    else if (qtitle) {
      propTree_->addProperty(objName, qtitle, "text");

      setWindowTitle(qtitle->getQText());
    }
    else if (qtspan) {
      propTree_->addProperty(objName, qtspan, "x"   );
      propTree_->addProperty(objName, qtspan, "y"   );
      propTree_->addProperty(objName, qtspan, "text");
    }
    else if (quse) {
      propTree_->addProperty(objName, quse, "x"     );
      propTree_->addProperty(objName, quse, "y"     );
      propTree_->addProperty(objName, quse, "width" );
      propTree_->addProperty(objName, quse, "height");
      propTree_->addProperty(objName, quse, "xlink" );
    }

    //---

    if (! qobj->object()->isAnimated()) {
      if (qobj->object()->isDrawable() || hasChildren)
        propTree_->addProperty(objName, qobj, "transform");

      if (qobj->object()->isDrawable() || hasChildren) {
        propTree_->addProperty(objName, qobj, "opacity");

        //---

        QString strokeName = objName + "/stroke";

        propTree_->addProperty(strokeName, qobj, "strokeNoColor"     )->setLabel("noColor"     );
        propTree_->addProperty(strokeName, qobj, "strokeCurrentColor")->setLabel("currentColor");
        propTree_->addProperty(strokeName, qobj, "strokeColor"       )->setLabel("color"       );
        propTree_->addProperty(strokeName, qobj, "strokeOpacity"     )->setLabel("opacity"     );
        propTree_->addProperty(strokeName, qobj, "strokeWidth"       )->setLabel("width"       );
        propTree_->addProperty(strokeName, qobj, "strokeDash"        )->setLabel("dash"        );
        propTree_->addProperty(strokeName, qobj, "strokeOffset"      )->setLabel("offset"      );
        propTree_->addProperty(strokeName, qobj, "strokeCap"         )->setLabel("cap"         );
        propTree_->addProperty(strokeName, qobj, "strokeJoin"        )->setLabel("join"        );

        //---

        QString fillName = objName + "/fill";

        propTree_->addProperty(fillName, qobj, "fillNoColor"     )->setLabel("noColor"     );
        propTree_->addProperty(fillName, qobj, "fillCurrentColor")->setLabel("currentColor");
        propTree_->addProperty(fillName, qobj, "fillColor"       )->setLabel("color"       );
        propTree_->addProperty(fillName, qobj, "fillOpacity"     )->setLabel("opacity"     );
        propTree_->addProperty(fillName, qobj, "fillRule"        )->setLabel("rule"        );
        propTree_->addProperty(fillName, qobj, "fillUrl"         )->setLabel("url"         );
      }

      if (qobj->object()->hasFont()) {
        QString fontName = objName + "/font";

        propTree_->addProperty(fontName, qobj, "fontFamily")->setLabel("family");
      //propTree_->addProperty(fontName, qobj, "fontStyle" )->setLabel("style");
        propTree_->addProperty(fontName, qobj, "fontSize"  )->setLabel("size");
        propTree_->addProperty(fontName, qobj, "font"      )->setLabel("font");
      }
    }
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
