#include <CQSVGCanvas.h>
#include <CQSVG.h>
#include <CQSVGWindow.h>
#include <CQSVGObject.h>
#include <CQSVGRenderer.h>
#include <CQSVGUtil.h>
#include <CQPropertyTree.h>
#include <QMouseEvent>

CQSVGCanvas::
CQSVGCanvas(CQSVGWindow *window, CQSVG *svg) :
 QWidget(window), window_(window), svg_(svg)
{
  setObjectName("canvas");

  renderer_ = new CQSVGRenderer;

  svg_->setRenderer(renderer_);

  opainter_ = new QPainter;

  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);
}

void
CQSVGCanvas::
redraw()
{
  if (renderer_->isDrawing())
    return;

  renderer_->setSize(width(), height());

  if (autoScale_) {
    double sx = (1.0*width ())/svg_->getWidth ();
    double sy = (1.0*height())/svg_->getHeight();

    scale_ = std::min(sx, sy);
  }

  double bw = svg_->getWidth ()*scale_;
  double bh = svg_->getHeight()*scale_;

  renderer_->setPixelRange(bw, bh);

  renderer_->setBackground(svg_->CSVG::background());

  if (width() != oimage_.width() || height() != oimage_.height())
    oimage_ = QImage(QSize(width(), height()), QImage::Format_ARGB32);

  oimage_.fill(0);

  //---

  //CSVG::ObjectList objs;

  //svg_->getAllChildren(objs);

  //int n = objs.size();

  //---

  if (window_->isInitialized()) {
    window_->startBusy();

    opainter_->begin(&oimage_);

    svg_->draw(offset_, scale_, scale_);

    opainter_->end();

    window_->endBusy();
  }
}

void
CQSVGCanvas::
resizeEvent(QResizeEvent *)
{
  redraw();
}

void
CQSVGCanvas::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  if (! svg_->isCheckerboard())
    painter.fillRect(rect(), svg_->background());
  else
    CQSVGUtil::drawCheckerboard(&painter, 32);

  renderer_->paint(&painter);

  //painter_->setWorldMatrixEnabled(false);

  painter.drawImage(0, 0, oimage_);

  //painter_->setWorldMatrixEnabled(true);
}

void
CQSVGCanvas::
mousePressEvent(QMouseEvent *e)
{
  CQPropertyTree *tree = window_->propertiesTree();

  if (tree) {
    tree->blockSignals(true);

    tree->clearSelection();
  }

  window_->deselectAllObjects();

  CPoint2D w;

  pixelToWindow(CPoint2D(e->pos().x(), e->pos().y()), w);

  std::vector<CSVGObject *> objects;

  svg_->getObjectsAtPoint(w, objects);

  //std::cout << objects.size() << std::endl;

  for (const auto &obj : objects) {
    auto *qobj = dynamic_cast<CQSVGObject *>(obj);
    if (! qobj) continue;

    qobj->setSelected(true);

    if (tree)
      tree->selectObject(qobj);

    //std::string id = obj->getObjName() + std::to_string(obj->getInd());
    //std::cout << id << std::endl;
  }

  if (tree)
   tree->blockSignals(false);

  redraw();

  update();
}

void
CQSVGCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  CPoint2D w;

  pixelToWindow(CPoint2D(e->pos().x(), e->pos().y()), w);

  std::vector<CSVGObject *> objects;

  svg_->getAllChildren(objects);

  for (const auto &obj : objects) {
    auto *qobj = dynamic_cast<CQSVGObject *>(obj);
    if (! qobj) continue;

    if (obj->inside(w)) {
      if (! obj->getInside()) {
        obj->setInside(true);

        obj->handleEvent(CSVGEventType::MOUSE_OVER);
      }
    }
    else {
      if (obj->getInside()) {
        obj->setInside(false);

        obj->handleEvent(CSVGEventType::MOUSE_OUT);
      }
    }
  }

  window_->showPos(e->pos(), QPointF(w.x, w.y));
}

