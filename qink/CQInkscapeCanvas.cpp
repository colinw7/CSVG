#include <CQInkscapeCanvas.h>
#include <CQInkscapeWindow.h>

#include <CQSVGRenderer.h>
#include <CQSVGImageData.h>
#include <CQSVGFontObj.h>
#include <CSVG.h>
#include <CSVGFill.h>
#include <CSVGBuffer.h>
#include <CSVGTitle.h>
#include <CSVGDesc.h>
#include <CSVGRect.h>
#include <CSVGEllipse.h>

#include <CQUtil.h>
#include <CQRubberBand.h>

#include <QMouseEvent>
#include <QToolTip>

#include <iostream>

namespace CQInkscape {

//---

class SVG : public CSVG {
 public:
  SVG(Canvas *canvas) :
   CSVG(), canvas_(canvas) {
    setSelectedStroke(false);
  }

  Canvas *canvas() const { return canvas_; }

  CSVGImageData *createImageData() override {
    return new CQSVGImageData;
  }

  CSVGFontObj *createFontObj(const CSVGFontDef &def) override {
    return new CQSVGFontObj(def);
  }

  void drawBackground() override {
    auto viewBox = getRootBlock()->getViewBox();

    auto *buffer = getCurrentBuffer();

    auto bbox = buffer->bbox();

    if (! bbox.isSet())
       bbox = viewBox;

    //---

    if (canvas_->window()->isCheckerboard() && bbox.isSet()) {
      int cs = 32;

      int x = bbox.getXMin();
      int y = bbox.getYMin();
      int w = bbox.getWidth ();
      int h = bbox.getHeight();

      int nc = (w + cs - 1)/cs;
      int nr = (h + cs - 1)/cs;

      for (int r = 0; r < nr; ++r) {
        int y1 = y + r*cs;

        for (int c = 0; c < nc; ++c) {
          int x1 = x + c*cs;

          CRGBA rgba;

          if ((r + c) & 1)
            rgba = CRGBA(0.8, 0.8, 0.8);
          else
            rgba = CRGBA(1.0, 1.0, 1.0);

          CSVGFill fill(*this);

          fill.setColor(CSVGStroke::Color(CSVGColor(rgba)));

          setFill(fill);

          buffer->fillRectangle(CBBox2D(x1, y1, x1 + cs, y1 + cs));
        }
      }
    }

    //---

    if (canvas_->window()->isShowViewBox() && viewBox.isSet()) {
      CSVGStroke stroke(*this);

      stroke.setColor(CSVGStroke::Color(CSVGColor(CRGBA(0, 0, 0))));

      setStroke(stroke);

      buffer->drawRectangle(viewBox);
    }
  }

  void drawForeground() override {
    auto *buffer = getCurrentBuffer();

    CSVG::ObjectList objects;

    getSelectedObjects(objects);

    for (const auto &obj : objects) {
      CBBox2D bbox;

      if (! obj->getFlatTransformedBBox(bbox))
        continue;

      if (! bbox.isSet()) continue;

      CSVGStroke stroke(*this);
      CSVGFill   fill  (*this);

      if (canvas_->window()->mode() == Window::Mode::SELECT_POINT) {
        auto parts = obj->getPartList();

        double lw = 16;

        stroke.setColor(CSVGStroke::Color(CSVGColor(CRGBA(1, 0, 0))));
        stroke.setWidth(CSVGStroke::Width(lw));

        fill.setColor(CSVGStroke::Color(CSVGColor(CRGBA(1, 0, 0))));
        fill.setOpacity(CSVGStroke::Opacity(0.0));

        setStroke(stroke);
        setFill  (fill);

        auto transform = buffer->transform();

        buffer->setTransform(obj->getFlatTransform());

        buffer->drawParts(parts);

        buffer->setTransform(transform);
      }
      else {
        CSVGLineDash::Dashes dashes;

        dashes.push_back(CScreenUnits::makePixel(4));
        dashes.push_back(CScreenUnits::makePixel(4));

        stroke.setColor(CSVGStroke::Color(CSVGColor(CRGBA(0, 0, 0))));
        stroke.setDashArray(CSVGStroke::DashArray(CSVGLineDash(dashes)));

        setStroke(stroke);

        buffer->drawRectangle(bbox);
      }
    }
  }

