#include <CSVGI.h>

CSVGBufferMgr::
CSVGBufferMgr(CSVG &svg) :
 svg_(svg), anti_alias_(true), buffer_map_()
{
}

CSVGBufferMgr::
~CSVGBufferMgr()
{
  BufferMap::iterator p1 = buffer_map_.begin();
  BufferMap::iterator p2 = buffer_map_.end  ();

  for ( ; p1 != p2; ++p1)
    delete (*p1).second;
}

CSVGBuffer *
CSVGBufferMgr::
lookupBuffer(const std::string &name)
{
  BufferMap::iterator p = buffer_map_.find(name);

  if (p != buffer_map_.end())
    return (*p).second;

  return 0;
}

CSVGBuffer *
CSVGBufferMgr::
createBuffer(const std::string &name)
{
  CSVGBuffer *buffer = new CSVGBuffer(svg_, name);

  buffer->setAntiAlias(anti_alias_);

  addBuffer(buffer);

  return buffer;
}

void
CSVGBufferMgr::
addBuffer(CSVGBuffer *buffer)
{
  buffer_map_[buffer->getName()] = buffer;
}

//------------

CSVGBuffer::
CSVGBuffer(CSVG &svg, const std::string &name) :
 svg_(svg), name_(name), renderer_(0), lineDash_()
{
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
  if (! renderer_) {
    CSVGBuffer *th = const_cast<CSVGBuffer *>(this);

    th->renderer_ = svg_.createRenderer();
  }

  return renderer_;
}

void
CSVGBuffer::
setAntiAlias(bool flag)
{
  getRenderer()->setAntiAlias(flag);
}

CImagePtr
CSVGBuffer::
getImage() const
{
  return getRenderer()->getImage();
}

void
CSVGBuffer::
setImage(CImagePtr image)
{
  getRenderer()->setImage(image);
}

void
CSVGBuffer::
reset()
{
  CSVGRenderer *renderer = getRenderer();

  renderer->setDataRange(0, 0, 1, 1);

  renderer->setViewMatrix(CMatrix2D(CMATRIX_TYPE_IDENTITY));

  //------

  CImagePtr image = renderer->getImage();

  if (image.isValid())
    image->setRGBAData(CRGBA(0,0,0,0));

  //------

  renderer->setStrokeColor(CRGBA(0,0,0,1));
  renderer->setFillColor  (CRGBA(0,0,0,1));
}

void
CSVGBuffer::
setup(const CBBox2D &bbox)
{
  reset();

  CSVGRenderer *renderer = getRenderer();

  double x1 = bbox.getXMin();
  double y1 = bbox.getYMin();
  double x2 = bbox.getXMax();
  double y2 = bbox.getYMax();

  renderer->setSize(x2 - x1, y2 - y1);

  renderer->setDataRange(x1, y2, x2, x1);
}

void
CSVGBuffer::
beginDraw(int w, int h, const CBBox2D &bbox)
{
  renderer_->setSize(w, h);

  renderer_->beginDraw();

  renderer_->setDataRange(bbox.getXMin(), bbox.getYMin(), bbox.getXMax(), bbox.getYMax());
}

void
CSVGBuffer::
endDraw()
{
  renderer_->endDraw();
}

void
CSVGBuffer::
setAlign(CHAlignType halign, CVAlignType valign)
{
  renderer_->setAlign(halign, valign);
}

void
CSVGBuffer::
setEqualScale(bool equalScale)
{
  renderer_->setEqualScale(equalScale);
}

void
CSVGBuffer::
setScaleMin(bool scale)
{
  renderer_->setScaleMin(scale);
}

void
CSVGBuffer::
fill(const CRGBA &bg)
{
  renderer_->clear(bg);
}

//-------

void
CSVGBuffer::
setViewMatrix(const CMatrix2D &matrix)
{
  renderer_->setViewMatrix(matrix);
}

void
CSVGBuffer::
setStrokeColor(const CRGBA &color)
{
  renderer_->setStrokeColor(color);
}

void
CSVGBuffer::
setLineWidth(double width)
{
  renderer_->setLineWidth(width);
}

