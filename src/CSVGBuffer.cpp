#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGRenderer.h>
#include <CSVGUtil.h>

CSVGBufferMgr::
CSVGBufferMgr(CSVG &svg) :
 svg_(svg)
{
}

CSVGBufferMgr::
~CSVGBufferMgr()
{
  for (const auto &b : bufferMap_)
    delete b.second;
}

CSVGBuffer *
CSVGBufferMgr::
lookupBuffer(const std::string &name, bool create)
{
  auto p = bufferMap_.find(name);

  if (p != bufferMap_.end())
    return (*p).second;

  if (! create)
    return 0;

  return createBuffer(name);
}

CSVGBuffer *
CSVGBufferMgr::
lookupAlphaBuffer(CSVGBuffer *buffer, bool create)
{
  auto p = alphaBufferMap_.find(buffer->getName());

  if (p != alphaBufferMap_.end())
    return (*p).second;

  if (! create)
    return 0;

  return createAlphaBuffer(buffer);
}

CSVGBuffer *
CSVGBufferMgr::
createBuffer(const std::string &name)
{
  CSVGBuffer *buffer = new CSVGBuffer(svg_, name);

  buffer->setAntiAlias(antiAlias_);

  addBuffer(buffer);

  return buffer;
}

CSVGBuffer *
CSVGBufferMgr::
createAlphaBuffer(CSVGBuffer *buffer)
{
  CSVGBuffer *alphaBuffer = new CSVGBuffer(buffer);

  addAlphaBuffer(alphaBuffer);

  return alphaBuffer;
}

void
CSVGBufferMgr::
addBuffer(CSVGBuffer *buffer)
{
  bufferMap_[buffer->getName()] = buffer;
}

void
CSVGBufferMgr::
addAlphaBuffer(CSVGBuffer *buffer)
{
  alphaBufferMap_[buffer->getName()] = buffer;
}

void
CSVGBufferMgr::
getBufferNames(std::vector<std::string> &names) const
{
  for (const auto &b : bufferMap_)
    names.push_back(b.first);
}

//------------

CSVGBuffer::
CSVGBuffer(CSVG &svg, const std::string &name) :
 svg_(svg), name_(name)
{
  transform_.reset();
}

CSVGBuffer::
CSVGBuffer(CSVGBuffer *refBuffer) :
 svg_(refBuffer->svg_), refBuffer_(refBuffer)
{
  if      (refBuffer_->name_ == "SourceGraphic")
    name_ = "SourceAlpha";
  else if (refBuffer_->name_ == "BackgroundImage")
    name_ = "BackgroundAlpha";
  else
    name_ = refBuffer_->name_ + "Alpha";
}

CSVGBuffer::
~CSVGBuffer()
{
  delete renderer_;
}

CSVGRenderer *
CSVGBuffer::
getRenderer() const
{
  if (refBuffer_)
    return refBuffer_->getRenderer();

  //---

  if (! renderer_) {
    CSVGBuffer *th = const_cast<CSVGBuffer *>(this);

    th->renderer_ = svg_.createRenderer();
  }

  return renderer_;
}

bool
CSVGBuffer::
isAntiAlias() const
{
  if (refBuffer_)
    return refBuffer_->isAntiAlias();

  //---

  return getRenderer()->isAntiAlias();
}

void
CSVGBuffer::
setAntiAlias(bool flag)
{
  if (refBuffer_)
    return refBuffer_->setAntiAlias(flag);

  //---

  getRenderer()->setAntiAlias(flag);
}

CISize2D
CSVGBuffer::
getImageSize() const
{
  if (refBuffer_)
    return refBuffer_->getImageSize();

  //---

  return getRenderer()->getImageSize();
}

CImagePtr
CSVGBuffer::
getImage() const
{
  if (refBuffer_) { // alpha
    CImagePtr image = refBuffer_->getImage();

    image->setAlphaGray(0);

    return image;
  }

  return getRenderer()->getImage();
}

void
CSVGBuffer::
setImage(CImagePtr image)
{
  if (refBuffer_)
    return refBuffer_->setImage(image);

  //---

  getRenderer()->setImage(image);
}

void
CSVGBuffer::
addImage(CImagePtr image)
{
  if (refBuffer_)
    return refBuffer_->addImage(image);

  //---

  addImage(0, 0, image);
}

