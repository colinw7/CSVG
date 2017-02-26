#include <CQSVGRenderer.h>
#include <CQSVGImageData.h>
#include <CQSVGFontObj.h>
#include <CSVGFontDef.h>
#include <CQUtil.h>
#include <CLinearGradient.h>
#include <CRadialGradient.h>

namespace {
  CRGBA getPixel(const QImage &image, int x, int y) {
    QRgb rgb = image.pixel(x, y);

    double r = qRed  (rgb)/255.0;
    double g = qGreen(rgb)/255.0;
    double b = qBlue (rgb)/255.0;
    double a = qAlpha(rgb)/255.0;

    return CRGBA(r, g, b, a);
  }

  void setPixel(QImage &image, int x, int y, const CRGBA &rgba) {
    QRgb rgb = qRgba(rgba.getRedI(), rgba.getGreenI(), rgba.getBlueI(), rgba.getAlphaI());

    image.setPixel(x, y, rgb);
  }

  void clipOutside(QImage &image, int x1, int y1, int x2, int y2) {
    QRgb rgb = qRgba(0, 0, 0, 0);

    for (int y = 0; y < image.height(); ++y) {
      for (int x = 0; x < image.width(); ++x) {
        if (y >= y1 && y <= y2 && x >= x1 && x <= x2)
          continue;

        image.setPixel(x, y, rgb);
      }
    }
  }

  QImage createImage(int w, int h) {
    QImage image = QImage(QSize(w, h), QImage::Format_ARGB32);

    image.fill(0);

    return image;
  }

#if 0
  QImage alphaImage(const QImage &image, int gray=0) {
    QImage image1 = createImage(image.width(), image.height());

    for (int y = 0; y < image.height(); ++y) {
      for (int x = 0; x < image.width(); ++x) {
        QRgb rgb = image.pixel(x, y);

        int a = qAlpha(rgb);

        image1.setPixel(x, y, qRgba(gray, gray, gray, a));
      }
    }

    return image1;
  }
#endif

  QImage opacityImage(const QImage &image, double opacity) {
    QImage image1 = createImage(image.width(), image.height());

    for (int y = 0; y < image.height(); ++y) {
      for (int x = 0; x < image.width(); ++x) {
        QRgb rgb = image.pixel(x, y);

        int r = qRed  (rgb);
        int g = qGreen(rgb);
        int b = qBlue (rgb);
        int a = qAlpha(rgb)*opacity;

        if (a > 0)
          image1.setPixel(x, y, qRgba(r, g, b, a));
      }
    }

    return image1;
  }
}

//------

CQSVGRenderer::
CQSVGRenderer()
{
  imageData_ = new CQSVGImageData;

  painter_ = new QPainter;

  viewMatrix_.setIdentity();

  int width  = 100;
  int height = 100;

  setSize(width, height);

  setPixelRange(width, height);

  setDataRange(0, 0, width, height);

  range_.setEqualScale(false);
  range_.setScaleMin  (false);
}

CQSVGRenderer::
~CQSVGRenderer()
{
  delete imageData_;

  delete path_;
  delete savePath_;
}

CQSVGRenderer *
CQSVGRenderer::
dup() const
{
  return new CQSVGRenderer;
}

void
CQSVGRenderer::
setSize(int width, int height)
{
  assert(! drawing_);

  imageData_->setSize(width, height);

  imageData_->clear();
}

void
CQSVGRenderer::
getSize(int *width, int *height) const
{
  *width  = imageData_->getWidth ();
  *height = imageData_->getHeight();
}

void
CQSVGRenderer::
setPixelRange(int width, int height)
{
  if (width  < 1) width  = 1;
  if (height < 1) height = 1;

  range_.setPixelRange(0, height - 1, width - 1, 0);
}

void
CQSVGRenderer::
setDataRange(double xmin, double ymin, double xmax, double ymax)
{
  range_.setWindowRange(xmin, ymin, xmax, ymax);

  if (drawing_) {
    painter_->setWorldMatrix(CQUtil::toQMatrix(range_.getMatrix()*viewMatrix_));

    transform_  = painter_->worldTransform();
    itransform_ = transform_.inverted();
  }
}

void
CQSVGRenderer::
getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const
{
  range_.getWindowRange(xmin, ymin, xmax, ymax);
}

void
CQSVGRenderer::
setAntiAlias(bool aa)
{
  antiAlias_ = aa;
}