 private:
  Canvas* canvas_ { nullptr };
};

//---


Canvas::
Canvas(Window *window) :
 QFrame(window), window_(window)
{
  setObjectName("canvas");

  //---

  svg_ = new SVG(this);

  renderer_ = new CQSVGRenderer;

  svg_->setRenderer(renderer_);

  updateMatrix();

  //---

  setMouseTracking(true);

  setFocusPolicy(Qt::StrongFocus);
}

void
Canvas::
print(const QString &filename)
{
  renderer_->getImage()->write(filename.toStdString(), CFILE_TYPE_IMAGE_PNG);
}

void
Canvas::
resizeEvent(QResizeEvent *)
{
  renderer_->setSize(width(), height());

  //renderer_->setImageSize(width(), height());

  redraw(/*update*/false);
}

void
Canvas::
redraw(bool update)
{
  //double xs = (svg_->getWidth () > 0.0 ? width ()/svg_->getWidth () : 1.0);
  //double ys = (svg_->getHeight() > 0.0 ? height()/svg_->getHeight() : 1.0);

  //svg_->setRootBlockXScale(xs);
  //svg_->setRootBlockYScale(ys);

  //double bw = std::max(1.0*width (), svg_->getWidth ());
  //double bh = std::max(1.0*height(), svg_->getHeight());

  renderer_->setImageSize(width(), height());

  renderer_->setPixelRange(svg_->getWidth(), svg_->getHeight());

  auto viewBox = svg_->getRootBlock()->getViewBox();

  if (viewBox.isSet())
    renderer_->setDataRange(viewBox.getXMin(), viewBox.getYMin(),
                            viewBox.getXMax(), viewBox.getYMax());

  renderer_->setBackground(svg_->background());

  svg_->draw(offset_, scale_, scale_);

  if (update)
    this->update();

  if (viewBox.isSet()) {
    // TODO: equal scale, scal min
    CPoint2D p1(offset_.x                          , offset_.y                           );
    CPoint2D p2(offset_.x + scale_*svg_->getWidth(), offset_.y + scale_*svg_->getHeight());

    range_.setPixelRange(p1.x, p2.y, p2.x, p1.y);

    range_.setWindowRange(viewBox.getXMin(), viewBox.getYMin(),
                          viewBox.getXMax(), viewBox.getYMax());
  }
}

void
Canvas::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  //CQSVGUtil::drawCheckerboard(&painter, 32);

  renderer_->paint(&painter);
}

void
Canvas::
mousePressEvent(QMouseEvent *me)
{
  if (me->button() == Qt::LeftButton) {
    pressed_ = true;

    bool isShift   = (me->modifiers() & Qt::ShiftModifier);
    bool isControl = (me->modifiers() & Qt::ControlModifier);

    pressPixel_ = me->pos();
    pressPoint_ = pixelToWindow(CPoint2D(pressPixel_.x(), pressPixel_.y()));
    movePixel_  = pressPixel_;
    movePoint_  = pressPoint_;

    //---

    auto mode = window()->mode();

    if (mode == Window::Mode::SELECT_OBJECT || mode == Window::Mode::SELECT_POINT) {
      CSVG::ObjectList objects;

      svg_->getObjectsAtPoint(pressPoint_, objects);

      if (! objects.empty()) {
        using AreaObjects  = std::map<double, CSVG::ObjectList>;
        using DepthObjects = std::map<int, AreaObjects>;

        DepthObjects depthObjects;

        for (const auto &obj : objects) {
          if (obj == svg_->getRoot())
            continue;

          if (! obj->isDrawable())
            continue;

          CBBox2D bbox;

          if (! obj->getFlatTransformedBBox(bbox))
            continue;

          if (mode == Window::Mode::SELECT_POINT) {
            auto parts = obj->getPartList();

            if (parts.empty())
              continue;
          }

          double area = bbox.area();

          depthObjects[obj->getDepth()][area].push_back(obj);
        }

        if (! depthObjects.empty()) {
          AreaObjects areaObjects;

          if (! isControl)
            areaObjects = depthObjects.begin()->second;
          else
            areaObjects = depthObjects.rbegin()->second;

          auto *obj = areaObjects.begin()->second.front();

          window_->setCurrentObject(obj);

          if (! isShift)
            window_->selectNone();

          if (! obj->getSelected()) {
            obj->setSelected(true);

            window_->updateCurrentObject();

            window_->redraw(true);
          }
        }
        else
          window_->selectNone();
      }
      else
        window_->selectNone();
    }
    else {
      if (! rubberBand_)
        rubberBand_ = new CQRubberBand(this);

      rubberBand_->setBounds(pressPixel_, movePixel_);

      rubberBand_->show();
    }
  }
}

void
Canvas::
mouseMoveEvent(QMouseEvent *me)
{
  movePixel_ = me->pos();
  movePoint_ = pixelToWindow(CPoint2D(movePixel_.x(), movePixel_.y()));

  if (pressed_) {
    auto mode = window()->mode();

    if (mode == Window::Mode::SELECT_OBJECT || mode == Window::Mode::SELECT_POINT) {
    }
    else {
      if (rubberBand_)
        rubberBand_->setBounds(pressPixel_, movePixel_);
    }
  }

  window_->setMousePos(movePoint_);
}

