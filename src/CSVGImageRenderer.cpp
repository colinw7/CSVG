#include <CSVGImageRenderer.h>

#include <CSVGImageData.h>
#include <CSVGFontObj.h>
#include <CSVGFontDef.h>
#include <CImageRenderer2D.h>
#include <CImageLib.h>

CSVGImageRenderer::
CSVGImageRenderer(int w, int h) :
 w_(w), h_(h)
{
  CImageNoSrc src;

  image_ = CImageMgrInst->createImage(src);

  image_->setDataSize(w_, h_);

  renderer_ = new CImageRenderer2D(image_);
}

CSVGImageRenderer::
~CSVGImageRenderer()
{
  //delete renderer_;
}

CSVGImageRenderer *
CSVGImageRenderer::
dup() const
{
  return new CSVGImageRenderer(w_, h_);
}

void
CSVGImageRenderer::
setSize(int width, int height)
{
  w_ = width;
  h_ = height;

  image_->setDataSize(w_, h_);

  renderer_->updateSize(width, height);
}

void
CSVGImageRenderer::
getSize(int *width, int *height) const
{
  *width  = w_;
  *height = h_;
}

void
CSVGImageRenderer::
setPixelRange(int w, int h)
{
  renderer_->setPixelRange(0, 0, w - 1, h - 1);
}

void
CSVGImageRenderer::
setDataRange(double xmin, double ymin, double xmax, double ymax)
{
  renderer_->setDataRange(xmin, ymin, xmax, ymax);
}

void
CSVGImageRenderer::
getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const
{
  renderer_->getDataRange(xmin, ymin, xmax, ymax);
}

bool
CSVGImageRenderer::
isAntiAlias() const
{
  return renderer_->isAntiAlias();
}

void
CSVGImageRenderer::
setAntiAlias(bool b)
{
  renderer_->setAntiAlias(b);
}

void
CSVGImageRenderer::
setEqualScale(bool b)
{
  renderer_->setEqualScale(b);
}

void
CSVGImageRenderer::
setScaleMin(bool b)
{
  renderer_->setScaleMin(b);
}

void
CSVGImageRenderer::
beginDraw()
{
  renderer_->beginDraw();
}

void
CSVGImageRenderer::
endDraw()
{
  renderer_->endDraw();
}

void
CSVGImageRenderer::
setViewMatrix(const CMatrix2D &m)
{
  renderer_->setViewMatrix(m);
}

void
CSVGImageRenderer::
setBackground(const CRGBA &)
{
}

void
CSVGImageRenderer::
clear(const CRGBA &c)
{
  renderer_->setBackground(c); renderer_->clear();
}

void
CSVGImageRenderer::
pathInit()
{
  renderer_->pathInit();
}

void
CSVGImageRenderer::
pathMoveTo(const CPoint2D &p)
{
  renderer_->pathMoveTo(p);
}

void
CSVGImageRenderer::
pathRMoveTo(const CPoint2D &p)
{
  renderer_->pathRMoveTo(p);
}

void
CSVGImageRenderer::
pathLineTo(const CPoint2D &p)
{
  renderer_->pathLineTo(p);
}

void
CSVGImageRenderer::
pathRLineTo(const CPoint2D &p)
{
  renderer_->pathRLineTo(p);
}

void
CSVGImageRenderer::
pathCurveTo(const CPoint2D &p1, const CPoint2D &p2)
{
  renderer_->pathBezier2To(p1, p2);
}

void
CSVGImageRenderer::
pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2)
{
  renderer_->pathRBezier2To(p1, p2);
}

void
CSVGImageRenderer::
pathCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  renderer_->pathBezier3To(p1, p2, p3);
}

void
CSVGImageRenderer::
pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3)
{
  renderer_->pathRBezier3To(p1, p2, p3);
}

void
CSVGImageRenderer::
pathArcTo(const CPoint2D &c, double rx, double ry, double a1, double a2)
{
  renderer_->pathArc(c, rx, ry, a1, a2);
}

void
CSVGImageRenderer::
pathText(const std::string &)
{
  std::cerr << "UNSUPPORTED: pathText\n";
}

void
CSVGImageRenderer::
pathClose()
{
  renderer_->pathClose();
}

bool
CSVGImageRenderer::
pathGetCurrentPoint(CPoint2D &p)
{
  return renderer_->pathGetCurrentPoint(p);
}

void
CSVGImageRenderer::
pathStroke()
{
  renderer_->pathStroke();
}