void
CQSVGRenderer::
setEqualScale(bool eq)
{
  range_.setEqualScale(eq);
}

void
CQSVGRenderer::
setScaleMin(bool sm)
{
  range_.setScaleMin(sm);
}

void
CQSVGRenderer::
beginDraw()
{
  startDraw();

  painter_->setWorldMatrixEnabled(false);

  QRect irect(0, 0, imageData_->getWidth(), imageData_->getHeight());

  painter_->fillRect(irect, QBrush(QColor(0,0,0,0)));

  painter_->setWorldMatrixEnabled(true);

  transform_  = painter_->worldTransform();
  itransform_ = transform_.inverted();
}

void
CQSVGRenderer::
endDraw()
{
  stopDraw();
}

void
CQSVGRenderer::
startDraw()
{
  assert(! drawing_);

  QImage *qimage = imageData_->lockImage();

  painter_->begin(qimage);

  drawing_ = true;

  if (antiAlias_)
    painter_->setRenderHints(QPainter::Antialiasing |
                             QPainter::TextAntialiasing |
                             QPainter::SmoothPixmapTransform);

  painter_->setWorldMatrix(CQUtil::toQMatrix(range_.getMatrix()*viewMatrix_));
}

void
CQSVGRenderer::
stopDraw()
{
  assert(drawing_);

  painter_->end();

  imageData_->unlockImage();

  drawing_ = false;
}

void
CQSVGRenderer::
setViewMatrix(const CMatrix2D &m)
{
  viewMatrix_ = m;

  if (drawing_) {
    painter_->setWorldMatrix(CQUtil::toQMatrix(range_.getMatrix()*viewMatrix_));

    transform_  = painter_->worldTransform();
    itransform_ = transform_.inverted();
  }
}

void
CQSVGRenderer::
clear(const CRGBA &bg)
{
  if (drawing_) {
    QRect irect(0, 0, imageData_->getWidth(), imageData_->getHeight());

    if (bg.getAlphaI() > 0)
      painter_->fillRect(irect, QBrush(CQUtil::rgbaToColor(bg)));
    else
      painter_->fillRect(irect, Qt::transparent);
  }
  else {
    if (bg.getAlphaI() > 0)
      imageData_->set(bg);
    else
      imageData_->clear();
  }
}

void
CQSVGRenderer::
pathInit()
{
  delete path_;

  path_ = new QPainterPath;

  path_->setFillRule(Qt::WindingFill);

  pathEmpty_ = true;
}