void
CSVGBuffer::
addImage(double x, double y, CImagePtr image)
{
  if (refBuffer_)
    return refBuffer_->addImage(x, y, image);

  //---

  int ix = CSVGUtil::round(x);
  int iy = CSVGUtil::round(y);

  CImagePtr image1 = getRenderer()->getImage();

  uint w = std::max(image1->getWidth (), ix + image->getWidth ());
  uint h = std::max(image1->getHeight(), iy + image->getHeight());

  if (! drawing_ && (w > image1->getWidth() || h > image1->getHeight())) {
    renderer_->setSize(w, h);

    CImagePtr image2 = getRenderer()->getImage();

    image2->combine(image1);
    image2->combine(ix, iy, image);

    getRenderer()->setImage(image2);
  }
  else {
#if 0
    if (svg_.getDebugFilter()) {
      CSVGBuffer *buffer1 = svg_.getBuffer("_buffer1");
      CSVGBuffer *buffer2 = svg_.getBuffer("_buffer2");

      buffer1->setImage(image1);
      buffer2->setImage(image );
    }
#endif

    image1->combine(ix, iy, image);

#if 0
    if (svg_.getDebugFilter()) {
      CSVGBuffer *buffer3 = svg_.getBuffer("_buffer3");

      buffer3->setImage(image1);
    }
#endif

    if (drawing_)
      getRenderer()->stopDraw();

    getRenderer()->setImage(image1);

    if (drawing_)
      getRenderer()->startDraw();
  }
}

void
CSVGBuffer::
reset()
{
  if (refBuffer_)
    return refBuffer_->reset();

  //---

  CSVGRenderer *renderer = getRenderer();

  renderer->setDataRange(0, 0, 1, 1);

  CMatrixStack2D matrix;

  renderer->setViewMatrix(matrix.getMatrix());

  //------

  clear();

  //------

  renderer->setStrokeColor(CRGBA(0,0,0,1));
  renderer->setFillColor  (CRGBA(0,0,0,1));
}

void
CSVGBuffer::
clear()
{
  if (refBuffer_)
    return refBuffer_->clear();

  //---

  CSVGRenderer *renderer = getRenderer();

  renderer->clear(CRGBA());
}

void
CSVGBuffer::
setup(const CBBox2D &bbox)
{
  if (refBuffer_)
    return refBuffer_->setup(bbox);

  //---

  reset();

  CSVGRenderer *renderer = getRenderer();

  double x1 = bbox.getXMin();
  double y1 = bbox.getYMin();
  double x2 = bbox.getXMax();
  double y2 = bbox.getYMax();

  int w = CSVGUtil::round(x2 - x1);
  int h = CSVGUtil::round(y2 - y1);

  renderer->setSize(w, h);

  renderer->setDataRange(x1, y2, x2, y1);
}

void
CSVGBuffer::
beginDraw(double w, double h, const CBBox2D &bbox)
{
  if (refBuffer_)
    return refBuffer_->beginDraw(w, h, bbox);

  //---

  renderer_->setBackground(svg_.background());

  renderer_->setSize(w, h);

  startDraw();

  CPoint2D bmin, bmax;

  svg_.viewMatrix().multiplyPoint(bbox.getLL(), bmin);
  svg_.viewMatrix().multiplyPoint(bbox.getUR(), bmax);

  //renderer_->setDataRange(bbox.getXMin(), bbox.getYMin(), bbox.getXMax(), bbox.getYMax());
  renderer_->setDataRange(bmin.x, bmin.y, bmax.x, bmax.y);
}

void
CSVGBuffer::
endDraw()
{
  if (refBuffer_)
    return refBuffer_->endDraw();

  //---

  stopDraw();
}

void
CSVGBuffer::
startDraw()
{
  if (refBuffer_)
    return refBuffer_->startDraw();

  //---

  assert(! drawing_);

  drawing_ = true;

  renderer_->beginDraw();
}

void
CSVGBuffer::
stopDraw()
{
  if (refBuffer_)
    return refBuffer_->stopDraw();

  //---

  assert(drawing_);

  renderer_->endDraw();

  drawing_ = false;
}

void
CSVGBuffer::
setAlign(CHAlignType halign, CVAlignType valign)
{
  if (refBuffer_)
    return refBuffer_->setAlign(halign, valign);

  //---

  renderer_->setAlign(halign, valign);
}

void
CSVGBuffer::
setEqualScale(bool equalScale)
{
  if (refBuffer_)
    return refBuffer_->setEqualScale(equalScale);

  //---

  renderer_->setEqualScale(equalScale);
}

void
CSVGBuffer::
setScaleMin(bool scale)
{
  if (refBuffer_)
    return refBuffer_->setScaleMin(scale);

  //---

  renderer_->setScaleMin(scale);
}

void
CSVGBuffer::
fill(const CRGBA &bg)
{
  if (refBuffer_)
    return refBuffer_->fill(bg);

  //---

  renderer_->clear(bg);
}

//-------

void
CSVGBuffer::
setTransform(const CMatrixStack2D &matrix)
{
  if (refBuffer_)
    return refBuffer_->setTransform(matrix);

  //---

  transform_ = matrix;

  CMatrixStack2D matrix1(svg_.viewMatrix());

  matrix1.append(transform_);

  setViewMatrix(matrix1);
}