void
Canvas::
mouseReleaseEvent(QMouseEvent *me)
{
  movePoint_ = pixelToWindow(CPoint2D(me->x(), me->y()));

  if (rubberBand_)
    rubberBand_->hide();

  if (pressed_) {
    auto mode = window()->mode();

    if      (mode == Window::Mode::CREATE_RECT) {
      auto *svg = window()->svg();

      auto *rect = dynamic_cast<CSVGRect *>(svg->createObjectByName("rect"));

      rect->setX(CScreenUnits::makePixel(std::min(pressPoint_.x, movePoint_.x)));
      rect->setY(CScreenUnits::makePixel(std::min(pressPoint_.y, movePoint_.y)));

      rect->setWidth (CScreenUnits::makePixel(std::abs(pressPoint_.x - movePoint_.x)));
      rect->setHeight(CScreenUnits::makePixel(std::abs(pressPoint_.y - movePoint_.y)));

      svg->getRoot()->addChildObject(rect);

      window()->redraw(/*update*/true);
    }
    else if (mode == Window::Mode::CREATE_ELLIPSE) {
      auto *svg = window()->svg();

      auto *ellipse = dynamic_cast<CSVGEllipse *>(svg->createObjectByName("ellipse"));

      double cx = (pressPoint_.x + movePoint_.x)/2.0;
      double cy = (pressPoint_.y + movePoint_.y)/2.0;

      ellipse->setCenterX(CScreenUnits::makePixel(cx));
      ellipse->setCenterY(CScreenUnits::makePixel(cy));

      ellipse->setRadiusX(CScreenUnits::makePixel(std::abs(pressPoint_.x - movePoint_.x)/2.0));
      ellipse->setRadiusY(CScreenUnits::makePixel(std::abs(pressPoint_.y - movePoint_.y)/2.0));

      svg->getRoot()->addChildObject(ellipse);

      window()->redraw(/*update*/true);
    }
  }

  pressed_ = false;
}

void
Canvas::
keyPressEvent(QKeyEvent *ke)
{
  // zoom
  if      (ke->key() == Qt::Key_Plus)
    return zoomIn();
  else if (ke->key() == Qt::Key_Minus)
    return zoomOut();
  else if (ke->key() == Qt::Key_Home)
    return zoomReset();

  // pan
  if      (ke->key() == Qt::Key_Left)
    offset_.x -= 4;
  else if (ke->key() == Qt::Key_Right)
    offset_.x += 4;
  else if (ke->key() == Qt::Key_Up)
    offset_.y -= 4;
  else if (ke->key() == Qt::Key_Down)
    offset_.y += 4;
  else
    return;

  updateMatrix();

  redraw(/*update*/true);
}

void
Canvas::
zoomIn()
{
  scale_ *= 1.20;

  updateMatrix();

  redraw(/*update*/true);
}

void
Canvas::
zoomOut()
{
  scale_ /= 1.20;

  updateMatrix();

  redraw(/*update*/true);
}

void
Canvas::
zoomReset()
{
  offset_ = CPoint2D(0, 0);
  scale_  = 1;

  updateMatrix();

  redraw(/*update*/true);
}

bool
Canvas::
event(QEvent *e)
{
  if (e->type() == QEvent::ToolTip) {
    bool shown = false;

    auto *helpEvent = static_cast<QHelpEvent *>(e);

    auto p = helpEvent->pos();

    auto w = pixelToWindow(CPoint2D(p.x(), p.y()));

    CSVG::ObjectList objects;

    svg_->getObjectsAtPoint(w, objects);

    if (! objects.empty()) {
      QString tip;

      for (const auto &obj : objects) {
        if (! obj->isDrawable())
          continue;

        for (const auto &c : obj->children()) {
          auto *title = dynamic_cast<CSVGTitle *>(c);
          auto *desc  = dynamic_cast<CSVGDesc  *>(c);

          if      (title && title->hasText()) {
            if (title->isToolTip() || ! tip.length())
              tip = QString::fromStdString(title->getText());
          }
          else if (desc && desc->hasText()) {
            if (desc->isToolTip() || ! tip.length())
              tip = QString::fromStdString(desc->getText());
          }
        }
      }

      if (tip.length()) {
        QToolTip::showText(helpEvent->globalPos(), tip);

        shown = true;
      }
    }

    if (! shown) {
      QToolTip::hideText();
      e->ignore();
    }
  }

  return QWidget::event(e);
}

void
Canvas::
updateMatrix()
{
  matrix_ = CMatrix2D::translation(-offset_)*
            CMatrix2D::scale(scale_)*
            CMatrix2D::translation(offset_);

  imatrix_ = matrix_.inverse();
}

CPoint2D
Canvas::
pixelToWindow(const CPoint2D &p) const
{
  CPoint2D w;

  renderer_->pixelToWindow(p, w);

  range_.pixelToWindow(p, w);

  return w;
}

CPoint2D
Canvas::
windowToPixel(const CPoint2D &w) const
{
  CPoint2D p;

  renderer_->windowToPixel(w, p);

  return p;
}

}