void
CSVGBuffer::
setLineDash(const CLineDash &dash)
{
  lineDash_ = dash;

  renderer_->setLineDash(lineDash_);
}

void
CSVGBuffer::
setLineDashOffset(double offset)
{
  lineDash_.setOffset(offset);

  renderer_->setLineDash(lineDash_);
}

void
CSVGBuffer::
setLineCap(CLineCapType line_cap)
{
  renderer_->setLineCap(line_cap);
}

void
CSVGBuffer::
setLineJoin(CLineJoinType line_join)
{
  renderer_->setLineJoin(line_join);
}

void
CSVGBuffer::
setLineMitreLimit(double limit)
{
  renderer_->setMitreLimit(limit);
}

void
CSVGBuffer::
setFillType(CFillType type)
{
  renderer_->setFillType(type);
}

void
CSVGBuffer::
setFillColor(const CRGBA &color)
{
  renderer_->setFillColor(color);
}

void
CSVGBuffer::
setFillGradient(CGenGradient *g)
{
  renderer_->setFillGradient(g);
}

void
CSVGBuffer::
setFillImage(CImagePtr image)
{
  renderer_->setFillImage(image);
}

void
CSVGBuffer::
drawImage(double x, double y, CImagePtr image)
{
  renderer_->drawImage(CPoint2D(x, y), image);
}

void
CSVGBuffer::
drawImage(const CBBox2D &bbox, CImagePtr image)
{
  renderer_->drawImage(bbox.getLL(), image);
}

void
CSVGBuffer::
pathText(const std::string &text, CFontPtr font, CHAlignType align)
{
  renderer_->setFont(font);

  CBBox2D box;

  renderer_->textBounds(text, box);

  int dx = 0;

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
  renderer_->pathInit();
}

void
CSVGBuffer::
pathMoveTo(double x, double y)
{
  renderer_->pathMoveTo(CPoint2D(x, y));
}

void
CSVGBuffer::
pathLineTo(double x, double y)
{
  renderer_->pathLineTo(CPoint2D(x, y));
}

void
CSVGBuffer::
pathRLineTo(double dx, double dy)
{
  renderer_->pathRLineTo(CPoint2D(dx, dy));
}

void
CSVGBuffer::
pathArcTo(double cx, double cy, double rx, double ry, double theta1, double theta2)
{
  renderer_->pathArcTo(CPoint2D(cx, cy), rx, ry, theta1, theta2);
}

void
CSVGBuffer::
pathBezier2To(double x1, double y1, double x2, double y2)
{
  renderer_->pathCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2));
}

void
CSVGBuffer::
pathRBezier2To(double x1, double y1, double x2, double y2)
{
  renderer_->pathRCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2));
}

void
CSVGBuffer::
pathBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  renderer_->pathCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2), CPoint2D(x3, y3));
}

void
CSVGBuffer::
pathRBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  renderer_->pathRCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2), CPoint2D(x3, y3));
}

void
CSVGBuffer::
pathClose()
{
  renderer_->pathClose();
}

bool
CSVGBuffer::
pathGetCurrentPoint(double *x, double *y)
{
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
  renderer_->pathStroke();
}

void
CSVGBuffer::
pathFill()
{
  renderer_->pathFill();
}

void
CSVGBuffer::
pathClip()
{
  renderer_->pathClip();
}

void
CSVGBuffer::
pathEoClip()
{
  renderer_->pathEoclip();
}

void
CSVGBuffer::
pathBBox(CBBox2D &bbox)
{
  renderer_->pathBBox(bbox);
}

void
CSVGBuffer::
initClip()
{
  renderer_->initClip();
}

bool
CSVGBuffer::
mmToPixel(double mm, double *pixel)
{
  //return renderer_->mmToPixel(mm, pixel);

  // 75 dpi
  *pixel = 75.0*mm/25.4;

  return true;
}

void
CSVGBuffer::
windowToPixel(double xi, double yi, int *xo, int *yo)
{
  CPoint2D p;

  renderer_->windowToPixel(CPoint2D(xi, yi), p);

  *xo = p.x;
  *yo = p.y;
}