void
CSVGBuffer::
unsetTransform()
{
  if (refBuffer_)
    return refBuffer_->unsetTransform();

  //---

  transform_.reset();

  setViewMatrix(svg_.viewMatrix());
}

void
CSVGBuffer::
setViewMatrix(const CMatrixStack2D &matrix)
{
  if (refBuffer_)
    return refBuffer_->setViewMatrix(matrix);

  //---

  renderer_->setViewMatrix(matrix.getMatrix());
}

void
CSVGBuffer::
setStrokeColor(const CRGBA &color)
{
  if (refBuffer_)
    return refBuffer_->setStrokeColor(color);

  //---

  renderer_->setStrokeColor(color);
}

void
CSVGBuffer::
setLineWidth(double width)
{
  if (refBuffer_)
    return refBuffer_->setLineWidth(width);

  //---

  renderer_->setLineWidth(width);
}

void
CSVGBuffer::
setLineDash(const CLineDash &dash)
{
  if (refBuffer_)
    return refBuffer_->setLineDash(dash);

  //---

  lineDash_ = dash;

  renderer_->setLineDash(lineDash_);
}

void
CSVGBuffer::
setLineDashOffset(double offset)
{
  if (refBuffer_)
    return refBuffer_->setLineDashOffset(offset);

  //---

  lineDash_.setOffset(offset);

  renderer_->setLineDash(lineDash_);
}

void
CSVGBuffer::
setLineCap(CLineCapType line_cap)
{
  if (refBuffer_)
    return refBuffer_->setLineCap(line_cap);

  //---

  renderer_->setLineCap(line_cap);
}

void
CSVGBuffer::
setLineJoin(CLineJoinType line_join)
{
  if (refBuffer_)
    return refBuffer_->setLineJoin(line_join);

  //---

  renderer_->setLineJoin(line_join);
}

void
CSVGBuffer::
setLineMitreLimit(double limit)
{
  if (refBuffer_)
    return refBuffer_->setLineMitreLimit(limit);

  //---

  renderer_->setMitreLimit(limit);
}

void
CSVGBuffer::
setFillType(CFillType type)
{
  if (refBuffer_)
    return refBuffer_->setFillType(type);

  //---

  renderer_->setFillType(type);
}

void
CSVGBuffer::
setFillColor(const CRGBA &color)
{
  if (refBuffer_)
    return refBuffer_->setFillColor(color);

  //---

  renderer_->setFillColor(color);
}

void
CSVGBuffer::
setFillGradient(CGenGradient *g)
{
  if (refBuffer_)
    return refBuffer_->setFillGradient(g);

  //---

  renderer_->setFillGradient(g);
}

void
CSVGBuffer::
setFillImage(CImagePtr image)
{
  if (refBuffer_)
    return refBuffer_->setFillImage(image);

  //---

  renderer_->setFillImage(image);
}

void
CSVGBuffer::
drawImage(double x, double y, CImagePtr image)
{
  if (refBuffer_)
    return refBuffer_->drawImage(x, y, image);

  //---

  renderer_->drawImage(CPoint2D(x, y), image);
}

void
CSVGBuffer::
drawImage(const CBBox2D &bbox, CImagePtr image)
{
  if (refBuffer_)
    return refBuffer_->drawImage(bbox, image);

  //---

  CPoint2D p1, p2;

  renderer_->windowToPixel(bbox.getLL(), p1);
  renderer_->windowToPixel(bbox.getUR(), p2);

  int pw = CSVGUtil::round(fabs(p2.x - p1.x));
  int ph = CSVGUtil::round(fabs(p2.y - p1.y));

  image->reshape(pw, ph);

  //double x = std::min(p1.x, p2.x);
  //double y = std::min(p1.y, p2.y);

  // draw image resize to bbox
  renderer_->drawImage(bbox.getLL(), image);
}

void
CSVGBuffer::
pathText(const std::string &text, CFontPtr font, CHAlignType align)
{
  if (refBuffer_)
    return refBuffer_->pathText(text, font, align);

  //---

  renderer_->setFont(font);

  CBBox2D box;

  renderer_->textBounds(text, box);

  double dx = 0;

  if      (align == CHALIGN_TYPE_LEFT)
    dx = 0;
  else if (align == CHALIGN_TYPE_CENTER)
    dx = -box.getWidth()/2;
  else if (align == CHALIGN_TYPE_RIGHT)
    dx = -box.getWidth();

  renderer_->pathRMoveTo(CPoint2D(dx, 0));

  renderer_->pathText(text);
}

void
CSVGBuffer::
pathInit()
{
  if (refBuffer_)
    return refBuffer_->pathInit();

  //---

  renderer_->pathInit();
}

void
CSVGBuffer::
pathMoveTo(double x, double y)
{
  if (refBuffer_)
    return refBuffer_->pathMoveTo(x, y);

  //---

  renderer_->pathMoveTo(CPoint2D(x, y));
}

