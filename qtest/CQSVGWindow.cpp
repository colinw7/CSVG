#include <CQSVGWindow.h>
#include <CQSVG.h>
#include <CQSVGCanvas.h>
#include <CQSVGPropertiesDlg.h>
#include <CQSVGBufferView.h>

#include <CQSVGAnchor.h>
#include <CQSVGBlock.h>
#include <CQSVGCircle.h>
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
#include <CQSVGMask.h>
#include <CQSVGPath.h>
#include <CQSVGPolygon.h>
#include <CQSVGPolyLine.h>
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

#include <CQApp.h>
#include <CQStyle.h>
#include <QSplitter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>

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

  if (! files.empty())
    window->loadFile(files[0], image, imageDir, print);

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

  QAction *propertiesAction = new QAction("&Properties", this);

  connect(propertiesAction, SIGNAL(triggered()), this, SLOT(showProperties()));

  viewMenu->addAction(propertiesAction);

  QAction *buffersAction = new QAction("&Buffers", this);

  connect(buffersAction, SIGNAL(triggered()), this, SLOT(showBuffers()));

  viewMenu->addAction(buffersAction);

  //---

  posLabel_ = new QLabel;
  posLabel_->setObjectName("posLabel");

  statusBar()->addPermanentWidget(posLabel_);

  zoomLabel_ = new QLabel;
  zoomLabel_->setObjectName("zoomLabel");

  statusBar()->addPermanentWidget(zoomLabel_);

  showPos(QPoint(0, 0), QPointF(0, 0));
}

void
CQSVGWindow::
loadFile(const std::string &filename, bool image, const std::string &imageDir, bool print)
{
  if (! CFile::isRegular(filename)) {
    std::cerr << "Invalid file \"" << filename << "\"" << std::endl;
    return;
  }

  if      (image) {
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

    if (imageDir != "")
      name = CStrUtil::strprintf("%s/svg_%s.png", imageDir.c_str(), base.c_str());
    else
      name = CStrUtil::strprintf("svg_%s.png", base.c_str());

    renderer.getImage()->write(name, CFILE_TYPE_IMAGE_PNG);
  }
  else if (print) {
    svg_->print(std::cout, true);
  }
  else {
    svg_->stopTimer();

    svg_->read(filename);

    addProperties();

    svg_->startTimer();
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

  CSVGBlock *block = svg_->getBlock();

  std::string id = block->getId();

  if (id == "")
    id = block->getUniqueName();

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
    CQSVGCircle             *qcircle   = dynamic_cast<CQSVGCircle             *>(obj);
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
    CQSVGMask               *qmask     = dynamic_cast<CQSVGMask               *>(obj);
    CQSVGPath               *qpath     = dynamic_cast<CQSVGPath               *>(obj);
    CQSVGPolygon            *qpolygon  = dynamic_cast<CQSVGPolygon            *>(obj);
    CQSVGPolyLine           *qpolyline = dynamic_cast<CQSVGPolyLine           *>(obj);
    CQSVGRect               *qrect     = dynamic_cast<CQSVGRect               *>(obj);
    CQSVGStop               *qstop     = dynamic_cast<CQSVGStop               *>(obj);
    CQSVGText               *qtext     = dynamic_cast<CQSVGText               *>(obj);
    CQSVGTextPath           *qtextpath = dynamic_cast<CQSVGTextPath           *>(obj);
    CQSVGTitle              *qtitle    = dynamic_cast<CQSVGTitle              *>(obj);
    CQSVGTSpan              *qtspan    = dynamic_cast<CQSVGTSpan              *>(obj);
    CQSVGUse                *quse      = dynamic_cast<CQSVGUse                *>(obj);

    //---

    QString objName = name.c_str();

    tree_->addProperty(objName, qobj, "id");

    if (qobj->object()->isDrawable() || qobj->object()->hasChildren())
      tree_->addProperty(objName, qobj, "visible");

    if (qobj->object()->getFilter())
      tree_->addProperty(objName, qobj, "filtered");

    //---

    if      (qblock) {
      tree_->addProperty(objName, qblock, "x"     );
      tree_->addProperty(objName, qblock, "y"     );
      tree_->addProperty(objName, qblock, "width" );
      tree_->addProperty(objName, qblock, "height");
    }
    else if (qanchor) {
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
    else if (qmask) {
      tree_->addProperty(objName, qmask, "x"     );
      tree_->addProperty(objName, qmask, "y"     );
      tree_->addProperty(objName, qmask, "width" );
      tree_->addProperty(objName, qmask, "height");
    }
    else if (qpath) {
    }
    else if (qpolygon) {
    }
    else if (qpolyline) {
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
      tree_->addProperty(objName, qtext, "x"   );
      tree_->addProperty(objName, qtext, "y"   );
      tree_->addProperty(objName, qtext, "text");
    }
    else if (qtextpath) {
      tree_->addProperty(objName, qtextpath, "text");
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
    }

    //---

    if (qobj->object()->isDrawable() || qobj->object()->hasChildren())
      tree_->addProperty(objName, qobj, "transform");

    if (qobj->object()->isDrawable() || qobj->object()->hasChildren()) {
      QString strokeName = objName + "/stroke";

      tree_->addProperty(strokeName, qobj, "strokeColor"  )->setLabel("color"  );
      tree_->addProperty(strokeName, qobj, "strokeOpacity")->setLabel("opacity");
      tree_->addProperty(strokeName, qobj, "strokeWidth"  )->setLabel("width"  );
      tree_->addProperty(strokeName, qobj, "strokeDash"   )->setLabel("dash"   );
      tree_->addProperty(strokeName, qobj, "strokeCap"    )->setLabel("cap"    );
      tree_->addProperty(strokeName, qobj, "strokeJoin"   )->setLabel("join"   );
    }

    if (qobj->object()->isDrawable() || qobj->object()->hasChildren()) {
      QString fillName = objName + "/fill";

      tree_->addProperty(fillName, qobj, "fillIsNoColor"     )->setLabel("isNoColor"     );
      tree_->addProperty(fillName, qobj, "fillIsCurrentColor")->setLabel("isCurrentColor");
      tree_->addProperty(fillName, qobj, "fillColor"         )->setLabel("color"         );
      tree_->addProperty(fillName, qobj, "fillOpacity"       )->setLabel("opacity"       );
      tree_->addProperty(fillName, qobj, "fillRule"          )->setLabel("rule"          );
      tree_->addProperty(fillName, qobj, "fillUrl"           )->setLabel("url"           );
    }
  }

  for (const auto &o : obj->children()) {
    std::string id = o->getId();

    if (id == "")
      id = o->getUniqueName();

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

QSize
CQSVGWindow::
sizeHint() const
{
  return QSize(800, 800);
}