void
CQSVGCanvas::
mouseReleaseEvent(QMouseEvent *e)
{
  CPoint2D w;

  pixelToWindow(CPoint2D(e->pos().x(), e->pos().y()), w);

  std::vector<CSVGObject *> objects;

  svg_->getObjectsAtPoint(w, objects);

  for (const auto &obj : objects) {
    auto *qobj = dynamic_cast<CQSVGObject *>(obj);
    if (! qobj) continue;

    qobj->object()->handleEvent(CSVGEventType::CLICK, "", "", /*propagate*/false);

    if (qobj->object()->getId() != "")
      svg_->sendEvent(CSVGEventType::CLICK, qobj->object()->getId(), "");
  }
}

void
CQSVGCanvas::
keyPressEvent(QKeyEvent *ke)
{
  if      (ke->key() == Qt::Key_Plus)
    scale_ *= 1.20;
  else if (ke->key() == Qt::Key_Minus)
    scale_ /= 1.20;
  else if (ke->key() == Qt::Key_Home) {
    offset_ = CPoint2D(0, 0);
    scale_  = 1;
  }
  else if (ke->key() == Qt::Key_Left)
    offset_.x -= 4;
  else if (ke->key() == Qt::Key_Right)
    offset_.x += 4;
  else if (ke->key() == Qt::Key_Up)
    offset_.y -= 4;
  else if (ke->key() == Qt::Key_Down)
    offset_.y += 4;
  else if (ke->key() == Qt::Key_A)
    autoScale_ = ! autoScale_;
  else
    return;

  redraw();

  update();
}

void
CQSVGCanvas::
drawSelected()
{
  oimage_.fill(0);

  opainter_->begin(&oimage_);

  std::vector<CSVGObject *> objects;

  auto *block = svg_->getRoot();

  auto *qblock = dynamic_cast<CQSVGObject *>(block);

  if (qblock)
    qblock->drawSelected();

  block->getAllChildren(objects);

  for (const auto &obj : objects) {
    auto *qobj = dynamic_cast<CQSVGObject *>(obj);
    if (! qobj) continue;

    qobj->drawSelected();
  }

  opainter_->end();

  update();
}

void
CQSVGCanvas::
drawRect(const CBBox2D &bbox, const QColor &c)
{
  CPoint2D p1, p2;

  renderer_->windowToPixel(bbox.getLL(), p1);
  renderer_->windowToPixel(bbox.getUR(), p2);

  opainter_->setPen(c);

  opainter_->drawLine(p1.x, p1.y, p2.x, p1.y);
  opainter_->drawLine(p2.x, p1.y, p2.x, p2.y);
  opainter_->drawLine(p2.x, p2.y, p1.x, p2.y);
  opainter_->drawLine(p1.x, p2.y, p1.x, p1.y);
}

void
CQSVGCanvas::
drawLine(const CPoint2D &p1, const CPoint2D &p2, const QColor &c)
{
  opainter_->setPen(c);

  opainter_->setPen(c);

  opainter_->drawLine(p1.x, p1.y, p2.x, p2.y);
}

void
CQSVGCanvas::
drawPoints(std::initializer_list<CPoint2D> points, Shape shape, int size,
           const QColor &bg, const QColor &fg)
{
  for (const auto &p : points)
    drawPoint(p, shape, size, bg, fg);
}

void
CQSVGCanvas::
drawPoint(const CPoint2D &p, Shape /*shape*/, int s, const QColor &bg, const QColor &fg)
{
  CPoint2D p1, p2;

  renderer_->windowToPixel(CPoint2D(0, 0), p1);
  renderer_->windowToPixel(CPoint2D(s, s), p2);

  double w = std::abs(p2.x - p1.x);
  double h = std::abs(p2.y - p1.y);

  opainter_->setBrush(bg);
  opainter_->setPen  (fg);

  opainter_->drawEllipse(QRectF(QPointF(p.x - w/2, p.y - h/2), QSizeF(w, h)));
}

void
CQSVGCanvas::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  renderer_->pixelToWindow(p, w);

  w.x /= scale_;
  w.y /= scale_;
}
