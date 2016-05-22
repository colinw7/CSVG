#include <CQSVGRenderer.h>
#include <CQUtil.h>
#include <CQImage.h>
#include <CLinearGradient.h>
#include <CRadialGradient.h>

CQSVGRenderer::
CQSVGRenderer()
{
  painter_ = new QPainter;

  viewMatrix_.setIdentity();

  setSize(100, 100);

  setDataRange(0, 0, 100, 100);

  range_.setEqualScale(false);
  range_.setScaleMin  (false);
}

CQSVGRenderer::
~CQSVGRenderer()
{
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
  if (width  < 1) width  = 1;
  if (height < 1) height = 1;

  if (width != qimage_.width() || height != qimage_.height())
    qimage_ = QImage(QSize(width, height), QImage::Format_ARGB32);

  qimage_.fill(0);

  setPixelRange(width, height);

  //range_.setWindowRange(0, 0, width, height);
}

void
CQSVGRenderer::
getSize(int *width, int *height) const
{
  *width  = qimage_.width ();
  *height = qimage_.height();
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

  painter_->fillRect(qimage_.rect(), QBrush(QColor(0,0,0,0)));

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

  painter_->begin(&qimage_);

  drawing_ = true;

  if (antiAlias_)
    painter_->setRenderHints(QPainter::Antialiasing);

  painter_->setWorldMatrix(CQUtil::toQMatrix(range_.getMatrix()*viewMatrix_));
}

void
CQSVGRenderer::
stopDraw()
{
  assert(drawing_);

  painter_->end();

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
    if (bg.getAlphaI() > 0)
      painter_->fillRect(qimage_.rect(), QBrush(CQUtil::rgbaToColor(bg)));
    else
      painter_->fillRect(qimage_.rect(), Qt::transparent);
  }
  else {
    if (bg.getAlphaI() > 0)
      qimage_.fill(CQUtil::rgbaToColor(bg).rgb());
    else
      qimage_.fill(0);
  }
}

void
CQSVGRenderer::
pathInit()
{
  delete path_;

  path_ = new QPainterPath;

  path_->setFillRule(Qt::WindingFill);
}

void
CQSVGRenderer::
pathMoveTo(const CPoint2D &p)
{
  path_->moveTo(p.x, p.y);
}

void
CQSVGRenderer::
pathRMoveTo(const CPoint2D &p)
{
  CPoint2D c = CQUtil::fromQPoint(path_->currentPosition());

  path_->moveTo(c.x + p.x, c.y + p.y);
}

void
CQSVGRenderer::
pathLineTo(const CPoint2D &p)
{
  path_->lineTo(p.x, p.y);
}

void
CQSVGRenderer::
pathRLineTo(const CPoint2D &p)
{
  CPoint2D c = CQUtil::fromQPoint(path_->currentPosition());

  path_->lineTo(c.x + p.x, c.y + p.y);
}

void
CQSVGRenderer::
pathCurveTo(const CPoint2D &p1, const CPoint2D &p2)
{
  path_->quadTo(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
}

void
CQSVGRenderer::
pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2)
{
  CPoint2D c = CQUtil::fromQPoint(path_->currentPosition());

  path_->quadTo(CQUtil::toQPoint(c + p1), CQUtil::toQPoint(c + p2));
}

void
CQSVGRenderer::
pathCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  path_->cubicTo(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2), CQUtil::toQPoint(p3));
}

void
CQSVGRenderer::
pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  CPoint2D c = CQUtil::fromQPoint(path_->currentPosition());

  path_->cubicTo(CQUtil::toQPoint(c + p1), CQUtil::toQPoint(c + p2), CQUtil::toQPoint(c + p3));
}

void
CQSVGRenderer::
pathArcTo(const CPoint2D &c, double xr, double yr, double angle1, double angle2)
{
  QRectF rect(QPointF(c.x - xr, c.y - yr), QPointF(c.x + xr, c.y + yr));

  double a1 = -CMathGen::RadToDeg(angle1);
  double a2 = -CMathGen::RadToDeg(angle2);

  path_->arcTo(rect, a1, a2 - a1);
}

void
CQSVGRenderer::
pathText(const std::string &str)
{
  QString qstr = QString::fromUtf8(str.c_str());

  QPointF c = path_->currentPosition();

  path_->addText(c, qfont_, qstr);
}

void
CQSVGRenderer::
pathClose()
{
  path_->closeSubpath();
}

bool
CQSVGRenderer::
pathGetCurrentPoint(CPoint2D &p)
{
  if (! path_->elementCount())
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
    if (qrenderer->savePath_)
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
    if (qrenderer->savePath_)
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
drawImage(const CPoint2D &p, CImagePtr image)
{
  assert(drawing_);

  QTransform transform = painter_->worldTransform();

  QPointF p1 = transform.map(CQUtil::toQPoint(p));

  painter_->setWorldMatrixEnabled(false);

  CQImage *cqimage = image.cast<CQImage>();

  painter_->drawImage(p1, cqimage->getQImage());

  painter_->setWorldMatrixEnabled(true);
}

void
CQSVGRenderer::
setFont(CFontPtr font)
{
  assert(drawing_);

  qfont_ = CQUtil::toQFont(font);

  painter_->setFont(qfont_);
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

    pen_.setDashOffset(dash.getOffset());

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
setFillImage(CImagePtr image)
{
  assert(drawing_);

  if (! image.isValid())
    return;

  CQImage *cqimage = image.cast<CQImage>();

  QImage qimage = cqimage->getQImage();

  fillBrush_.setTextureImage(qimage);

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
setStrokeFillImage(CImagePtr image)
{
  assert(drawing_);

  if (! image.isValid())
    return;

  CQImage *cqimage = image.cast<CQImage>();

  QImage qimage = cqimage->getQImage();

  strokeBrush_.setTextureImage(qimage);

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
  return CISize2D(qimage_.width(), qimage_.height());
}

CImagePtr
CQSVGRenderer::
getImage() const
{
  CImagePtr image = CImageMgrInst->createImage();

  CQImage *cqimage = image.cast<CQImage>();

  cqimage->setImage(qimage_);

  return image;
}

void
CQSVGRenderer::
setImage(CImagePtr image)
{
  CQImage *cqimage = image.cast<CQImage>();

  // ensure qimage is correct size
  if (! drawing_) {
    setSize(image->getWidth(), image->getHeight());

    qimage_ = cqimage->getQImage();
  }
  else {
    if (painter_->worldMatrixEnabled()) {
      painter_->setWorldMatrixEnabled(false);

      painter_->fillRect(qimage_.rect(), Qt::transparent);

      painter_->drawImage(QPointF(0,0), cqimage->getQImage());

      painter_->setWorldMatrixEnabled(true);
    }
    else
      painter_->drawImage(QPointF(0,0), cqimage->getQImage());
  }
}

void
CQSVGRenderer::
paint(QPainter *painter)
{
  painter->setWorldMatrixEnabled(false);

  painter->drawImage(QPointF(0,0), qimage_);

  painter->setWorldMatrixEnabled(true);
}