void
CSVGBuffer::
pathLineTo(double x, double y)
{
  if (refBuffer_)
    return refBuffer_->pathLineTo(x, y);

  //---

  renderer_->pathLineTo(CPoint2D(x, y));
}

void
CSVGBuffer::
pathRLineTo(double dx, double dy)
{
  if (refBuffer_)
    return refBuffer_->pathRLineTo(dx, dy);

  //---

  renderer_->pathRLineTo(CPoint2D(dx, dy));
}

void
CSVGBuffer::
pathArcTo(double cx, double cy, double rx, double ry, double theta1, double theta2)
{
  if (refBuffer_)
    return refBuffer_->pathArcTo(cx, cy, rx, ry, theta1, theta2);

  //---

  renderer_->pathArcTo(CPoint2D(cx, cy), rx, ry, theta1, theta2);
}

void
CSVGBuffer::
pathBezier2To(double x1, double y1, double x2, double y2)
{
  if (refBuffer_)
    return refBuffer_->pathBezier2To(x1, y1, x2, y2);

  //---

  renderer_->pathCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2));
}

void
CSVGBuffer::
pathRBezier2To(double x1, double y1, double x2, double y2)
{
  if (refBuffer_)
    return refBuffer_->pathRBezier2To(x1, y1, x2, y2);

  //---

  renderer_->pathRCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2));
}

void
CSVGBuffer::
pathBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  if (refBuffer_)
    return refBuffer_->pathBezier3To(x1, y1, x2, y2, x3, y3);

  //---

  renderer_->pathCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2), CPoint2D(x3, y3));
}

void
CSVGBuffer::
pathRBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  if (refBuffer_)
    return refBuffer_->pathRBezier3To(x1, y1, x2, y2, x3, y3);

  //---

  renderer_->pathRCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2), CPoint2D(x3, y3));
}

void
CSVGBuffer::
pathClose()
{
  if (refBuffer_)
    return refBuffer_->pathClose();

  //---

  renderer_->pathClose();
}

bool
CSVGBuffer::
pathGetCurrentPoint(double *x, double *y)
{
  if (refBuffer_)
    return refBuffer_->pathGetCurrentPoint(x, y);

  //---

  *x = 0;
  *y = 0;

  CPoint2D p;

  if (! renderer_->pathGetCurrentPoint(p))
    return false;

  *x = p.x;
  *y = p.y;

  return true;
}

void
CSVGBuffer::
pathStroke()
{
  if (refBuffer_)
    return refBuffer_->pathStroke();

  //---

  renderer_->pathStroke();
}

void
CSVGBuffer::
pathFill()
{
  if (refBuffer_)
    return refBuffer_->pathFill();

  //---

  renderer_->pathFill();
}

void
CSVGBuffer::
pathClip(CSVGBuffer *buffer)
{
  if (refBuffer_)
    return refBuffer_->pathClip(buffer);

  //---

  renderer_->pathClip(buffer ? buffer->renderer_ : 0);
}

void
CSVGBuffer::
pathEoClip(CSVGBuffer *buffer)
{
  if (refBuffer_)
    return refBuffer_->pathEoClip(buffer);

  //---

  renderer_->pathEoclip(buffer ? buffer->renderer_ : 0);
}

void
CSVGBuffer::
pathBBox(CBBox2D &bbox)
{
  if (refBuffer_)
    return refBuffer_->pathBBox(bbox);

  //---

  renderer_->pathBBox(bbox);
}

void
CSVGBuffer::
initClip()
{
  if (refBuffer_)
    return refBuffer_->initClip();

  //---

  renderer_->initClip();
}

bool
CSVGBuffer::
mmToPixel(double mm, double *pixel)
{
  if (refBuffer_)
    return refBuffer_->mmToPixel(mm, pixel);

  //---

  //return renderer_->mmToPixel(mm, pixel);

  // 75 dpi
  *pixel = 75.0*mm/25.4;

  return true;
}

void
CSVGBuffer::
lengthToPixel(double xi, double yi, double *xo, double *yo)
{
  if (refBuffer_)
    return refBuffer_->lengthToPixel(xi, yi, xo, yo);

  //---

  double px1, py1, px2, py2;

  windowToPixel( 0,  0, &px1, &py1);
  windowToPixel(xi, yi, &px2, &py2);

  *xo = fabs(px2 - px1);
  *yo = fabs(py2 - py1);
}

void
CSVGBuffer::
windowToPixel(double xi, double yi, double *xo, double *yo)
{
  if (refBuffer_)
    return refBuffer_->windowToPixel(xi, yi, xo, yo);

  //---

  CPoint2D p;

  renderer_->windowToPixel(CPoint2D(xi, yi), p);

  *xo = p.x;
  *yo = p.y;
}