void
CSVGImageRenderer::
pathFill()
{
  renderer_->pathFill();
}

void
CSVGImageRenderer::
savePath(const CMatrix2D &)
{
  std::cerr << "UNSUPPORTED: savePath\n";
}

void
CSVGImageRenderer::
pathClip(CSVGRenderer *)
{
  renderer_->pathClip();
}

void
CSVGImageRenderer::
pathEoclip(CSVGRenderer *)
{
  renderer_->pathEoclip();
}

void
CSVGImageRenderer::
initClip()
{
  renderer_->initClip();
}

void
CSVGImageRenderer::
addClipPath(CSVGRenderer *)
{
  std::cerr << "UNSUPPORTED: addClipPath\n";
}

void
CSVGImageRenderer::
pathBBox(CBBox2D &bbox)
{
  renderer_->pathBBox(bbox);
}

void
CSVGImageRenderer::
drawImage(const CPoint2D &p, CSVGImageData *data)
{
  renderer_->drawImage(p, data->image());
}

void
CSVGImageRenderer::
setFont(const CSVGFontDef &fontDef)
{
  CSVGFontObj *fontObj = fontDef.getObj();

  CFontPtr font = fontObj->font();

  renderer_->setFont(font);
}

void
CSVGImageRenderer::
resetStroke()
{
}

void
CSVGImageRenderer::
setStrokeColor(const CRGBA &c)
{
  renderer_->setForeground(c);
}

void
CSVGImageRenderer::
setLineWidth(double w)
{
  renderer_->setLineWidth(w);
}

void
CSVGImageRenderer::
setLineDash(const CLineDash &d)
{
  renderer_->setLineDash(d);
}

void
CSVGImageRenderer::
setLineCap(const CLineCapType &c)
{
  renderer_->setLineCap(c);
}

void
CSVGImageRenderer::
setLineJoin(const CLineJoinType &j)
{
  renderer_->setLineJoin(j);
}

void
CSVGImageRenderer::
setMitreLimit(double l)
{
  renderer_->setMitreLimit(l);
}

void
CSVGImageRenderer::
resetFill()
{
}

void
CSVGImageRenderer::
setFillType(CFillType t)
{
  renderer_->setFillType(t);
}

void
CSVGImageRenderer::
setFillColor(const CRGBA &c)
{
  renderer_->setForeground(c);
}

void
CSVGImageRenderer::
setFillGradient(CGenGradient *)
{
}

void
CSVGImageRenderer::
setFillImage(double, double, CSVGImageData *)
{
}

void
CSVGImageRenderer::
setFillMatrix(const CMatrix2D &)
{
  std::cerr << "UNSUPPORTED: setFillMatrix\n";
}

void
CSVGImageRenderer::
setStrokeFilled(bool)
{
}

void
CSVGImageRenderer::
setStrokeFillType(CFillType)
{
}

void
CSVGImageRenderer::
setStrokeFillGradient(CGenGradient *)
{
}

void
CSVGImageRenderer::
setStrokeFillImage(CSVGImageData *)
{
}

void
CSVGImageRenderer::
setAlign(CHAlignType halign, CVAlignType valign)
{
  renderer_->setAlign((CDisplayRange2D::HAlign) halign, (CDisplayRange2D::VAlign) valign);
}

void
CSVGImageRenderer::
windowToPixel(const CPoint2D &w, CPoint2D &p)
{
  renderer_->windowToPixel(w, p);
}

void
CSVGImageRenderer::
pixelToWindow(const CPoint2D &p, CPoint2D &w)
{
  renderer_->pixelToWindow(p, w);
}

void
CSVGImageRenderer::
textBounds(const std::string &text, CBBox2D &bbox)
{
  renderer_->textBounds(text, bbox);
}

CISize2D
CSVGImageRenderer::
getImageSize() const
{
  return CISize2D(renderer_->getPixelWidth(), renderer_->getPixelHeight());
}

CSVGImageData *
CSVGImageRenderer::
getImage() const
{
  return nullptr;
}

void
CSVGImageRenderer::
setImage(CSVGImageData *data)
{
  return renderer_->setImage(data->image());
}

void
CSVGImageRenderer::
setImage(CSVGRenderer *renderer)
{
  auto *srenderer = dynamic_cast<CSVGImageRenderer *>(renderer);

  if (srenderer)
    renderer_->setImage(srenderer->renderer_);
  else
    renderer_->setImage(renderer->getImage()->image());
}
