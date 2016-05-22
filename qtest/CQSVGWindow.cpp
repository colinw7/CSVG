#include <CQSVGWindow.h>
#include <CQSVG.h>
#include <CQSVGCanvas.h>
#include <CQSVGPropertiesDlg.h>
#include <CQSVGBufferView.h>

#include <CQSVGAnchor.h>
#include <CQSVGAnimateColor.h>
#include <CQSVGAnimate.h>
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

#include <CQSVGRenderer.h>
#include <CQPropertyTree.h>
#include <CQPropertyItem.h>
#include <CQPixmapCache.h>

#include <CQApp.h>
#include <CQStyle.h>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>

#include <svg/next_svg.h>
#include <svg/prev_svg.h>
#include <svg/properties_svg.h>
#include <svg/buffers_svg.h>
#include <svg/play_svg.h>
#include <svg/pause_svg.h>
#include <svg/play_one_svg.h>
#include <svg/rewind_one_svg.h>

int
main(int argc, char **argv)
{
  bool        debug    = false;
  bool        image    = false;
  std::string imageDir = "";
  bool        print    = false;

  std::vector<std::string> files;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if      (strcmp(&argv[i][1], "debug") == 0)
        debug = true;
      else if (strcmp(&argv[i][1], "image") == 0)
        image = true;
      else if (strcmp(&argv[i][1], "image_dir") == 0) {
        ++i;

        if (i < argc)
          imageDir = argv[i];
      }
      else if (strcmp(&argv[i][1], "print") == 0)
        print = true;
      else
        std::cerr << "Invalid option: " << argv[i] << std::endl;
    }
    else
      files.push_back(argv[i]);
  }

  //---

  CQApp app(argc, argv);

  app.setStyle(new CQStyle);

  CQSVGWindow *window = new CQSVGWindow;

  if (debug)
    window->svg()->setDebug(true);

  if (image)
    window->setImage(image);

  if (imageDir != "")
    window->setImageDir(imageDir);

  if (print)
    window->setPrint(print);

  for (const auto &file : files)
    window->addFile(file);

  window->loadFile();

  if (image || print)
    exit(0);

  window->show();

  app.exec();

  return 0;
}

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

  tree_ = new CQPropertyTree;

  connect(tree_, SIGNAL(valueChanged(QObject *, const QString &)),
          this, SLOT(redraw()));

  connect(tree_, SIGNAL(itemSelected(QObject *, const QString &)),
          this, SLOT(itemSelectedSlot(QObject *, const QString &)));

  //connect(tree_, SIGNAL(menuExec(QObject *, const QPoint &)),
  //        this, SLOT(treeMenuExec(QObject *, const QPoint &)));

  rframe->setMaximumWidth(400);

  rlayout->addWidget(tree_);

  splitter->addWidget(rframe);

  layout->addWidget(splitter);
#else
  layout->addWidget(canvas_);
#endif

  setCentralWidget(frame);

  //---

  QMenu *fileMenu = menuBar()->addMenu("&File");

  QAction *printAction = new QAction("&Print", this);

  connect(printAction, SIGNAL(triggered()), this, SLOT(print()));

  fileMenu->addAction(printAction);

  QAction *exitAction = new QAction("&Exit", this);

  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

  fileMenu->addAction(exitAction);

  //---

  QMenu *viewMenu = menuBar()->addMenu("&View");

  prevAction_ = new QAction("&Prev", this);
  prevAction_->setIcon(CQPixmapCacheInst->getIcon("PREV"));

  connect(prevAction_, SIGNAL(triggered()), this, SLOT(prevFile()));

  viewMenu->addAction(prevAction_);

  nextAction_ = new QAction("&Next", this);
  nextAction_->setIcon(CQPixmapCacheInst->getIcon("NEXT"));

  connect(nextAction_, SIGNAL(triggered()), this, SLOT(nextFile()));

  viewMenu->addAction(nextAction_);

  QAction *propertiesAction = new QAction("&Properties", this);
  propertiesAction->setIcon(CQPixmapCacheInst->getIcon("PROPERTIES"));

  connect(propertiesAction, SIGNAL(triggered()), this, SLOT(showProperties()));

  viewMenu->addAction(propertiesAction);

  QAction *buffersAction = new QAction("&Buffers", this);
  buffersAction->setIcon(CQPixmapCacheInst->getIcon("BUFFERS"));

  connect(buffersAction, SIGNAL(triggered()), this, SLOT(showBuffers()));

  viewMenu->addAction(buffersAction);

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

  showPos(QPoint(0, 0), QPointF(0, 0));

  //---

  QToolBar *viewToolBar = new QToolBar(this);

  viewToolBar->addAction(prevAction_);
  viewToolBar->addAction(nextAction_);
  viewToolBar->addAction(propertiesAction);
  viewToolBar->addAction(buffersAction);

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
addFile(const std::string &file)
{
  if (! CFile::isRegular(file)) {
    std::cerr << "Invalid file \"" << file << "\"" << std::endl;
    return;
  }

  files_.push_back(file);
}