void
CQSVGRenderer::
pathMoveTo(const CPoint2D &p)
{
  path_->moveTo(p.x, p.y);

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathRMoveTo(const CPoint2D &p)
{
  CPoint2D c = CQUtil::fromQPoint(path_->currentPosition());

  path_->moveTo(c.x + p.x, c.y + p.y);

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathLineTo(const CPoint2D &p)
{
  path_->lineTo(p.x, p.y);

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathRLineTo(const CPoint2D &p)
{
  CPoint2D c = CQUtil::fromQPoint(path_->currentPosition());

  path_->lineTo(c.x + p.x, c.y + p.y);

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathCurveTo(const CPoint2D &p1, const CPoint2D &p2)
{
  path_->quadTo(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2)
{
  CPoint2D c = CQUtil::fromQPoint(path_->currentPosition());

  path_->quadTo(CQUtil::toQPoint(c + p1), CQUtil::toQPoint(c + p2));

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  path_->cubicTo(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2), CQUtil::toQPoint(p3));

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  CPoint2D c = CQUtil::fromQPoint(path_->currentPosition());

  path_->cubicTo(CQUtil::toQPoint(c + p1), CQUtil::toQPoint(c + p2), CQUtil::toQPoint(c + p3));

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathArcTo(const CPoint2D &c, double xr, double yr, double angle1, double angle2)
{
  QRectF rect(QPointF(c.x - xr, c.y - yr), QPointF(c.x + xr, c.y + yr));

  double a1 = -CMathGen::RadToDeg(angle1);
  double a2 = -CMathGen::RadToDeg(angle2);

  path_->arcTo(rect, a1, a2 - a1);

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathText(const std::string &str)
{
  QString qstr = QString::fromUtf8(str.c_str());

  QPointF c = path_->currentPosition();

  path_->addText(c, qfont_, qstr);

  pathEmpty_ = false;
}

void
CQSVGRenderer::
pathClose()
{
  path_->closeSubpath();

  pathEmpty_ = false;
}

bool
CQSVGRenderer::
pathGetCurrentPoint(CPoint2D &p)
{
  if (pathEmpty_)
    return false;

  p = CQUtil::fromQPoint(path_->currentPosition());

  return true;
}

void
CQSVGRenderer::
pathStroke()
{
  assert(drawing_);

  if (strokeFilled_) {
    QPainterPathStroker stroker;

    stroker.setCapStyle   (pen_.capStyle());
    stroker.setDashOffset (pen_.dashOffset());
    stroker.setDashPattern(pen_.dashPattern());
    stroker.setJoinStyle  (pen_.joinStyle());
    stroker.setMiterLimit (pen_.miterLimit());
    stroker.setWidth      (pen_.widthF());

    painter_->fillPath(stroker.createStroke(*path_), strokeBrush_);
  }
  else {
    painter_->strokePath(*path_, pen_);
  }
}

void
CQSVGRenderer::
pathFill()
{
  assert(drawing_);

  path_->setFillRule(fillType_ == FILL_TYPE_EVEN_ODD ? Qt::OddEvenFill : Qt::WindingFill);

  painter_->fillPath(*path_, fillBrush_);
}

void
CQSVGRenderer::
savePath(const CMatrix2D &m)
{
  QTransform matrix = CQUtil::toQTransform(m);

  QList<QPolygonF> polygons = path_->toSubpathPolygons(matrix);

  delete savePath_;

  savePath_ = new QPainterPath;

  for (int i = 0; i < polygons.size(); ++i)
    savePath_->addPolygon(polygons[i]);
}

void
CQSVGRenderer::
pathClip(CSVGRenderer *renderer)
{
  CQSVGRenderer *qrenderer = dynamic_cast<CQSVGRenderer *>(renderer);

  if (qrenderer) {
    if (! qrenderer->savePath_)
      qrenderer->savePath_ = new QPainterPath;

    painter_->setClipPath(*qrenderer->savePath_);
  }
  else
    painter_->setClipPath(*path_);
}

void
CQSVGRenderer::
pathEoclip(CSVGRenderer *renderer)
{
  // TODO: even odd ?
  CQSVGRenderer *qrenderer = dynamic_cast<CQSVGRenderer *>(renderer);

  if (qrenderer) {
    if (! qrenderer->savePath_)
      qrenderer->savePath_ = new QPainterPath;

    painter_->setClipPath(*qrenderer->savePath_);
  }
  else
    painter_->setClipPath(*path_);
}

void
CQSVGRenderer::
initClip()
{
  painter_->setClipping(false);
}

void
CQSVGRenderer::
addClipPath(CSVGRenderer *origRenderer)
{
  painter_->setClipPath(static_cast<CQSVGRenderer *>(origRenderer)->painter_->clipPath());
}

void
CQSVGRenderer::
pathBBox(CBBox2D &bbox)
{
  bbox = CQUtil::fromQRect(path_->boundingRect());
}

void
CQSVGRenderer::
drawImage(const CPoint2D &p, CSVGImageData *image)
{
  assert(drawing_);

  QTransform transform = painter_->worldTransform();

  QPointF p1 = transform.map(CQUtil::toQPoint(p));

  painter_->setWorldMatrixEnabled(false);

  painter_->drawImage(p1, static_cast<CQSVGImageData *>(image)->qimage());

  painter_->setWorldMatrixEnabled(true);
}

void
CQSVGRenderer::
setFont(const CSVGFontDef &fontDef)
{
  assert(drawing_);

  //std::string family = fontDef.getFamily();
  //CFontStyles styles = fontDef.getStyle();
  //CFontStyle  style  = (styles | CFONT_STYLE_FULL_SIZE).value();
  //double      size   = fontDef.getSize().px().value();
  //double      angle  = fontDef.getAngle();

  //CFontPtr font = CFontMgrInst->lookupFont(family, style, size, angle);

  //qfont_ = CQUtil::toQFont(font);

  CQSVGFontObj *fontObj = dynamic_cast<CQSVGFontObj *>(fontDef.getObj());

  if (fontObj) {
    qfont_ = fontObj->font();

    painter_->setFont(qfont_);
  }
}

void
CQSVGRenderer::
setStrokeColor(const CRGBA &fg)
{
  assert(drawing_);

  pen_.setColor(CQUtil::rgbaToColor(fg));

  painter_->setPen(pen_);
}

void
CQSVGRenderer::
setLineWidth(double w)
{
  assert(drawing_);

  //if (w <= 1.0) w = 1.0;

  pen_.setWidthF(w);

  painter_->setPen(pen_);
}

void
CQSVGRenderer::
setLineDash(const CLineDash &dash)
{
  assert(drawing_);

  int num = dash.getNumLengths();

  if (num > 0) {
    pen_.setStyle(Qt::CustomDashLine);

    pen_.setDashOffset(dash.getOffset()/pen_.widthF());

    QVector<qreal> dashes;

    for (int i = 0; i < num; ++i)
      dashes << dash.getLength(i)/pen_.widthF();

    if (num & 1)
      dashes << dash.getLength(0)/pen_.widthF();

    pen_.setDashPattern(dashes);
  }
  else
    pen_.setStyle(Qt::SolidLine);

  painter_->setPen(pen_);
}

void
CQSVGRenderer::
setLineCap(const CLineCapType &cap)
{
  assert(drawing_);

  switch (cap) {
    case LINE_CAP_TYPE_BUTT:
      pen_.setCapStyle(Qt::FlatCap);
      break;
    case LINE_CAP_TYPE_ROUND:
      pen_.setCapStyle(Qt::RoundCap);
      break;
    case LINE_CAP_TYPE_SQUARE:
      pen_.setCapStyle(Qt::SquareCap);
      break;
    default:
      break;
  }

  painter_->setPen(pen_);
}

void
CQSVGRenderer::
setLineJoin(const CLineJoinType &join)
{
  assert(drawing_);

  switch (join) {
    case LINE_JOIN_TYPE_MITRE:
      pen_.setJoinStyle(Qt::MiterJoin);
      break;
    case LINE_JOIN_TYPE_ROUND:
      pen_.setJoinStyle(Qt::RoundJoin);
      break;
    case LINE_JOIN_TYPE_BEVEL:
      pen_.setJoinStyle(Qt::BevelJoin);
      break;
    default:
      break;
  }

  painter_->setPen(pen_);
}

void
CQSVGRenderer::
setMitreLimit(double limit)
{
  assert(drawing_);

  pen_.setMiterLimit(limit);

  painter_->setPen(pen_);
}

void
CQSVGRenderer::
resetFill()
{
  fillBrush_ = QBrush();

  painter_->setBrush(fillBrush_);
}

void
CQSVGRenderer::
setFillColor(const CRGBA &fg)
{
  assert(drawing_);

  fillBrush_.setColor(CQUtil::rgbaToColor(fg));
  fillBrush_.setStyle(Qt::SolidPattern);

  painter_->setBrush(fillBrush_);
}

void
CQSVGRenderer::
setFillType(CFillType fillType)
{
  fillType_ = fillType;
}

void
CQSVGRenderer::
setFillGradient(CGenGradient *g)
{
  assert(drawing_);

  painter_->setBrushOrigin(QPointF(0,0));

  const CLinearGradient *lg = 0;
  const CRadialGradient *rg = 0;

  if      ((lg = dynamic_cast<const CLinearGradient *>(g)) != 0)
    fillBrush_ = QBrush(CQUtil::toQGradient(lg, QGradient::LogicalMode));
  else if ((rg = dynamic_cast<const CRadialGradient *>(g)) != 0)
    fillBrush_ = QBrush(CQUtil::toQGradient(rg, QGradient::LogicalMode));

  painter_->setBrush(fillBrush_);
}

void
CQSVGRenderer::
setFillImage(double xo, double yo, CSVGImageData *image)
{
  assert(drawing_);

  if (! image)
    return;

  fillBrush_.setTextureImage(static_cast<CQSVGImageData *>(image)->qimage());

  painter_->setBrush(fillBrush_);

  painter_->setBrushOrigin(QPointF(xo, yo));
}

void
CQSVGRenderer::
setFillMatrix(const CMatrix2D &m)
{
  assert(drawing_);

  fillBrush_.setMatrix(CQUtil::toQMatrix(m));

  painter_->setBrush(fillBrush_);
}

void
CQSVGRenderer::
setStrokeFilled(bool b)
{
  strokeFilled_ = b;
}

void
CQSVGRenderer::
setStrokeFillType(CFillType fillType)
{
  strokeFillType_ = fillType;
}

void
CQSVGRenderer::
setStrokeFillGradient(CGenGradient *g)
{
  assert(drawing_);

  painter_->setBrushOrigin(QPointF(0,0));

  const CLinearGradient *lg = 0;
  const CRadialGradient *rg = 0;

  if      ((lg = dynamic_cast<const CLinearGradient *>(g)) != 0)
    strokeBrush_ = QBrush(CQUtil::toQGradient(lg, QGradient::LogicalMode));
  else if ((rg = dynamic_cast<const CRadialGradient *>(g)) != 0)
    strokeBrush_ = QBrush(CQUtil::toQGradient(rg, QGradient::LogicalMode));

  painter_->setBrush(strokeBrush_);
}

void
CQSVGRenderer::
setStrokeFillImage(CSVGImageData *image)
{
  assert(drawing_);

  if (! image)
    return;

  strokeBrush_.setTextureImage(static_cast<CQSVGImageData *>(image)->qimage());

  painter_->setBrush(strokeBrush_);
}

void
CQSVGRenderer::
setAlign(CHAlignType halign, CVAlignType valign)
{
  range_.setAlign(halign, valign);
}

void
CQSVGRenderer::
windowToPixel(const CPoint2D &w, CPoint2D &p)
{
  p = CQUtil::fromQPoint(transform_.map(CQUtil::toQPoint(w)));
}

void
CQSVGRenderer::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  w = CQUtil::fromQPoint(itransform_.map(CQUtil::toQPoint(p)));
}

void
CQSVGRenderer::
textBounds(const std::string &str, CBBox2D &bbox)
{
  QString qstr = QString::fromUtf8(str.c_str());

  QFontMetrics fm(qfont_);

  int w = fm.width(qstr);
  int h = fm.height();

  bbox = CBBox2D(CPoint2D(0, 0), CPoint2D(w, h));
}

CISize2D
CQSVGRenderer::
getImageSize() const
{
  return CISize2D(imageData_->getWidth(), imageData_->getHeight());
}

CSVGImageData *
CQSVGRenderer::
getImage() const
{
  return imageData_->dup();
}

void
CQSVGRenderer::
setImage(CSVGRenderer *renderer)
{
  CQSVGRenderer *qrenderer = dynamic_cast<CQSVGRenderer *>(renderer);
  assert(qrenderer);

  imageData_->setQImage(qrenderer->imageData_->qimage());
}

void
CQSVGRenderer::
setImage(CSVGImageData *image)
{
  CQSVGImageData *imageData = dynamic_cast<CQSVGImageData *>(image);
  assert(imageData);

  setQImage(imageData->qimage());
}

void
CQSVGRenderer::
addImage(int x, int y, CSVGImageData *image)
{
  CQSVGImageData *imageData = dynamic_cast<CQSVGImageData *>(image);
  assert(imageData);

  QImage *qimage = imageData_->lockImage();

  QImage qimage1 = imageData->qimage();

  combineImage(*qimage, x, y, qimage1);

  imageData_->unlockImage();
}

void
CQSVGRenderer::
combine(int ix, int iy, CSVGRenderer *r)
{
  CQSVGRenderer *qr = dynamic_cast<CQSVGRenderer *>(r);
  assert(qr);

  int iwidth1, iheight1, iwidth2, iheight2;

     getSize(&iwidth1, &iheight1);
  r->getSize(&iwidth2, &iheight2);

  int w = std::max(iwidth1 , ix + iwidth2 );
  int h = std::max(iheight1, iy + iheight2);

  QImage image2 = qr->qimage();

  if (qr->opacity() < 1)
    image2 = opacityImage(image2, qr->opacity());

  if (w > iwidth1 || h > iheight1) {
    QImage image1 = imageData_->qimage();
    QImage image3 = createImage(w, h);

    image3.fill(0);

    combineImage(image3,  0,  0, image1);
    combineImage(image3, ix, iy, image2);

    setQImage(image3);
  }
  else {
    QImage *qimage = imageData_->lockImage();

    combineImage(*qimage, ix, iy, image2);

    imageData_->unlockImage();
  }
}

void
CQSVGRenderer::
combineImage(QImage &image1, int x, int y, const QImage &image2)
{
  // combine image2 into image1 at (x, y)

  CRGBA rgba1, rgba2;

  int w1 = image1.width ();
  int h1 = image1.height();

  int w2 = image2.width ();
  int h2 = image2.height();

  int w = std::min(w1, w2);
  int h = std::min(h1, h2);

  for (int y1 = 0; y1 < h; ++y1) {
    int y2 = y1 + y;

    if (y2 < 0 || y2 >= h1) continue;

    for (int x1 = 0; x1 < w; ++x1) {
      int x2 = x1 + x;

      if (x2 < 0 || x2 >= w1) continue;

      CRGBA rgba1 = getPixel(image2, x1, y1);

      if (! rgba1.getAlphaI())
        continue;

      CRGBA rgba2 = getPixel(image1, x2, y2);

      if (! rgba2.getAlphaI())
        setPixel(image1, x2, y2, rgba1);
      else {
        CRGBA rgba = rgba2.combined(rgba1);

        setPixel(image1, x2, y2, rgba);
      }
    }
  }
}

void
CQSVGRenderer::
addResizedImage(CSVGRenderer *src, double x, double y, double w, double h)
{
  CQSVGRenderer *qsrc = dynamic_cast<CQSVGRenderer *>(src);
  assert(qsrc);

  CPoint2D p;

  lengthToPixel(CPoint2D(w, h), p);

  QImage qimage1 = qsrc->imageData_->qimage();

  QImage qimage2 = qimage1.scaled(QSize(p.x, p.y), Qt::IgnoreAspectRatio, Qt::FastTransformation);

  windowToPixel(CPoint2D(x, y), p);

  // TODO: handle out of bounds
  QImage *qimage = imageData_->lockImage();

  combineImage(*qimage, p.x, p.y, qimage2);

  imageData_->unlockImage();
}

void
CQSVGRenderer::
addClippedImage(CSVGRenderer *src, int x, int y, int px1, int py1, int px2, int py2)
{
  CQSVGRenderer *qsrc = dynamic_cast<CQSVGRenderer *>(src);
  assert(qsrc);

  QImage qimage1 = qsrc->qimage();

  clipOutside(qimage1, px1, py1, px2, py2);

  QImage *qimage = imageData_->lockImage();

  combineImage(*qimage, x - px1, y - py1, qimage1);

  imageData_->unlockImage();
}

void
CQSVGRenderer::
setClippedImage(CSVGRenderer *src, int px1, int py1, int px2, int py2)
{
  CQSVGRenderer *qsrc = dynamic_cast<CQSVGRenderer *>(src);
  assert(qsrc);

  QImage qimage1 = qsrc->qimage();

  clipOutside(qimage1, px1, py1, px2, py2);

  imageData_->setQImage(qimage1);
}

void
CQSVGRenderer::
setOffsetImage(CSVGRenderer *src, int dx, int dy)
{
  CQSVGRenderer *qsrc = dynamic_cast<CQSVGRenderer *>(src);
  assert(qsrc);

  QImage src_image = qsrc->qimage();

  int w = src_image.width () + dx;
  int h = src_image.height() + dy;

  QImage dst_image = createImage(w, h);

  QPainter painter(&dst_image);

  painter.drawImage(QPoint(dx, dy), src_image);

  imageData_->setQImage(dst_image);
}

void
CQSVGRenderer::
setQImage(const QImage &qimage)
{
  // ensure qimage is correct size
  if (! drawing_) {
    setSize(qimage.width(), qimage.height());

    imageData_->setQImage(qimage);
  }
  else {
    if (painter_->worldMatrixEnabled()) {
      painter_->setWorldMatrixEnabled(false);

      QRect irect(0, 0, imageData_->getWidth(), imageData_->getHeight());

      painter_->fillRect(irect, Qt::transparent);

      painter_->drawImage(QPointF(0,0), qimage);

      painter_->setWorldMatrixEnabled(true);
    }
    else
      painter_->drawImage(QPointF(0,0), qimage);
  }
}

const QImage &
CQSVGRenderer::
qimage() const
{
  return imageData_->qimage();
}

void
CQSVGRenderer::
paint(QPainter *painter)
{
  painter->setWorldMatrixEnabled(false);

  QImage qimage = imageData_->qimage();

  painter->drawImage(QPointF(0,0), qimage);

  painter->setWorldMatrixEnabled(true);
}