void
CQSVGWindow::
nextFile()
{
  if (ind_ < 0)
    ind_ = 0;
  else {
    ++ind_;

    if (ind_ >= int(files_.size()))
      ind_ = files_.size() - 1;
  }

  loadFile();
}

void
CQSVGWindow::
prevFile()
{
  if (ind_ < 0)
    ind_ = 0;
  else {
    --ind_;

    if (ind_ < 0)
      ind_ = 0;
  }

  loadFile();
}

void
CQSVGWindow::
loadFile()
{
  if (ind_ < 0)
    ind_ = 0;

  updateState();

  if (ind_ < 0 || ind_ >= int(files_.size()))
    return;

  std::string filename = files_[ind_];

  setWindowTitle(filename.c_str());

  if      (isImage()) {
    CImageNoSrc src;

    CImagePtr image = CImageMgrInst->createImage(src);

    svg_->read(filename);

    CQSVGRenderer renderer;

    renderer.setSize(svg_->getIWidth(), svg_->getIHeight());

    svg_->setRenderer(&renderer);

    svg_->draw();

    CFile file(filename);

    std::string base = file.getBase();

    std::string name;

    if (imageDir() != "")
      name = CStrUtil::strprintf("%s/svg_%s.png", imageDir().c_str(), base.c_str());
    else
      name = CStrUtil::strprintf("svg_%s.png", base.c_str());

    renderer.getImage()->write(name, CFILE_TYPE_IMAGE_PNG);
  }
  else if (isPrint()) {
    svg_->print(std::cout, true);
  }
  else {
    svg_->stopTimer();

    svg_->read(filename);

    addProperties();

    if (svg_->hasAnimation())
      svg_->startTimer();

    updateState();
  }

  redraw();
}

void
CQSVGWindow::
updateState()
{
  prevAction_->setEnabled(ind_ > 0);
  nextAction_->setEnabled(ind_ < int(files_.size()) - 1);

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
addProperties()
{
  if (propertiesDlg_)
    tree_ = propertiesDlg_->tree();

  if (! tree_)
    return;

  tree_->clear();

  tree_->addProperty("", svg_, "background");
  tree_->addProperty("", svg_, "checkerboard");

  CSVGBlock *block = svg_->getBlock();

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

  CQSVGObject *qobj = dynamic_cast<CQSVGObject *>(obj);

  if (qobj) {
    CQSVGBlock              *qblock    = dynamic_cast<CQSVGBlock              *>(obj);
    CQSVGAnchor             *qanchor   = dynamic_cast<CQSVGAnchor             *>(obj);
    CQSVGAnimate            *qanim     = dynamic_cast<CQSVGAnimate            *>(obj);
    CQSVGAnimateColor       *qanimcol  = dynamic_cast<CQSVGAnimateColor       *>(obj);
    CQSVGAnimateMotion      *qanimmot  = dynamic_cast<CQSVGAnimateMotion      *>(obj);
    CQSVGAnimateTransform   *qanimtran = dynamic_cast<CQSVGAnimateTransform   *>(obj);
    CQSVGCircle             *qcircle   = dynamic_cast<CQSVGCircle             *>(obj);
    CQSVGClipPath           *qclippath = dynamic_cast<CQSVGClipPath           *>(obj);
    CQSVGDefs               *qdefs     = dynamic_cast<CQSVGDefs               *>(obj);
    CQSVGDesc               *qdesc     = dynamic_cast<CQSVGDesc               *>(obj);
    CQSVGEllipse            *qellipse  = dynamic_cast<CQSVGEllipse            *>(obj);
    CQSVGFeBlend            *qfeblend  = dynamic_cast<CQSVGFeBlend            *>(obj);
    CQSVGFeColorMatrix      *qfecmtrx  = dynamic_cast<CQSVGFeColorMatrix      *>(obj);
    CQSVGFeComposite        *qfecomp   = dynamic_cast<CQSVGFeComposite        *>(obj);
    CQSVGFeDiffuseLighting  *qfediffl  = dynamic_cast<CQSVGFeDiffuseLighting  *>(obj);
    CQSVGFeDisplacementMap  *qfedmap   = dynamic_cast<CQSVGFeDisplacementMap  *>(obj);
    CQSVGFeDistantLight     *qfedlight = dynamic_cast<CQSVGFeDistantLight     *>(obj);
    CQSVGFeFlood            *qfeflood  = dynamic_cast<CQSVGFeFlood            *>(obj);
    CQSVGFeGaussianBlur     *qfegauss  = dynamic_cast<CQSVGFeGaussianBlur     *>(obj);
    CQSVGFeImage            *qfeimage  = dynamic_cast<CQSVGFeImage            *>(obj);
    CQSVGFeMerge            *femerge   = dynamic_cast<CQSVGFeMerge            *>(obj);
    CQSVGFeMergeNode        *femnode   = dynamic_cast<CQSVGFeMergeNode        *>(obj);
    CQSVGFeMorphology       *femorph   = dynamic_cast<CQSVGFeMorphology       *>(obj);
    CQSVGFeOffset           *qfeoff    = dynamic_cast<CQSVGFeOffset           *>(obj);
    CQSVGFePointLight       *qfepl     = dynamic_cast<CQSVGFePointLight       *>(obj);
    CQSVGFeSpecularLighting *qfespec   = dynamic_cast<CQSVGFeSpecularLighting *>(obj);
    CQSVGFeSpotLight        *qfespot   = dynamic_cast<CQSVGFeSpotLight        *>(obj);
    CQSVGFeTile             *qfetile   = dynamic_cast<CQSVGFeTile             *>(obj);
    CQSVGFeTurbulence       *qfeturb   = dynamic_cast<CQSVGFeTurbulence       *>(obj);
    CQSVGFilter             *qfilter   = dynamic_cast<CQSVGFilter             *>(obj);
    CQSVGGroup              *qgroup    = dynamic_cast<CQSVGGroup              *>(obj);
    CQSVGImage              *qimage    = dynamic_cast<CQSVGImage              *>(obj);
    CQSVGLine               *qline     = dynamic_cast<CQSVGLine               *>(obj);
    CQSVGLinearGradient     *qlgrad    = dynamic_cast<CQSVGLinearGradient     *>(obj);
    CQSVGMarker             *qmarker   = dynamic_cast<CQSVGMarker             *>(obj);
    CQSVGMask               *qmask     = dynamic_cast<CQSVGMask               *>(obj);
    CQSVGPath               *qpath     = dynamic_cast<CQSVGPath               *>(obj);
    CQSVGPattern            *qpattern  = dynamic_cast<CQSVGPattern            *>(obj);
    CQSVGPolygon            *qpolygon  = dynamic_cast<CQSVGPolygon            *>(obj);
    CQSVGPolyLine           *qpolyline = dynamic_cast<CQSVGPolyLine           *>(obj);
    CQSVGRadialGradient     *qrgrad    = dynamic_cast<CQSVGRadialGradient     *>(obj);
    CQSVGRect               *qrect     = dynamic_cast<CQSVGRect               *>(obj);
    CQSVGStop               *qstop     = dynamic_cast<CQSVGStop               *>(obj);
    CQSVGText               *qtext     = dynamic_cast<CQSVGText               *>(obj);
    CQSVGTextPath           *qtextpath = dynamic_cast<CQSVGTextPath           *>(obj);
    CQSVGTitle              *qtitle    = dynamic_cast<CQSVGTitle              *>(obj);
    CQSVGTSpan              *qtspan    = dynamic_cast<CQSVGTSpan              *>(obj);
    CQSVGUse                *quse      = dynamic_cast<CQSVGUse                *>(obj);

    //---

    bool hasChildren = qobj->object()->hasChildren();

    //---

    QString objName = name.c_str();

    tree_->addProperty(objName, qobj, "id");

    if (qobj->object()->isDrawable() || hasChildren)
      tree_->addProperty(objName, qobj, "visible");

    if (qobj->object()->getFilter())
      tree_->addProperty(objName, qobj, "filtered");

    if (qobj->object()->getClipPath())
      tree_->addProperty(objName, qobj, "clipped");

    if (qobj->object()->getMask())
      tree_->addProperty(objName, qobj, "masked");

    //---

    if      (qblock) {
      tree_->addProperty(objName, qblock, "x"     );
      tree_->addProperty(objName, qblock, "y"     );
      tree_->addProperty(objName, qblock, "width" );
      tree_->addProperty(objName, qblock, "height");
    }
    else if (qanchor) {
      tree_->addProperty(objName, qanchor, "xlink");
    }
    else if (qanim) {
      tree_->addProperty(objName, qanim, "attributeName");
      tree_->addProperty(objName, qanim, "attributeType");
      tree_->addProperty(objName, qanim, "from");
      tree_->addProperty(objName, qanim, "to");
      tree_->addProperty(objName, qanim, "begin");
      tree_->addProperty(objName, qanim, "end");
      tree_->addProperty(objName, qanim, "dur");
      tree_->addProperty(objName, qanim, "fill");
    }
    else if (qanimcol) {
    }
    else if (qanimmot) {
    }
    else if (qanimtran) {
    }
    else if (qdefs) {
    }
    else if (qdesc) {
      tree_->addProperty(objName, qdesc, "text");
    }
    else if (qcircle) {
      tree_->addProperty(objName, qcircle, "cx");
      tree_->addProperty(objName, qcircle, "cy");
      tree_->addProperty(objName, qcircle, "r" );
    }
    else if (qclippath) {
    }
    else if (qellipse) {
      tree_->addProperty(objName, qellipse, "cx");
      tree_->addProperty(objName, qellipse, "cy");
      tree_->addProperty(objName, qellipse, "rx");
      tree_->addProperty(objName, qellipse, "ry");
    }
    else if (qfeblend) {
    }
    else if (qfecmtrx) {
      tree_->addProperty(objName, qfecmtrx, "type");
    }
    else if (qfecomp) {
      tree_->addProperty(objName, qfecomp, "k1");
      tree_->addProperty(objName, qfecomp, "k2");
      tree_->addProperty(objName, qfecomp, "k3");
      tree_->addProperty(objName, qfecomp, "k4");
    }
    else if (qfediffl) {
      tree_->addProperty(objName, qfediffl, "lightingColor");
      tree_->addProperty(objName, qfediffl, "surfaceScale");
      tree_->addProperty(objName, qfediffl, "diffuseConstant");
    }
    else if (qfedmap) {
      tree_->addProperty(objName, qfedmap, "scale");
    }
    else if (qfedlight) {
      tree_->addProperty(objName, qfedlight, "elevation");
      tree_->addProperty(objName, qfedlight, "azimuth"  );
    }
    else if (qfeflood) {
      tree_->addProperty(objName, qfeflood, "opacity");
    }
    else if (qfegauss) {
      tree_->addProperty(objName, qfegauss, "stdDev");
    }
    else if (qfeimage) {
      tree_->addProperty(objName, qfeimage, "width" );
      tree_->addProperty(objName, qfeimage, "height");
    }
    else if (femerge) {
    }
    else if (femnode) {
    }
    else if (femorph) {
    }
    else if (qfilter) {
      tree_->addProperty(objName, qfilter, "x");
      tree_->addProperty(objName, qfilter, "y");
      tree_->addProperty(objName, qfilter, "width");
      tree_->addProperty(objName, qfilter, "height");
    }
    else if (qfeoff) {
      tree_->addProperty(objName, qfeoff, "dx");
      tree_->addProperty(objName, qfeoff, "dy");
    }
    else if (qfepl) {
      tree_->addProperty(objName, qfepl, "x");
      tree_->addProperty(objName, qfepl, "y");
      tree_->addProperty(objName, qfepl, "z");
    }
    else if (qfespec) {
      tree_->addProperty(objName, qfespec, "lightingColor");
      tree_->addProperty(objName, qfespec, "specularConstant");
      tree_->addProperty(objName, qfespec, "specularExponent");
      tree_->addProperty(objName, qfespec, "surfaceScale");
    }
    else if (qfespot) {
      tree_->addProperty(objName, qfespot, "x");
      tree_->addProperty(objName, qfespot, "y");
      tree_->addProperty(objName, qfespot, "z");
      tree_->addProperty(objName, qfespot, "pointsAtX");
      tree_->addProperty(objName, qfespot, "pointsAtY");
      tree_->addProperty(objName, qfespot, "pointsAtZ");
      tree_->addProperty(objName, qfespot, "specularExponent");
      tree_->addProperty(objName, qfespot, "limitingConeAngle");
    }
    else if (qfetile) {
    }
    else if (qfeturb) {
      tree_->addProperty(objName, qfeturb, "fractalNoise");
      tree_->addProperty(objName, qfeturb, "baseFreq");
      tree_->addProperty(objName, qfeturb, "numOctaves");
      tree_->addProperty(objName, qfeturb, "seed");
    }
    else if (qgroup) {
    }
    else if (qimage) {
      tree_->addProperty(objName, qimage, "x"     );
      tree_->addProperty(objName, qimage, "y"     );
      tree_->addProperty(objName, qimage, "width" );
      tree_->addProperty(objName, qimage, "height");
    }
    else if (qline) {
      tree_->addProperty(objName, qline, "x1");
      tree_->addProperty(objName, qline, "y1");
      tree_->addProperty(objName, qline, "x2");
      tree_->addProperty(objName, qline, "y2");
    }
    else if (qlgrad) {
      tree_->addProperty(objName, qlgrad, "x1");
      tree_->addProperty(objName, qlgrad, "y1");
      tree_->addProperty(objName, qlgrad, "x2");
      tree_->addProperty(objName, qlgrad, "y2");
    }
    else if (qmarker) {
      tree_->addProperty(objName, qmarker, "refX");
      tree_->addProperty(objName, qmarker, "refY");
    }
    else if (qmask) {
      tree_->addProperty(objName, qmask, "x"     );
      tree_->addProperty(objName, qmask, "y"     );
      tree_->addProperty(objName, qmask, "width" );
      tree_->addProperty(objName, qmask, "height");
    }
    else if (qpath) {
      tree_->addProperty(objName, qpath, "pathString");
    }
    else if (qpattern) {
      tree_->addProperty(objName, qpattern, "x"           );
      tree_->addProperty(objName, qpattern, "y"           );
      tree_->addProperty(objName, qpattern, "width"       );
      tree_->addProperty(objName, qpattern, "height"      );
      tree_->addProperty(objName, qpattern, "units"       );
      tree_->addProperty(objName, qpattern, "contentUnits");
    }
    else if (qpolygon) {
    }
    else if (qpolyline) {
    }
    else if (qrgrad) {
      tree_->addProperty(objName, qrgrad, "cx");
      tree_->addProperty(objName, qrgrad, "cy");
      tree_->addProperty(objName, qrgrad, "r" );
      tree_->addProperty(objName, qrgrad, "fx");
      tree_->addProperty(objName, qrgrad, "fy");
    }
    else if (qrect) {
      tree_->addProperty(objName, qrect, "x" );
      tree_->addProperty(objName, qrect, "y" );
      tree_->addProperty(objName, qrect, "w" );
      tree_->addProperty(objName, qrect, "h" );
      tree_->addProperty(objName, qrect, "rx");
      tree_->addProperty(objName, qrect, "ry");
    }
    else if (qstop) {
      tree_->addProperty(objName, qstop, "offset" );
      tree_->addProperty(objName, qstop, "opacity");
    }
    else if (qtext) {
      tree_->addProperty(objName, qtext, "x"           );
      tree_->addProperty(objName, qtext, "y"           );
      tree_->addProperty(objName, qtext, "dx"          );
      tree_->addProperty(objName, qtext, "dy"          );
      tree_->addProperty(objName, qtext, "text"        );
      tree_->addProperty(objName, qtext, "rotate"      );
      tree_->addProperty(objName, qtext, "textLength"  );
      tree_->addProperty(objName, qtext, "lengthAdjust");
    }
    else if (qtextpath) {
      tree_->addProperty(objName, qtextpath, "text"       );
      tree_->addProperty(objName, qtextpath, "xlink"      );
      tree_->addProperty(objName, qtextpath, "startOffset");
    }
    else if (qtitle) {
      tree_->addProperty(objName, qtitle, "text");

      setWindowTitle(qtitle->getQText());
    }
    else if (qtspan) {
      tree_->addProperty(objName, qtspan, "x"   );
      tree_->addProperty(objName, qtspan, "y"   );
      tree_->addProperty(objName, qtspan, "text");
    }
    else if (quse) {
      tree_->addProperty(objName, quse, "x"     );
      tree_->addProperty(objName, quse, "y"     );
      tree_->addProperty(objName, quse, "width" );
      tree_->addProperty(objName, quse, "height");
      tree_->addProperty(objName, quse, "xlink" );
    }

    //---

    if (! qobj->object()->isAnimated()) {
      if (qobj->object()->isDrawable() || hasChildren)
        tree_->addProperty(objName, qobj, "transform");

      if (qobj->object()->isDrawable() || hasChildren) {
        tree_->addProperty(objName, qobj, "opacity");

        //---

        QString strokeName = objName + "/stroke";

        tree_->addProperty(strokeName, qobj, "strokeColor"  )->setLabel("color"  );
        tree_->addProperty(strokeName, qobj, "strokeOpacity")->setLabel("opacity");
        tree_->addProperty(strokeName, qobj, "strokeWidth"  )->setLabel("width"  );
        tree_->addProperty(strokeName, qobj, "strokeDash"   )->setLabel("dash"   );
        tree_->addProperty(strokeName, qobj, "strokeCap"    )->setLabel("cap"    );
        tree_->addProperty(strokeName, qobj, "strokeJoin"   )->setLabel("join"   );

        //---

        QString fillName = objName + "/fill";

        tree_->addProperty(fillName, qobj, "fillIsNoColor"     )->setLabel("isNoColor"     );
        tree_->addProperty(fillName, qobj, "fillIsCurrentColor")->setLabel("isCurrentColor");
        tree_->addProperty(fillName, qobj, "fillColor"         )->setLabel("color"         );
        tree_->addProperty(fillName, qobj, "fillOpacity"       )->setLabel("opacity"       );
        tree_->addProperty(fillName, qobj, "fillRule"          )->setLabel("rule"          );
        tree_->addProperty(fillName, qobj, "fillUrl"           )->setLabel("url"           );
      }

      if (qobj->object()->hasFont()) {
        QString fontName = objName + "/font";

        tree_->addProperty(fontName, qobj, "fontFamily")->setLabel("family");
      //tree_->addProperty(fontName, qobj, "fontStyle" )->setLabel("style");
        tree_->addProperty(fontName, qobj, "fontSize"  )->setLabel("size");
        tree_->addProperty(fontName, qobj, "font"      )->setLabel("font");
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

  CQSVGObject *qobj = dynamic_cast<CQSVGObject *>(obj);

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
  CSVGBlock *block = svg_->getBlock();

  deselectObjects(block);
}

void
CQSVGWindow::
deselectObjects(CSVGObject *obj)
{
  CQSVGObject *qobj = dynamic_cast<CQSVGObject *>(obj);

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
print() const
{
  svg_->print(std::cout, true);
}

void
CQSVGWindow::
showProperties()
{
  if (! propertiesDlg_) {
    propertiesDlg_ = new CQSVGPropertiesDlg(this);

    addProperties();
  }

  propertiesDlg_->show();
}

void
CQSVGWindow::
showBuffers()
{
  if (! bufferView_)
    bufferView_ = new CQSVGBufferView(svg_);

  bufferView_->show();
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

QSize
CQSVGWindow::
sizeHint() const
{
  return QSize(800, 800);
}
