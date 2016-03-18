#include <CPixelRenderer.h>
#include <CFreeTypePixelRenderer.h>
#include <COptVal.h>
#include <CPolygon2D.h>
#include <CImageLib.h>
#include <CImagePixelRenderer.h>
#include <CFreeType.h>

#ifndef ROUND
# define ROUND(x) ((int)((x) + 0.5))
#endif

class GenGradientPixelFill : public CPixelRendererFiller {
 private:
   CRefPtr<CGenGradient> gradient_;
   int                   x1_, y1_, x2_, y2_;

 public:
  GenGradientPixelFill(CRefPtr<CGenGradient> gradient, int x1, int y1, int x2, int y2) :
   gradient_(gradient), x1_(x1), y1_(y1), x2_(x2), y2_(y2) {
    gradient_->init(1, 1);
  }

  void getColor(const CIPoint2D &point, CRGBA &rgba) const {
    double xt = double(point.x - x1_)/(x2_ - x1_);
    double yt = double(point.y - y1_)/(y2_ - y1_);

    rgba = gradient_->getColor(xt, yt);
  }
};

//--------------

CPixelRenderer::
CPixelRenderer() :
 vsize_(0,0), bg_(0,0,0), contentsChanged_(true)
{
  pen_.color_ = CRGBA(1,1,1);
  pen_.width_ = 0;
}

CPixelRenderer::
CPixelRenderer(const CPixelRenderer &renderer) :
 clip_           (renderer.clip_),
 offset_         (renderer.offset_),
 vsize_          (renderer.vsize_),
 bg_             (renderer.bg_),
 bg_image_       (renderer.bg_image_),
 font_           (renderer.font_),
 contentsChanged_(renderer.contentsChanged_),
 pen_            (renderer.pen_)
{
}

CPixelRenderer::
~CPixelRenderer()
{
}

void
CPixelRenderer::
setOffset(const CIPoint2D &offset)
{
  offset_ = offset;
}

void
CPixelRenderer::
setVSize(const CISize2D &vsize)
{
  vsize_ = vsize;
}

void
CPixelRenderer::
setContentsChanged(bool flag)
{
  contentsChanged_ = flag;
}

void
CPixelRenderer::
applyOffset(CIPoint2D &point) const
{
  point.x += offset_.x;
  point.y += offset_.y;
}

void
CPixelRenderer::
unapplyOffset(CIPoint2D &point) const
{
  point.x -= offset_.x;
  point.y -= offset_.y;
}

void
CPixelRenderer::
setForeground(const CRGBA &fg)
{
  pen_.color_ = fg;

  updatePen();
}

void
CPixelRenderer::
setBackground(const CRGBA &bg)
{
  bg_ = bg;
}

void
CPixelRenderer::
setBackgroundImage(CImagePtr image)
{
  bg_image_ = image;
}

void
CPixelRenderer::
setFont(CFontPtr font)
{
  font_ = font;
}

void
CPixelRenderer::
setLineWidth(uint width)
{
  pen_.width_ = width;

  updatePen();
}

void
CPixelRenderer::
setLineDash(const CILineDash &dash)
{
  pen_.dash_ = dash;

  updatePen();
}

void
CPixelRenderer::
setLineCap(CLineCapType cap)
{
  pen_.cap_ = cap;

  updatePen();
}

void
CPixelRenderer::
setLineJoin(CLineJoinType join)
{
  pen_.join_ = join;

  updatePen();
}

void
CPixelRenderer::
clear()
{
  CRGBA fg, bg;

  getForeground(fg);
  getBackground(bg);

  setForeground(bg);

  fill();

  setForeground(fg);
}

void
CPixelRenderer::
fill()
{
  int iwidth  = getWidth ();
  int iheight = getHeight();

  for (int y = 0; y < iheight; ++y)
    drawClippedHLine(0, iwidth - 1, y);
}

void
CPixelRenderer::
drawPoint(const CIPoint2D &point)
{
  CIPoint2D point1(point);

  applyOffset(point1);

  int iwidth  = getWidth ();
  int iheight = getHeight();

  if (point1.x >= 0 && point1.x < iwidth && point1.y >= 0 && point1.y < iheight)
    drawClippedPoint(point1);
}

void
CPixelRenderer::
drawSymbol(const CIPoint2D &point, CSymbolType symbol)
{
  int size = getSymbolSize();

  const CSymbol2D &psymbol = CSymbol2DMgr::getSymbol(symbol);

  CPoint2D p(point);

  for (uint i = 0; i < psymbol.lines.size(); ++i) {
    const CSymbol2D::Line &line = psymbol.lines[i];

    CPoint2D start(line.x1, line.y1);
    CPoint2D end  (line.x2, line.y2);

    CPoint2D p1 = p + start*size;
    CPoint2D p2 = p + end  *size;

    drawLine(p1.toIPoint(), p2.toIPoint());
  }
}

uint
CPixelRenderer::
getSymbolSize() const
{
  return 4;
}

void
CPixelRenderer::
drawChar(const CIPoint2D &point, char c)
{
  CIPoint2D point1(point);

  applyOffset(point1);

  drawClippedChar(point, c);
}

void
CPixelRenderer::
drawClippedChar(const CIPoint2D &point, char c)
{
  CFreeTypeMgrInst->setFont(font_);

  double x = point.x;
  double y = point.y;

  CFreeTypePixelRenderer renderer(this);

  CFreeTypeMgrInst->drawChar(&renderer, c, &x, &y);
}

#if 0
void
CPixelRenderer::
drawCharToRenderer(CPixelRenderer *renderer, const CIPoint2D &point, char c)
{
  drawClippedCharToRenderer(renderer, point, c);
}

void
CPixelRenderer::
drawClippedCharToRenderer(CPixelRenderer *, const CIPoint2D &, char)
{
  assert(0);
}
#endif

void
CPixelRenderer::
drawString(const CIPoint2D &point, const std::string &str)
{
  CIPoint2D point1(point);

  applyOffset(point1);

  drawClippedString(point1, str);
}

void
CPixelRenderer::
drawClippedString(const CIPoint2D &point, const std::string &str)
{
  CFreeTypeMgrInst->setFont(font_);

  double x = point.x;
  double y = point.y;

  CFreeTypePixelRenderer renderer(this);

  CFreeTypeMgrInst->drawString(&renderer, str.c_str(), &x, &y);
}

void
CPixelRenderer::
drawStringInRect(const CIBBox2D &bbox, const std::string &str)
{
  int w = getStringWidth(str);
  int a = getCharAscent();
  int d = getCharDescent();

  int xm = (bbox.getXMin() + bbox.getXMax())/2;
  int ym = (bbox.getYMin() + bbox.getYMax())/2;

  int x = xm - w/2;
  int y = ym - (a + d)/2 + d;

  drawString(CIPoint2D(x, y), str);
}

void
CPixelRenderer::
drawLine(const CIPoint2D &point1, const CIPoint2D &point2)
{
  CIPoint2D p1(point1);
  CIPoint2D p2(point2);

  applyOffset(p1);
  applyOffset(p2);

  if (! clip_.isEmpty()) {
    uint  num;
    int  *xx1, *yy1, *xx2, *yy2;

    clip_.processLine(p1.x, p1.y, p2.x, p2.y, &xx1, &yy1, &xx2, &yy2, &num);

    for (uint i = 0; i < num; ++i)
      drawLine1(CIPoint2D(xx1[i], yy1[i]), CIPoint2D(xx2[i], yy2[i]));
  }
  else
    drawLine1(p1, p2);
}

void
CPixelRenderer::
drawFilledLine(const CIPoint2D &point1, const CIPoint2D &point2,
               const CPixelRendererFiller &filler)
{
  if (! clip_.isEmpty()) {
    uint  num;
    int  *xx1, *yy1, *xx2, *yy2;

    clip_.processLine(point1.x, point1.y, point2.x, point2.y, &xx1, &yy1, &xx2, &yy2, &num);

    for (uint i = 0; i < num; ++i)
      drawFilledLine1(CIPoint2D(xx1[i], yy1[i]), CIPoint2D(xx2[i], yy2[i]), filler);
  }
  else
    drawFilledLine1(point1, point2, filler);
}

void
CPixelRenderer::
drawLine1(const CIPoint2D &point1, const CIPoint2D &point2)
{
  int iwidth  = getWidth ();
  int iheight = getHeight();

  CBBox2D bbox(0, 0, iwidth - 1, iheight - 1);

  CLine2D line(point1, point2);

  CLine2D line1;

  if (line.clip(bbox, line1))
    drawClippedLine(line1.start().toIPoint(), line1.end().toIPoint());
}

void
CPixelRenderer::
drawFilledLine1(const CIPoint2D &point1, const CIPoint2D &point2,
                const CPixelRendererFiller &filler)
{
  int iwidth  = getWidth ();
  int iheight = getHeight();

  CBBox2D bbox(0, 0, iwidth - 1, iheight - 1);

  CLine2D line(point1, point2);

  CLine2D line1;

  if (line.clip(bbox, line1))
    drawFilledClippedLine(line1.start().toIPoint(), line1.end().toIPoint(), filler);
}

void
CPixelRenderer::
drawClippedLine(const CIPoint2D &point1, const CIPoint2D &point2)
{
  CPixelRendererBresenham bresenham(this);

  bresenham.drawLine(point1.x, point1.y, point2.x, point2.y);
}

void
CPixelRenderer::
drawFilledClippedLine(const CIPoint2D &point1, const CIPoint2D &point2,
                      const CPixelRendererFiller &filler)
{
  class BresenhamRenderer : public CBresenham {
   private:
    CPixelRenderer             *renderer_;
    const CPixelRendererFiller &filler_;

   public:
    BresenhamRenderer(CPixelRenderer *renderer, const CPixelRendererFiller &filler) :
     renderer_(renderer), filler_(filler) {
    }

    void drawPoint(int x, int y) {
      CRGBA rgba;

      filler_.getColor(CIPoint2D(x, y), rgba);

      renderer_->setForeground(rgba);

      renderer_->drawClippedPoint(CIPoint2D(x, y));
    }

    const CILineDash &getLineDash() const {
      return renderer_->getLineDash();
    }
  };

  BresenhamRenderer bresenham(this, filler);

  bresenham.drawLine(point1.x, point1.y, point2.x, point2.y);
}

void
CPixelRenderer::
drawAALine(const CIPoint2D &point1, const CIPoint2D &point2)
{
  double xp[4], yp[4];

  lineToPolygon(point1, point2, pen_.width_, xp, yp);

  fillAAPolygon(xp, yp, 4);
}

void
CPixelRenderer::
lineToPolygon(const CIPoint2D &point1, const CIPoint2D &point2,
              double width, double *xp, double *yp)
{
  // Convert Line into Polygon

  double dx = point2.x - point1.x;
  double dy = point2.y - point1.y;

  if (dx <= 0 && dy <= 0)
    return;

  double l = sqrt(dx*dx + dy*dy);

  double s = dy/l;
  double c = dx/l;

  double width2 = width*0.5;

  double dxe = s*width2;
  double dye = c*width2;

  xp[0] = point1.x + dxe; yp[0] = point1.y - dye;
  xp[1] = point2.x + dxe; yp[1] = point2.y - dye;
  xp[2] = point2.x - dxe; yp[2] = point2.y + dye;
  xp[3] = point1.x - dxe; yp[3] = point1.y + dye;
}

void
CPixelRenderer::
drawRectangle(const CIBBox2D &bbox)
{
  CIPoint2D point1(bbox.getXMin(), bbox.getYMin());
  CIPoint2D point2(bbox.getXMax(), bbox.getYMax());

  applyOffset(point1);
  applyOffset(point2);

  if (! clip_.isEmpty()) {
    CIPoint2D p1(point2.x, point1.y);
    CIPoint2D p2(point1.x, point2.y);

    drawLine(point1, p1);
    drawLine(p1    , point2);
    drawLine(point2, p2);
    drawLine(p2    , point1);
  }
  else
    drawClippedRectangle(CIBBox2D(point1.x, point1.y, point2.x, point2.y));
}

void
CPixelRenderer::
drawClippedRectangle(const CIBBox2D &bbox)
{
  drawLine1(CIPoint2D(bbox.getXMin(), bbox.getYMin()), CIPoint2D(bbox.getXMax(), bbox.getYMin()));
  drawLine1(CIPoint2D(bbox.getXMax(), bbox.getYMin()), CIPoint2D(bbox.getXMax(), bbox.getYMax()));
  drawLine1(CIPoint2D(bbox.getXMax(), bbox.getYMax()), CIPoint2D(bbox.getXMin(), bbox.getYMax()));
  drawLine1(CIPoint2D(bbox.getXMin(), bbox.getYMax()), CIPoint2D(bbox.getXMin(), bbox.getYMin()));
}

void
CPixelRenderer::
fillRectangle(const CIBBox2D &bbox)
{
  CIPoint2D point1(bbox.getXMin(), bbox.getYMin());
  CIPoint2D point2(bbox.getXMax(), bbox.getYMax());

  int iwidth  = getWidth ();
  int iheight = getHeight();

  if (point2.x < 0 || point2.y < 0 || point1.x >= iwidth || point1.y >= iheight) return;

  point1.x = std::max(point1.x, 0); point2.x = std::min(point2.x, iwidth  - 1);
  point1.y = std::max(point1.y, 0); point2.y = std::min(point2.y, iheight - 1);

  if (! clip_.isEmpty()) {
    for (int y = point1.y; y <= point2.y; ++y)
      drawLine(CIPoint2D(point1.x, y), CIPoint2D(point2.x, y));
  }
  else
    fillClippedRectangle(CIBBox2D(point1.x, point1.y, point2.x, point2.y));
}

void
CPixelRenderer::
fillClippedRectangle(const CIBBox2D &bbox)
{
  int x1 = bbox.getXMin();
  int y1 = bbox.getYMin();
  int x2 = bbox.getXMax();
  int y2 = bbox.getYMax();

  for (int y = y1; y <= y2; ++y)
    drawClippedLine(CIPoint2D(x1, y), CIPoint2D(x2, y));
}

void
CPixelRenderer::
fillTriangle(const CIPoint2D &point1, const CIPoint2D &point2, const CIPoint2D &point3)
{
  if (! clip_.isEmpty()) {
    IPointList points;

    points.resize(3);

    points[0] = point1;
    points[1] = point2;
    points[2] = point3;

    fillPolygon(points);
  }
  else
    fillClippedTriangle(point1, point2, point3);
}

void
CPixelRenderer::
fillClippedTriangle(const CIPoint2D &point1, const CIPoint2D &point2, const CIPoint2D &point3)
{
  IPointList points;

  points.resize(3);

  points[0] = point1;
  points[1] = point2;
  points[2] = point3;

  fillClippedPolygon(points);
}

void
CPixelRenderer::
drawPolygon(const IPointList &points)
{
  if (! clip_.isEmpty()) {
    uint num_points = points.size();

    if (num_points == 0)
      return;

    uint i1 = num_points - 1;
    uint i2 = 0;

    for ( ; i2 < num_points; i1 = i2++)
      drawLine(points[i1], points[i2]);
  }
  else
    drawClippedPolygon(points);
}

void
CPixelRenderer::
drawClippedPolygon(const IPointList &points)
{
  uint num_points = points.size();

  if (num_points == 0)
    return;

  uint i1 = num_points - 1;
  uint i2 = 0;

  for ( ; i2 < num_points; i1 = i2++)
    drawLine1(points[i1], points[i2]);
}

void
CPixelRenderer::
fillPolygon(const IPointList &points)
{
  if (! clip_.isEmpty()) {
    uint num_points = points.size();

    if (num_points == 0)
      return;

    int ymin = getHeight();
    int ymax = -1;

    for (uint i1 = 0; i1 < num_points; ++i1) {
      ymin = std::min(ymin, points[i1].y);
      ymax = std::max(ymax, points[i1].y);
    }

    ymin = std::max(ymin, 0);
    ymax = std::min(ymax, (int) getHeight() - 1);

    for (int yy = ymin; yy <= ymax; ++yy) {
      int xmin = getWidth();
      int xmax = -1;

      int xx;

      for (uint i1 = num_points - 1, i2 = 0; i2 < num_points; i1 = i2++) {
        if ((points[i1].y < yy && points[i2].y < yy) ||
            (points[i1].y > yy && points[i2].y > yy) ||
            points[i1].y == points[i2].y)
          continue;

        if      (yy == points[i1].y)
          xx = points[i1].x;
        else if (yy == points[i2].y)
          xx = points[i2].x;
        else {
          double factor = double(points[i2].x - points[i1].x)/
                          double(points[i2].y - points[i1].y);

          xx = CMathGen::Round((yy - points[i1].y)*factor + points[i1].x);
        }

        xmin = std::min(xmin, xx);
        xmax = std::max(xmax, xx);
      }

      xmin = std::max(xmin, 0);
      xmax = std::min(xmax, (int) getWidth() - 1);

      if (xmin > xmax)
        continue;

      drawLine(CIPoint2D(xmin, yy), CIPoint2D(xmax, yy));
    }
  }
  else
    fillClippedPolygon(points);
}

void
CPixelRenderer::
fillClippedPolygon(const IPointList &points)
{
  uint num_points = points.size();

  if (num_points == 0)
    return;

  int ymin = getHeight();
  int ymax = -1;

  for (uint i1 = 0; i1 < num_points; ++i1) {
    ymin = std::min(ymin, points[i1].y);
    ymax = std::max(ymax, points[i1].y);
  }

  ymin = std::max(ymin, 0);
  ymax = std::min(ymax, (int) getHeight() - 1);

  for (int yy = ymin; yy <= ymax; ++yy) {
    int xmin = getWidth();
    int xmax = -1;

    int xx;

    for (uint i1 = num_points - 1, i2 = 0; i2 < num_points; i1 = i2++) {
      if ((points[i1].y < yy && points[i2].y < yy) ||
          (points[i1].y > yy && points[i2].y > yy) ||
          points[i1].y == points[i2].y)
        continue;

      if      (yy == points[i1].y)
        xx = points[i1].x;
      else if (yy == points[i2].y)
        xx = points[i2].x;
      else {
        double factor = double(points[i2].x - points[i1].x)/
                        double(points[i2].y - points[i1].y);

        xx = CMathGen::Round((yy - points[i1].y)*factor + points[i1].x);
      }

      xmin = std::min(xmin, xx);
      xmax = std::max(xmax, xx);
    }

    xmin = std::max(xmin, 0);
    xmax = std::min(xmax, (int) getWidth() - 1);

    if (xmin > xmax)
      continue;

    drawClippedLine(CIPoint2D(xmin, yy), CIPoint2D(xmax, yy));
  }
}

void
CPixelRenderer::
fillFilledPolygon(const IPointList &points, const CPixelRendererFiller &filler)
{
  if (! clip_.isEmpty()) {
    uint num_points = points.size();

    if (num_points == 0)
      return;

    int ymin = getHeight();
    int ymax = -1;

    for (uint i1 = 0; i1 < num_points; ++i1) {
      ymin = std::min(ymin, points[i1].y);
      ymax = std::max(ymax, points[i1].y);
    }

    ymin = std::max(ymin, 0);
    ymax = std::min(ymax, (int) getHeight() - 1);

    for (int yy = ymin; yy <= ymax; ++yy) {
      int xmin = getWidth();
      int xmax = -1;

      int xx;

      for (uint i1 = num_points - 1, i2 = 0; i2 < num_points; i1 = i2++) {
        if ((points[i1].y < yy && points[i2].y < yy) ||
            (points[i1].y > yy && points[i2].y > yy) ||
            points[i1].y == points[i2].y)
          continue;

        if      (yy == points[i1].y)
          xx = points[i1].x;
        else if (yy == points[i2].y)
          xx = points[i2].x;
        else {
          double factor = double(points[i2].x - points[i1].x)/
                          double(points[i2].y - points[i1].y);

          xx = CMathGen::Round((yy - points[i1].y)*factor + points[i1].x);
        }

        xmin = std::min(xmin, xx);
        xmax = std::max(xmax, xx);
      }

      xmin = std::max(xmin, 0);
      xmax = std::min(xmax, (int) getWidth() - 1);

      if (xmin > xmax)
        continue;

      drawFilledLine(CIPoint2D(xmin, yy), CIPoint2D(xmax, yy), filler);
    }
  }
  else
    fillFilledClippedPolygon(points, filler);
}

void
CPixelRenderer::
fillFilledClippedPolygon(const IPointList &points, const CPixelRendererFiller &filler)
{
  uint num_points = points.size();

  if (num_points == 0)
    return;

  int ymin = getHeight();
  int ymax = -1;

  for (uint i1 = 0; i1 < num_points; ++i1) {
    ymin = std::min(ymin, points[i1].y);
    ymax = std::max(ymax, points[i1].y);
  }

  ymin = std::max(ymin, 0);
  ymax = std::min(ymax, (int) getHeight() - 1);

  for (int yy = ymin; yy <= ymax; ++yy) {
    int xmin = getWidth();
    int xmax = -1;

    int xx;

    for (uint i1 = num_points - 1, i2 = 0; i2 < num_points; i1 = i2++) {
      if ((points[i1].y < yy && points[i2].y < yy) ||
          (points[i1].y > yy && points[i2].y > yy) ||
          points[i1].y == points[i2].y)
        continue;

      if      (yy == points[i1].y)
        xx = points[i1].x;
      else if (yy == points[i2].y)
        xx = points[i2].x;
      else {
        double factor = double(points[i2].x - points[i1].x)/
                        double(points[i2].y - points[i1].y);

        xx = CMathGen::Round((yy - points[i1].y)*factor + points[i1].x);
      }

      xmin = std::min(xmin, xx);
      xmax = std::max(xmax, xx);
    }

    xmin = std::max(xmin, 0);
    xmax = std::min(xmax, (int) getWidth() - 1);

    if (xmin > xmax)
      continue;

    drawFilledClippedLine(CIPoint2D(xmin, yy), CIPoint2D(xmax, yy), filler);
  }
}

void
CPixelRenderer::
fillImageRectangle(const CIBBox2D &bbox, CImagePtr image)
{
  IPointList points;

  points.resize(4);

  points[0] = bbox.getLL();
  points[1] = bbox.getLR();
  points[2] = bbox.getUR();
  points[3] = bbox.getUL();

  fillImagePolygon(points, image);
}

void
CPixelRenderer::
fillImagePolygon(const IPointList &points, CImagePtr image)
{
  if (! clip_.isEmpty()) {
    uint num_points = points.size();

    if (num_points == 0)
      return;

    int ymin = getHeight();
    int ymax = -1;

    for (uint i1 = 0; i1 < num_points; ++i1) {
      ymin = std::min(ymin, points[i1].y);
      ymax = std::max(ymax, points[i1].y);
    }

    ymin = std::max(ymin, 0);
    ymax = std::min(ymax, (int) getHeight() - 1);

    for (int yy = ymin; yy <= ymax; ++yy) {
      int xmin = getWidth();
      int xmax = -1;

      int xx;

      for (uint i1 = num_points - 1, i2 = 0; i2 < num_points; i1 = i2++) {
        if ((points[i1].y < yy && points[i2].y < yy) ||
            (points[i1].y > yy && points[i2].y > yy) ||
            points[i1].y == points[i2].y)
          continue;

        if      (yy == points[i1].y)
          xx = points[i1].x;
        else if (yy == points[i2].y)
          xx = points[i2].x;
        else {
          double factor = double(points[i2].x - points[i1].x)/
                          double(points[i2].y - points[i1].y);

          xx = CMathGen::Round((yy - points[i1].y)*factor + points[i1].x);
        }

        xmin = std::min(xmin, xx);
        xmax = std::max(xmax, xx);
      }

      xmin = std::max(xmin, 0);
      xmax = std::min(xmax, (int) getWidth() - 1);

      if (xmin > xmax)
        continue;

      drawLine(CIPoint2D(xmin, yy), CIPoint2D(xmax, yy));
    }
  }
  else
    fillImageClippedPolygon(points, image);
}

void
CPixelRenderer::
fillImageClippedRectangle(const CIBBox2D &bbox, CImagePtr image)
{
  IPointList points;

  points.resize(4);

  points[0] = bbox.getLL();
  points[1] = bbox.getLR();
  points[2] = bbox.getUR();
  points[3] = bbox.getUL();

  fillImageClippedPolygon(points, image);
}

void
CPixelRenderer::
fillImageClippedPolygon(const IPointList &points, CImagePtr image)
{
  if (! image.isValid())
    return;

  uint num_points = points.size();

  if (num_points == 0)
    return;

  int xmin = getWidth();
  int xmax = -1;
  int ymin = getHeight();
  int ymax = -1;

  for (uint i1 = 0; i1 < num_points; ++i1) {
    xmin = std::min(xmin, points[i1].x);
    ymin = std::min(ymin, points[i1].y);
    xmax = std::max(xmax, points[i1].x);
    ymax = std::max(ymax, points[i1].y);
  }

  int xo = xmin;
  int yo = ymin;

  ymin = std::max(ymin, 0);
  ymax = std::min(ymax, (int) getHeight() - 1);

  for (int yy = ymin; yy <= ymax; ++yy) {
    xmin = getWidth();
    xmax = -1;

    int xx;

    for (uint i1 = num_points - 1, i2 = 0; i2 < num_points; i1 = i2++) {
      if ((points[i1].y < yy && points[i2].y < yy) ||
          (points[i1].y > yy && points[i2].y > yy) ||
          points[i1].y == points[i2].y)
        continue;

      if      (yy == points[i1].y)
        xx = points[i1].x;
      else if (yy == points[i2].y)
        xx = points[i2].x;
      else {
        double factor = double(points[i2].x - points[i1].x)/
                        double(points[i2].y - points[i1].y);

        xx = CMathGen::Round((yy - points[i1].y)*factor + points[i1].x);
      }

      xmin = std::min(xmin, xx);
      xmax = std::max(xmax, xx);
    }

    xmin = std::max(xmin, 0);
    xmax = std::min(xmax, (int) getWidth() - 1);

    if (xmin > xmax)
      continue;

    //-----

    CRGBA rgba;

    int yi = yy - yo;

    yi %= image->getHeight();

    for (int xx = xmin; xx <= xmax; ++xx) {
      int xi = xx - xo;

      xi %= image->getWidth();

      image->getRGBAPixel(xi, yi, rgba);

      if (! rgba.isTransparent()) {
        setForeground(rgba);

        drawClippedPoint(CIPoint2D(xx, yy));
      }
    }
  }
}

void
CPixelRenderer::
fillGradientPolygon(const IPointList &points, CRefPtr<CGenGradient> gradient)
{
  fillGradientClippedPolygon(points, gradient);
}

void
CPixelRenderer::
fillGradientClippedPolygon(const IPointList &points, CRefPtr<CGenGradient> gradient)
{
  CIPoint2D min_point, max_point;

  CMathGeom2D::PointsRange(points, min_point, max_point);

  GenGradientPixelFill filler(gradient, min_point.x, min_point.y, max_point.x, max_point.y);

  fillFilledPolygon(points, filler);
}

void
CPixelRenderer::
fillPatternRectangle(const CIBBox2D &bbox, CBrushPattern pattern)
{
  fillPatternClippedRectangle(bbox, pattern);
}

void
CPixelRenderer::
fillPatternPolygon(const IPointList &ipoints, CBrushPattern pattern)
{
  fillPatternClippedPolygon(ipoints, pattern);
}

void
CPixelRenderer::
fillPatternClippedRectangle(const CIBBox2D &bbox, CBrushPattern pattern)
{
  fillImageClippedRectangle(bbox, getPatternImage(pattern));
}

void
CPixelRenderer::
fillPatternClippedPolygon(const IPointList &ipoints, CBrushPattern pattern)
{
  fillImageClippedPolygon(ipoints, getPatternImage(pattern));
}

CImagePtr
CPixelRenderer::
getPatternImage(CBrushPattern pattern)
{
  switch (pattern) {
    case CBRUSH_PATTERN_HORIZONTAL:     return getHorizontalImage();
    case CBRUSH_PATTERN_VERTICAL:       return getVerticalImage();
    case CBRUSH_PATTERN_CROSS:          return getCrossImage();
    case CBRUSH_PATTERN_DIAGONAL_UP:    return getUpImage();
    case CBRUSH_PATTERN_DIAGONAL_DOWN:  return getDownImage();
    case CBRUSH_PATTERN_DIAGONAL_CROSS: return getDiagonalCrossImage();
    case CBRUSH_PATTERN_DOTTED1:        return getDotted1Image();
    case CBRUSH_PATTERN_DOTTED2:        return getDotted2Image();
    case CBRUSH_PATTERN_DOTTED3:        return getDotted3Image();
    case CBRUSH_PATTERN_DOTTED4:        return getDotted4Image();
    case CBRUSH_PATTERN_DOTTED5:        return getDotted5Image();
    case CBRUSH_PATTERN_DOTTED6:        return getDotted6Image();
    case CBRUSH_PATTERN_DOTTED7:        return getDotted7Image();
    default:                            return CImagePtr();
  }
}

CImagePtr
CPixelRenderer::
getHorizontalImage()
{
#include "patterns/horizontal.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) horizontal_xpm_data,
                           sizeof(horizontal_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getVerticalImage()
{
#include "patterns/vertical.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) vertical_xpm_data,
                           sizeof(vertical_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getCrossImage()
{
#include "patterns/cross.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) cross_xpm_data,
                           sizeof(cross_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getUpImage()
{
#include "patterns/up.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) up_xpm_data,
                           sizeof(up_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getDownImage()
{
#include "patterns/down.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) down_xpm_data,
                           sizeof(down_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getDiagonalCrossImage()
{
#include "patterns/diagcross.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) diagonal_cross_xpm_data,
                           sizeof(diagonal_cross_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getDotted1Image()
{
#include "patterns/dense1.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) dense1_xpm_data,
                           sizeof(dense1_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getDotted2Image()
{
#include "patterns/dense2.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) dense2_xpm_data,
                           sizeof(dense2_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getDotted3Image()
{
#include "patterns/dense3.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) dense3_xpm_data,
                           sizeof(dense3_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getDotted4Image()
{
#include "patterns/dense4.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) dense4_xpm_data,
                           sizeof(dense4_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getDotted5Image()
{
#include "patterns/dense5.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) dense5_xpm_data,
                           sizeof(dense5_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getDotted6Image()
{
#include "patterns/dense6.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) dense6_xpm_data,
                           sizeof(dense6_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

CImagePtr
CPixelRenderer::
getDotted7Image()
{
#include "patterns/dense7.xpm"

  static CImagePtr image;

  if (! image)
    image = CImage::create((const char **) dense7_xpm_data,
                           sizeof(dense7_xpm_data)/sizeof(char *),
                           CFILE_TYPE_IMAGE_XPM);

  return image;
}

//-----------------

void
CPixelRenderer::
fillAAPolygon(double *x, double *y, uint num_xy)
{
  std::vector<CPoint2D> rpoints;

  rpoints.resize(num_xy);

  for (uint i = 0; i < num_xy; ++i) {
    rpoints[i].x = x[i];
    rpoints[i].y = y[i];
  }

  fillAAPolygon(rpoints);
}

void
CPixelRenderer::
fillFilledAAPolygon(double *x, double *y, uint num_xy, const CPixelRendererFiller &filler)
{
  std::vector<CPoint2D> rpoints;

  rpoints.resize(num_xy);

  for (uint i = 0; i < num_xy; ++i) {
    rpoints[i].x = x[i];
    rpoints[i].y = y[i];
  }

  fillFilledAAPolygon(rpoints, filler);
}

void
CPixelRenderer::
fillAAPolygon(const CPoint2D *points, uint num_points)
{
  std::vector<CPoint2D> rpoints;

  rpoints.resize(num_points);

  for (uint i = 0; i < num_points; ++i)
    rpoints[i] = points[i];

  fillAAPolygon(rpoints);
}

void
CPixelRenderer::
fillFilledAAPolygon(const CPoint2D *points, uint num_points, const CPixelRendererFiller &filler)
{
  std::vector<CPoint2D> rpoints;

  rpoints.resize(num_points);

  for (uint i = 0; i < num_points; ++i)
    rpoints[i] = points[i];

  fillFilledAAPolygon(rpoints, filler);
}

void
CPixelRenderer::
fillAAPolygon(const std::vector<CPoint2D> &points)
{
  CRGBA fg;

  getForeground(fg);

  CPixelRendererDefFiller filler(fg);

  fillFilledAAPolygon(points, filler);

  setForeground(fg);
}

void
CPixelRenderer::
fillFilledAAPolygon(const std::vector<CPoint2D> &points, const CPixelRendererFiller &filler)
{
  uint num_points = points.size();

  if (num_points == 0)
    return;

  // Calc bounding box

  double xmin = points[0].x; double ymin = points[0].y;
  double xmax = points[0].x; double ymax = points[0].y;

  for (uint i1 = 1; i1 < num_points; ++i1) {
    xmin = std::min(xmin, points[i1].x); ymin = std::min(ymin, points[i1].y);
    xmax = std::max(xmax, points[i1].x); ymax = std::max(ymax, points[i1].y);
  }

  int pxs = std::max((int) CMathGen::RoundDown(xmin), (int) 0);
  int pys = std::max((int) CMathGen::RoundDown(ymin), (int) 0);
  int pxe = std::min((int) CMathGen::RoundUp  (xmax), (int) getWidth () - 1);
  int pye = std::min((int) CMathGen::RoundUp  (ymax), (int) getHeight() - 1);

  int w = pxe - pxs;
  int h = pye - pys;

  CPolygon2D poly(points);

  std::vector<CPoint2D> points1;

  points1.resize(4);

  if      (w > h && w > 1) {
    int pxm = pxs + w/2;

    points1[0] = CPoint2D(pxs, pys);
    points1[1] = CPoint2D(pxm, pys);
    points1[2] = CPoint2D(pxm, pye);
    points1[3] = CPoint2D(pxs, pye);

    CPolygon2D poly1(points1);

    CPolygon2D ipoly;

    poly1.intersect(poly, ipoly);

    fillFilledAAPolygon(ipoly.getPoints(), filler);

    points1[0] = CPoint2D(pxm, pys);
    points1[1] = CPoint2D(pxe, pys);
    points1[2] = CPoint2D(pxe, pye);
    points1[3] = CPoint2D(pxm, pye);

    CPolygon2D poly2(points1);

    poly2.intersect(poly, ipoly);

    fillFilledAAPolygon(ipoly.getPoints(), filler);
  }
  else if (h > 1) {
    int pym = pys + h/2;

    points1[0] = CPoint2D(pxs, pys);
    points1[1] = CPoint2D(pxe, pys);
    points1[2] = CPoint2D(pxe, pym);
    points1[3] = CPoint2D(pxs, pym);

    CPolygon2D poly1(points1);

    CPolygon2D ipoly;

    poly1.intersect(poly, ipoly);

    fillFilledAAPolygon(ipoly.getPoints(), filler);

    points1[0] = CPoint2D(pxs, pym);
    points1[1] = CPoint2D(pxe, pym);
    points1[2] = CPoint2D(pxe, pye);
    points1[3] = CPoint2D(pxs, pye);

    CPolygon2D poly2(points1);

    poly2.intersect(poly, ipoly);

    fillFilledAAPolygon(ipoly.getPoints(), filler);
  }
  else {
    points1[0].x = pxs; points1[0].y = pys;
    points1[1].x = pxe; points1[1].y = pys;
    points1[2].x = pxe; points1[2].y = pye;
    points1[3].x = pxs; points1[3].y = pye;

    // TODO: fast intersect due to horizontal/vertical lines

    CPolygon2D poly1(points1);

    CPolygon2D ipoly;

    if (poly1.intersect(poly, ipoly)) {
      double area = ipoly.area();

      area = std::min(std::max(area, 0.0), 1.0);

      CRGBA rgba;

      filler.getColor(CIPoint2D(pxs, pys), rgba);

      rgba.setAlpha(rgba.getAlpha()*area);

      setForeground(rgba);

      drawPoint(CIPoint2D(pxs, pys));
    }
  }
}

void
CPixelRenderer::
drawHLine(int x1, int x2, int y)
{
  for (int x = x1; x <= x2; ++x)
    drawPoint(CIPoint2D(x, y));
}

void
CPixelRenderer::
drawClippedHLine(int x1, int x2, int y)
{
  for (int x = x1; x <= x2; ++x)
    drawClippedPoint(CIPoint2D(x, y));
}

void
CPixelRenderer::
drawVLine(int x, int y1, int y2)
{
  for (int y = y1; y <= y2; ++y)
    drawPoint(CIPoint2D(x, y));
}

void
CPixelRenderer::
drawClippedVLine(int x, int y1, int y2)
{
  for (int y = y1; y <= y2; ++y)
    drawClippedPoint(CIPoint2D(x, y));
}

void
CPixelRenderer::
drawCircle(const CIPoint2D &center, int r)
{
  drawEllipse(center, r, r);
}

void
CPixelRenderer::
fillCircle(const CIPoint2D &center, int r)
{
  fillEllipse(center, r, r);
}

void
CPixelRenderer::
fillImageCircle(const CIPoint2D &center, int r, CImagePtr image)
{
  fillImageEllipse(center, r, r, image);
}

void
CPixelRenderer::
drawEllipse(const CIPoint2D &center, int xr, int yr)
{
  CIPoint2D point(center);

  applyOffset(point);

  drawArc(point, xr, yr, 0, 360);
}

void
CPixelRenderer::
fillEllipse(const CIPoint2D &center, int xr, int yr)
{
  CIPoint2D point(center);

  applyOffset(point);

  fillArc(point, xr, yr, 0, 360);
}

void
CPixelRenderer::
fillImageEllipse(const CIPoint2D &center, int xr, int yr, CImagePtr image)
{
  CIPoint2D point(center);

  applyOffset(point);

  fillImageArc(point, xr, yr, 0, 360, image);
}

void
CPixelRenderer::
drawArc(const CIPoint2D &center, int xr, int yr, double a1, double a2)
{
  CIPoint2D point(center);

  applyOffset(point);

  drawClippedArc(point, xr, yr, a1, a2);
}

void
CPixelRenderer::
drawClippedArc(const CIPoint2D &center, int xr, int yr, double a1, double a2)
{
  while (a1 < 0) a1 += 360.0; while (a1 > 360) a1 -= 360.0;
  while (a2 < 0) a2 += 360.0; while (a2 > 360) a2 -= 360.0;

  if (a1 > a2) std::swap(a1, a2);

  drawClippedArc1(center, xr, yr, a1, a2);
}

void
CPixelRenderer::
drawClippedArc1(const CIPoint2D &center, int xr, int yr, double a1, double a2)
{
  double da = a2 - a1;

  if (da > 180) {
    drawClippedArc1(center, xr, yr, a1, a1 + 180);
    drawClippedArc1(center, xr, yr, a1 + 180, a2);

    return;
  }

  int x1 = int(center.x + xr*CMathGen::DCos(a1));
  int y1 = int(center.y + yr*CMathGen::DSin(a1));
  int x2 = int(center.x + xr*CMathGen::DCos(a2));
  int y2 = int(center.y + yr*CMathGen::DSin(a2));

  drawClippedArc2(center, xr, yr, x1, y1, x2, y2, a1, a2);
}

void
CPixelRenderer::
drawClippedArc2(const CIPoint2D &center, int xr, int yr,
                int x1, int y1, int x2, int y2, double a1, double a2)
{
  double a = 0.5*(a1 + a2);

  int xm1 = int(center.x + xr*CMathGen::DCos(a));
  int ym1 = int(center.y + yr*CMathGen::DSin(a));

  int xm2 = int(0.5*(x1 + x2));
  int ym2 = int(0.5*(y1 + y2));

  if (abs(xm1 - xm2) > 1 || abs(ym1 - ym2) > 1) {
    drawClippedArc2(center, xr, yr, x1, y1, xm1, ym1, a1, a);
    drawClippedArc2(center, xr, yr, xm1, ym1, x2, y2, a, a2);
  }
  else
    drawClippedLine(CIPoint2D(x1, y1), CIPoint2D(x2, y2));
}

void
CPixelRenderer::
fillArc(const CIPoint2D &center, int xr, int yr, double a1, double a2)
{
  CIPoint2D point(center);

  applyOffset(point);

  fillClippedArc(point, xr, yr, a1, a2);
}

void
CPixelRenderer::
fillClippedArc(const CIPoint2D &center, int xr, int yr, double a1, double a2)
{
  while (a1 < 0) a1 += 360.0; while (a1 > 360) a1 -= 360.0;
  while (a2 < 0) a2 += 360.0; while (a2 > 360) a2 -= 360.0;

  if (a1 > a2) std::swap(a1, a2);

  fillClippedArc1(center, xr, yr, a1, a2);
}

void
CPixelRenderer::
fillClippedArc1(const CIPoint2D &center, int xr, int yr, double a1, double a2)
{
  double da = a2 - a1;

  if (da > 180) {
    fillClippedArc1(center, xr, yr, a1, a1 + 180);
    fillClippedArc1(center, xr, yr, a1 + 180, a2);

    return;
  }

  IPointList points;

  points.resize(3);

  points[0] = center;
  points[1] = CIPoint2D(int(center.x + xr*CMathGen::DCos(a1)),
                        int(center.y + yr*CMathGen::DSin(a1)));
  points[2] = CIPoint2D(int(center.x + xr*CMathGen::DCos(a2)),
                        int(center.y + yr*CMathGen::DSin(a2)));

  fillPolygon(points);

  fillClippedArc2(center, xr, yr, points[1].x, points[1].y, points[2].x, points[2].y, a1, a2);
}

void
CPixelRenderer::
fillClippedArc2(const CIPoint2D &center, int xr, int yr,
                int x1, int y1, int x2, int y2, double a1, double a2)
{
  double a = 0.5*(a1 + a2);

  int xm1 = int(center.x + xr*CMathGen::DCos(a));
  int ym1 = int(center.y + yr*CMathGen::DSin(a));

  int xm2 = int(0.5*(x1 + x2));
  int ym2 = int(0.5*(y1 + y2));

  IPointList points;

  points.resize(3);

  points[0] = CIPoint2D(x1, y1);
  points[1] = CIPoint2D(xm1, ym1);
  points[2] = CIPoint2D(xm2, ym2);

  fillPolygon(points);

  points[0] = CIPoint2D(x2, y2);
  points[1] = CIPoint2D(xm1, ym1);
  points[2] = CIPoint2D(xm2, ym2);

  fillPolygon(points);

  if (abs(xm1 - xm2) > 1 || abs(ym1 - ym2) > 1) {
    fillClippedArc2(center, xr, yr, x1, y1, xm1, ym1, a1, a);
    fillClippedArc2(center, xr, yr, xm1, ym1, x2, y2, a, a2);
  }
}

void
CPixelRenderer::
fillImageArc(const CIPoint2D &center, int xr, int yr, double a1, double a2, CImagePtr image)
{
  CIPoint2D point(center);

  applyOffset(point);

  fillImageClippedArc(point, xr, yr, a1, a2, image);
}

void
CPixelRenderer::
fillImageClippedArc(const CIPoint2D &center, int xr, int yr, double a1, double a2, CImagePtr image)
{
  while (a1 < 0) a1 += 360.0; while (a1 > 360) a1 -= 360.0;
  while (a2 < 0) a2 += 360.0; while (a2 > 360) a2 -= 360.0;

  if (a1 > a2) std::swap(a1, a2);

  fillImageClippedArc1(center, xr, yr, a1, a2, image);
}

void
CPixelRenderer::
fillImageClippedArc1(const CIPoint2D &center, int xr, int yr,
                     double a1, double a2, CImagePtr image)
{
  double da = a2 - a1;

  if (da > 180) {
    fillImageClippedArc1(center, xr, yr, a1, a1 + 180, image);
    fillImageClippedArc1(center, xr, yr, a1 + 180, a2, image);

    return;
  }

  IPointList points;

  points.resize(3);

  points[0] = center;
  points[1] = CIPoint2D(int(center.x + xr*CMathGen::DCos(a1)),
                        int(center.y + yr*CMathGen::DSin(a1)));
  points[2] = CIPoint2D(int(center.x + xr*CMathGen::DCos(a2)),
                        int(center.y + yr*CMathGen::DSin(a2)));

  fillImagePolygon(points, image);

  fillImageClippedArc2(center, xr, yr, points[1].x, points[1].y,
                       points[2].x, points[2].y, a1, a2, image);
}

void
CPixelRenderer::
fillImageClippedArc2(const CIPoint2D &center, int xr, int yr,
                     int x1, int y1, int x2, int y2, double a1, double a2, CImagePtr image)
{
  double a = 0.5*(a1 + a2);

  int xm1 = int(center.x + xr*CMathGen::DCos(a));
  int ym1 = int(center.y + yr*CMathGen::DSin(a));

  int xm2 = int(0.5*(x1 + x2));
  int ym2 = int(0.5*(y1 + y2));

  IPointList points;

  points.resize(3);

  points[0] = CIPoint2D(x1, y1);
  points[1] = CIPoint2D(xm1, ym1);
  points[2] = CIPoint2D(xm2, ym2);

  fillImagePolygon(points, image);

  points[0] = CIPoint2D(x2, y2);
  points[1] = CIPoint2D(xm1, ym1);
  points[2] = CIPoint2D(xm2, ym2);

  fillImagePolygon(points, image);

  if (abs(xm1 - xm2) > 1 || abs(ym1 - ym2) > 1) {
    fillImageClippedArc2(center, xr, yr, x1, y1, xm1, ym1, a1, a, image);
    fillImageClippedArc2(center, xr, yr, xm1, ym1, x2, y2, a, a2, image);
  }
}
void
CPixelRenderer::
drawImage(const CIPoint2D &point, CImagePtr image)
{
  CIPoint2D point1(point);

  applyOffset(point1);

  if (! clip_.isEmpty()) {
    int iwidth  = getWidth ();
    int iheight = getHeight();

    int iwidth1  = image->getWidth ();
    int iheight1 = image->getHeight();

    int x1 = point1.x;
    int x2 = std::min(point1.x + iwidth1  - 1, iwidth  - 1);
    int y1 = point1.y;
    int y2 = std::min(point1.y + iheight1 - 1, iheight - 1);

    CRGBA rgba;

    uint  num;
    int  *xx1, *yy1, *xx2, *yy2;

    for (int y = y1; y <= y2; ++y) {
      clip_.processLine(x1, y, x2, y, &xx1, &yy1, &xx2, &yy2, &num);

      for (uint i = 0; i < num; ++i) {
        for (int x = xx1[i]; x <= xx2[i]; ++x) {
          image->getRGBAPixel(x - x1, y - y1, rgba);

          setForeground(rgba);

          drawClippedPoint(CIPoint2D(x, y));
        }
      }
    }
  }
  else
    drawClippedImage(point1, image);
}

void
CPixelRenderer::
drawClippedImage(const CIPoint2D &point, CImagePtr image)
{
  int iwidth  = getWidth ();
  int iheight = getHeight();

  int iwidth1  = image->getWidth ();
  int iheight1 = image->getHeight();

  int x1 = point.x;
  int x2 = std::min(point.x + iwidth1  - 1, iwidth  - 1);
  int y1 = point.y;
  int y2 = std::min(point.y + iheight1 - 1, iheight - 1);

  CRGBA rgba;

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      image->getRGBAPixel(x - x1, y - y1, rgba);

      setForeground(rgba);

      drawClippedPoint(CIPoint2D(x, y));
    }
  }
}

void
CPixelRenderer::
drawAlphaImage(const CIPoint2D &point, CImagePtr image)
{
  drawImage(point, image);
}

void
CPixelRenderer::
drawClippedAlphaImage(const CIPoint2D &point, CImagePtr image)
{
  drawClippedImage(point, image);
}

void
CPixelRenderer::
drawImageInBox(const CIBBox2D &bbox, CImagePtr image)
{
  int xmin = bbox.getXMin();
  int ymin = bbox.getYMin();
  int xmax = bbox.getXMax();
  int ymax = bbox.getYMax();

  CIPoint2D point1(xmin, ymin);
  CIPoint2D point2(xmax, ymax);

  applyOffset(point1);
  applyOffset(point2);

  int iwidth1  = image->getWidth ();
  int iheight1 = image->getHeight();

  int iwidth2  = abs(point2.x - point1.x + 1);
  int iheight2 = abs(point2.y - point1.y + 1);

  double sx = (iwidth1  > 0 ? (1.0*iwidth2 )/iwidth1  : 1);
  double sy = (iheight1 > 0 ? (1.0*iheight2)/iheight1 : 1);

  drawScaledImage(CIPoint2D(xmin, ymin), image, sx, sy);
}

void
CPixelRenderer::
drawAlphaImageInBox(const CIBBox2D &bbox, CImagePtr image)
{
  drawImageInBox(bbox, image);
}

void
CPixelRenderer::
drawScaledImage(const CIPoint2D &point, CImagePtr image, double sx, double sy)
{
  CIPoint2D point1(point);

  applyOffset(point1);

  if (! clip_.isEmpty()) {
    int iwidth  = getWidth ();
    int iheight = getHeight();

    int iwidth1  = int(sx*image->getWidth ());
    int iheight1 = int(sy*image->getHeight());

    double isx = 1.0/sx;
    double isy = 1.0/sy;

    int x1 = point1.x;
    int x2 = std::min(point1.x + iwidth1  - 1, iwidth  - 1);
    int y1 = point1.y;
    int y2 = std::min(point1.y + iheight1 - 1, iheight - 1);

    CRGBA rgba;

    uint  num;
    int  *xx1, *yy1, *xx2, *yy2;

    for (int y = y1; y <= y2; ++y) {
      clip_.processLine(x1, y, x2, y, &xx1, &yy1, &xx2, &yy2, &num);

      for (uint i = 0; i < num; ++i) {
        for (int x = xx1[i]; x <= xx2[i]; ++x) {
          int ix = (x - x1)*isx;
          int iy = (y - y1)*isy;

          image->getRGBAPixel(ix, iy, rgba);

          setForeground(rgba);

          drawClippedPoint(CIPoint2D(x, y));
        }
      }
    }
  }
  else
    drawClippedScaledImage(point1, image, sx, sy);
}

void
CPixelRenderer::
drawClippedScaledImage(const CIPoint2D &point, CImagePtr image, double sx, double sy)
{
  int iwidth  = getWidth ();
  int iheight = getHeight();

  int iwidth1  = int(sx*image->getWidth ());
  int iheight1 = int(sy*image->getHeight());

  double isx = 1.0/sx;
  double isy = 1.0/sy;

  int x1 = point.x;
  int x2 = std::min(point.x + iwidth1  - 1, iwidth  - 1);
  int y1 = point.y;
  int y2 = std::min(point.y + iheight1 - 1, iheight - 1);

  CRGBA rgba;

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      int ix = (x - x1)*isx;
      int iy = (y - y1)*isy;

      image->getRGBAPixel(ix, iy, rgba);

      setForeground(rgba);

      drawClippedPoint(CIPoint2D(x, y));
    }
  }
}

int
CPixelRenderer::
getCharWidth()
{
  if (font_.isValid())
    return font_->getICharWidth();

  return 8;
}

int
CPixelRenderer::
getCharWidth(char c)
{
  if (font_.isValid()) {
    std::string str(&c, 1);

    return font_->getIStringWidth(str);
  }

  return 8;
}

int
CPixelRenderer::
getCharHeight()
{
  return getCharAscent() + getCharDescent();
}

int
CPixelRenderer::
getCharAscent()
{
  if (font_.isValid())
    return font_->getICharAscent();

  return 10;
}

int
CPixelRenderer::
getCharDescent()
{
  if (font_.isValid())
    return font_->getICharDescent();

  return 2;
}

int
CPixelRenderer::
getStringWidth(const std::string &str)
{
  if (font_.isValid())
    return font_->getIStringWidth(str);

  return str.size()*getCharWidth();
}

void
CPixelRenderer::
flush()
{
}

//----------

CImagePtr
CPixelRenderer::
getImage()
{
  return CImagePtr();
}

void
CPixelRenderer::
setImage(CImagePtr image)
{
  drawImage(CIPoint2D(0, 0), image);
}

CImagePtr
CPixelRenderer::
getAlphaImage()
{
  CImagePtr image = getImage();

  CImagePtr alpha_image = image->dup();

  alpha_image->setRGBAData(CRGBA(0,0,0,0));

  alpha_image->setAlphaByImage(image);

  return alpha_image;
}

void
CPixelRenderer::
setAlphaImage(CImagePtr image)
{
  drawImage(CIPoint2D(0, 0), image);
}

//----------

void
CPixelRenderer::
setClip(int *x, int *y, uint num_xy)
{
  IPointList points;

  for (uint i = 0; i < num_xy; ++i)
    points.push_back(CIPoint2D(x[i], y[i]));

  resetClip();

  CPixelRendererPath path;

  path.addPathPolygon(points);

  clipPolygons(path.getIPolygons());
}

void
CPixelRenderer::
resetClip()
{
  clip_.init();
}

//----------

void
CPixelRenderer::
clipPolygons(IPointListList &point_list_list)
{
  clip_.addPolygons(FILL_TYPE_WINDING, point_list_list);
}

void
CPixelRenderer::
eoclipPolygons(IPointListList &point_list_list)
{
  clip_.addPolygons(FILL_TYPE_EVEN_ODD, point_list_list);
}

const CPixelRenderer::IPointListList &
CPixelRenderer::
getClipPolygons(uint i) const
{
  return clip_.getPolygons(i);
}

uint
CPixelRenderer::
getNumClipPolygons() const
{
  return clip_.getSize();
}

//----------

#if 0
void
CPixelRenderer::
fillPolygons(const CPixelRendererPath &path)
{
  CImagePtr timage;

  fillPolygons(path.getIPolygons(), timage, FILL_TYPE_WINDING);
  fillPolygons(path.getRPolygons(), timage, FILL_TYPE_WINDING);
}

void
CPixelRenderer::
eofillPolygons(const CPixelRendererPath &path)
{
  CImagePtr timage;

  fillPolygons(path.getIPolygons(), timage, FILL_TYPE_EVEN_ODD);
  fillPolygons(path.getRPolygons(), timage, FILL_TYPE_EVEN_ODD);
}

void
CPixelRenderer::
fillPolygons(const CPixelRendererPath &path, CImagePtr image)
{
  fillPolygons(path.getIPolygons(), image, FILL_TYPE_WINDING);
  fillPolygons(path.getRPolygons(), image, FILL_TYPE_WINDING);
}

void
CPixelRenderer::
eofillPolygons(const CPixelRendererPath &path, CImagePtr image)
{
  fillPolygons(path.getIPolygons(), image, FILL_TYPE_EVEN_ODD);
  fillPolygons(path.getRPolygons(), image, FILL_TYPE_EVEN_ODD);
}
#endif

struct CPixelIData {
  int x, xm, o;
};

void
CPixelRenderer::
fillPathPolygons(IPointListList &poly_points_list, CImagePtr image, CFillType type)
{
  typedef std::vector<CPixelIData> IDataList;

  static IDataList idata_list;

  COptValT<int> xmin;
  COptValT<int> ymin;
  COptValT<int> xmax;
  COptValT<int> ymax;

  uint ni1 = 0;

  uint num_polygons = poly_points_list.size();

  for (uint i = 0; i < num_polygons; ++i) {
    IPointList *points = poly_points_list[i];

    uint num_points = points->size();

    for (uint j = 0; j < num_points; ++j) {
      xmin.updateMin((*points)[j].x);
      ymin.updateMin((*points)[j].y);
      xmax.updateMax((*points)[j].x);
      ymax.updateMax((*points)[j].y);

      ++ni1;
    }
  }

  if (ni1 == 0)
    return;

  idata_list.resize(ni1);

  //---------

  ymin.updateMax(0);
  ymax.updateMin(getHeight() - 1);

  //---------

  uint ni2 = ni1;

  for (int y = ymin.getValue(); y <= ymax.getValue(); ++y) {
    COptValT<int> xmin1;
    COptValT<int> xmax1;

    ni2 = 0;

    uint num_polygons = poly_points_list.size();

    for (uint i = 0; i < num_polygons; ++i) {
      IPointList *points = poly_points_list[i];

      uint num_points = points->size();

      if (num_points < 3)
        continue;

      uint i1 = num_points - 1;
      uint i2 = 0;

      int x1 = (*points)[i1].x;
      int y1 = (*points)[i1].y;

      int x, xm, o;

      for ( ; i2 < num_points; i1 = i2++) {
        int x2 = (*points)[i2].x;
        int y2 = (*points)[i2].y;

        if ((y1 < y && y2 < y) || (y1 > y && y2 > y) || y1 == y2)
          goto next;

        xm = (x1 + x2)/2;

        if      (y == y1)
          x = x1;
        else if (y == y2)
          x = x2;
        else {
          double factor = double(x2 - x1)/double(y2 - y1);

          x = CMathGen::Round((y - y1)*factor + x1);

          x = std::max(x, 0);
          x = std::min(x, (int) getWidth() - 1);
        }

        o = CMathGen::sign((long) (y2 - y1));

        xmin1.updateMin(x);
        xmax1.updateMax(x);

        idata_list[ni2].x  = x;
        idata_list[ni2].xm = xm;
        idata_list[ni2].o  = o;

        ++ni2;

 next:
        x1 = x2;
        y1 = y2;
      }
    }

    xmin1.updateMax(0);
    xmax1.updateMin(getWidth() - 1);

    if (xmin1.getValue() > xmax1.getValue())
      continue;

    if (ni2 == 0)
      continue;

    //-------

    // Sort

    uint i;

    do {
      i = 0;

      for (uint i1 = i, i2 = i1 + 1; i1 < ni2 - 1; i1 = i2++) {
        if ((idata_list[i1].x  >  idata_list[i2].x) ||
            (idata_list[i1].x  == idata_list[i2].x  &&
             idata_list[i1].xm >  idata_list[i2].xm)) {
          std::swap(idata_list[i1], idata_list[i2]);

          i = i1;

          break;
        }
      }
    } while (i > 0 && i < ni2 - 1);

    // Remove duplicates

    uint ni3 = ni2;

    i = 0;

    for (uint i1 = i, i2 = i1 + 1; i2 < ni3; i1 = i2++) {
      if (idata_list[i1].x  == idata_list[i2].x  &&
          idata_list[i1].xm == idata_list[i2].xm &&
          idata_list[i1].o  == idata_list[i2].o) {
        for (uint j1 = i2, j2 = j1 + 1; j2 < ni3; j1 = j2++)
          idata_list[j1] = idata_list[j2];

        --ni3;
        --i2;
      }
    }

    //------

    if (type == FILL_TYPE_WINDING) {
      for (uint i = 1; i < ni3; ++i) {
        int i1 = 0;

        for (uint j = 0; j < i; ++j)
          i1 += idata_list[j].o;

        if (i1 != 0) {
          int x1 = idata_list[i - 1].x;
          int x2 = idata_list[i    ].x;

          if (! image)
            drawHLine(x1, x2, y);
          else {
            CRGBA rgba;

            int yi = y - ymin.getValue();

            for (int x = x1; x <= x2; ++x) {
              int xi = x - xmin.getValue();

              if (image->validPixel(xi, yi)) {
                image->getRGBAPixel(xi, yi, rgba);

                setForeground(rgba);
              }

              drawPoint(CIPoint2D(x, y));
            }
          }
        }
      }
    }
    else {
      int  x1 = idata_list[0].x;
      int  x2 = idata_list[1].x;
      uint j  = 1;

      for (uint i = 1; i < ni3; ++i) {
        if (j == 1) {
          if (! image)
            drawHLine(x1, x2, y);
          else {
            CRGBA rgba;

            int yi = y - ymin.getValue();

            for (int x = x1; x <= x2; ++x) {
              int xi = x - xmin.getValue();

              if (image->validPixel(xi, yi)) {
                image->getRGBAPixel(xi, yi, rgba);

                setForeground(rgba);
              }

              drawPoint(CIPoint2D(x, y));
            }
          }
        }

        j = 1 - j;

        x2 = x1;
        x1 = idata_list[i].x;
      }
    }
  }
}

struct CPixelRData {
  double x, xm, o;
};

void
CPixelRenderer::
fillPathPolygons(RPointListList &poly_points_list, CImagePtr image, CFillType type)
{
  typedef std::vector<CPixelRData> RDataList;

  static RDataList rdata_list;

  COptValT<double> xmin;
  COptValT<double> ymin;
  COptValT<double> xmax;
  COptValT<double> ymax;

  uint ni = 0;

  uint num_polygons = poly_points_list.size();

  for (uint i = 0; i < num_polygons; ++i) {
    RPointList *points = poly_points_list[i];

    uint num_points = points->size();

    for (uint j = 0; j < num_points; ++j) {
      xmin.updateMin((*points)[j].x);
      ymin.updateMin((*points)[j].y);
      xmax.updateMax((*points)[j].x);
      ymax.updateMax((*points)[j].y);

      ++ni;
    }
  }

  if (ni == 0)
    return;

  rdata_list.resize(ni);

  //---------

  ymin.updateMax(0);
  ymax.updateMin(getHeight() - 1);

  //---------

  for (double y = ymin.getValue(); y <= ymax.getValue(); ++y) {
    COptValT<double> xmin1;
    COptValT<double> xmax1;

    ni = 0;

    uint num_polygons = poly_points_list.size();

    for (uint i = 0; i < num_polygons; ++i) {
      RPointList *points = poly_points_list[i];

      uint num_points = points->size();

      if (num_points < 3)
        continue;

      uint i1 = num_points - 1;
      uint i2 = 0;

      double x1 = (*points)[i1].x;
      double y1 = (*points)[i1].y;

      double x, xm, o;

      for ( ; i2 < num_points; i1 = i2++) {
        double x2 = (*points)[i2].x;
        double y2 = (*points)[i2].y;

        if ((y1 < y && y2 < y) || (y1 > y && y2 > y) || y1 == y2)
          goto next;

        xm = (x1 + x2)/2;

        if      (y == y1)
          x = x1;
        else if (y == y2)
          x = x2;
        else {
          double factor = double(x2 - x1)/double(y2 - y1);

          x = CMathGen::Round((y - y1)*factor + x1);

          x = std::max(x, 0.0);
          x = std::min(x, (int) getWidth() - 1.0);
        }

        o = CMathGen::sign(y2 - y1);

        xmin1.updateMin(x);
        xmax1.updateMax(x);

        rdata_list[ni].x  = x;
        rdata_list[ni].xm = xm;
        rdata_list[ni].o  = o;

        ++ni;

 next:
        x1 = x2;
        y1 = y2;
      }
    }

    xmin1.updateMax(0);
    xmax1.updateMin(getWidth() - 1);

    if (xmin1.getValue() > xmax1.getValue())
      continue;

    if (ni == 0)
      continue;

    //-------

    // Sort

    uint i;

    do {
      i = 0;

      for (uint i1 = i, i2 = i1 + 1; i1 < ni - 1; i1 = i2++) {
        if ((rdata_list[i1].x >  rdata_list[i2].x) ||
            (rdata_list[i1].x  == rdata_list[i2].x  &&
             rdata_list[i1].xm >  rdata_list[i2].xm)) {
          std::swap(rdata_list[i1], rdata_list[i2]);

          i = i1;

          break;
        }
      }
    } while (i > 0 && i < ni - 1);

    // Remove duplicates

    i = 0;

    for (uint i1 = i, i2 = i1 + 1; i2 < ni; i1 = i2++) {
      if (rdata_list[i1].x  == rdata_list[i2].x  &&
          rdata_list[i1].xm == rdata_list[i2].xm &&
          rdata_list[i1].o  == rdata_list[i2].o) {
        for (uint j1 = i2, j2 = j1 + 1; j2 < ni; j1 = j2++)
          rdata_list[j1] = rdata_list[j2];

        --ni;
        --i2;
      }
    }

    //------

    if (type == FILL_TYPE_WINDING) {
      for (uint i = 1; i < ni; ++i) {
        double i1 = 0;

        for (uint j = 0; j < i; ++j)
          i1 += rdata_list[j].o;

        if (i1 != 0) {
          double x1 = rdata_list[i - 1].x;
          double x2 = rdata_list[i    ].x;

          if (! image)
            drawHLine(int(x1), int(x2), int(y));
          else {
            CRGBA rgba;

            double yi = y - ymin.getValue();

            for (double x = x1; x <= x2; ++x) {
              double xi = x - xmin.getValue();

              if (image->validPixel(int(xi), int(yi))) {
                image->getRGBAPixel(int(xi), int(yi), rgba);

                setForeground(rgba);
              }

              drawPoint(CIPoint2D(int(x), int(y)));
            }
          }
        }
      }
    }
    else {
      double x1 = rdata_list[0].x;
      double x2 = rdata_list[1].x;
      uint   j  = 1;

      for (uint i = 1; i < ni; ++i) {
        if (j == 1) {
          if (! image)
            drawHLine(int(x1), int(x2), int(y));
          else {
            CRGBA rgba;

            double yi = y - ymin.getValue();

            for (double x = x1; x <= x2; ++x) {
              double xi = x - xmin.getValue();

              if (image->validPixel(int(xi), int(yi))) {
                image->getRGBAPixel(int(xi), int(yi), rgba);

                setForeground(rgba);
              }

              drawPoint(CIPoint2D(int(x), int(y)));
            }
          }
        }

        j = 1 - j;

        x2 = x1;
        x1 = rdata_list[i].x;
      }
    }
  }
}

//------------------------

void
CPixelRenderer::
fillFlatTriangle(const CIPoint2D &point1, const CIPoint2D &point2, const CIPoint2D &point3)
{
  CIPoint2D p1(point1);
  CIPoint2D p2(point2);
  CIPoint2D p3(point3);

  if ((p1.x == p2.x && p2.x == p3.x) || (p1.y == p2.y && p2.y == p3.y))
    return;

  if (p2.y < p1.y)
    std::swap(p1, p2);

  if (p3.y < p1.y)
    std::swap(p1, p3);

  if (p3.y < p2.y)
    std::swap(p2, p3);

  int clip_x1, clip_y1, clip_x2, clip_y2;

  getPixelClip(&clip_x1, &clip_y1, &clip_x2, &clip_y2);

  if (p3.y < clip_y1 || p1.y > clip_y2 ||
      (p1.x < clip_x1 && p2.x < clip_x1 && p3.x < clip_x1) ||
      (p1.x > clip_x2 && p2.x > clip_x2 && p3.x > clip_x2) )
    return;

  if      (p1.y == p2.y)
    fillTriangleTop(p1, p2, p3);
  else if (p2.y == p3.y)
    fillTriangleBot(p1, p2, p3);
  else {
    int x4 = p1.x + ROUND((p2.y - p1.y)*(((double) (p3.x - p1.x))/(p3.y - p1.y)));

    CIPoint2D p4(x4, p2.y);

    fillTriangleBot(p1, p4, p2);
    fillTriangleTop(p2, p4, p3);
  }
}

// Flat Bottom (x1,y1) -> (x2, y2)

void
CPixelRenderer::
fillTriangleTop(const CIPoint2D &point1, const CIPoint2D &point2, const CIPoint2D &point3)
{
  CIPoint2D p1(point1);
  CIPoint2D p2(point2);
  CIPoint2D p3(point3);

  assert(p1.y == p2.y);

  int clip_x1, clip_y1, clip_x2, clip_y2;

  getPixelClip(&clip_x1, &clip_y1, &clip_x2, &clip_y2);

  if (p2.x < p1.x)
    std::swap(p1.x, p2.x);

  double dy = double(p3.y - p1.y);

  double dx_left  = (p3.x - p1.x)/dy;
  double dx_right = (p3.x - p2.x)/dy;

  double xs = p1.x;
  double xe = p2.x;

  if (p1.y < clip_y1) {
    dy = clip_y1 - p1.y;

    xs += dx_left *dy;
    xe += dx_right*dy;

    p1.y = clip_y1;
  }

  if (p3.y > clip_y2)
    p3.y = clip_y2;

  if (p1.x >= clip_x1 && p1.x <= clip_x2 &&
      p2.x >= clip_x1 && p2.x <= clip_x2 &&
      p3.x >= clip_x1 && p3.x <= clip_x2) {
    for (int y = p1.y; y <= p3.y; ++y) {
      drawLine(CIPoint2D(int(xs), y), CIPoint2D(int(xe), y));

      xs += dx_left;
      xe += dx_right;
    }
  }
  else {
    for (int y = p1.y; y <= p3.y; ++y) {
      int left  = int(xs);
      int right = int(xe);

      if (left < clip_x1) {
        left = clip_x1;

        if (right < clip_x1)
          continue;
      }

      if (right > clip_x2) {
        right = clip_x2;

        if (left > clip_x2)
          continue;
      }

      drawLine(CIPoint2D(left, y), CIPoint2D(right, y));

      xs += dx_left;
      xe += dx_right;
    }
  }
}

// Flat Top (x2,y2) -> (x3, y3)

void
CPixelRenderer::
fillTriangleBot(const CIPoint2D &point1, const CIPoint2D &point2, const CIPoint2D &point3)
{
  CIPoint2D p1(point1);
  CIPoint2D p2(point2);
  CIPoint2D p3(point3);

  assert(p2.y == p3.y);

  int clip_x1, clip_y1, clip_x2, clip_y2;

  getPixelClip(&clip_x1, &clip_y1, &clip_x2, &clip_y2);

  if (p3.x < p2.x)
    std::swap(p2.x, p3.x);

  double height = double(p3.y - p1.y);

  double dx_left  = (p2.x - p1.x)/height;
  double dx_right = (p3.x - p1.x)/height;

  double xs = p1.x;
  double xe = p1.x;

  if (p1.y < clip_y1) {
    xs = xs + dx_left *(clip_y1 - p1.y);
    xe = xe + dx_right*(clip_y1 - p1.y);

    p1.y = clip_y1;
  }

  if (p3.y > clip_y2)
    p3.y = clip_y2;

  if (p1.x >= clip_x1 && p1.x <= clip_x2 &&
      p2.x >= clip_x1 && p2.x <= clip_x2 &&
      p3.x >= clip_x1 && p3.x <= clip_x2) {
    for (int y = p1.y; y <= p3.y; ++y) {
      drawLine(CIPoint2D(int(xs), y), CIPoint2D(int(xe), y));

      xs += dx_left;
      xe += dx_right;
    }
  }
  else {
    for (int y = p1.y; y <= p3.y; ++y) {
      int left  = int(xs);
      int right = int(xe);

      xs += dx_left;
      xe += dx_right;

      if (left < clip_x1) {
        left = clip_x1;

        if (right < clip_x1)
          continue;
      }

      if (right > clip_x2) {
        right = clip_x2;

        if (left > clip_x2)
          continue;
      }

      drawLine(CIPoint2D(left, y), CIPoint2D(right, y));
    }
  }
}

void
CPixelRenderer::
fillGouraudTriangle(const CPoint2D &point1, CRGBA rgb1, const CPoint2D &point2, CRGBA rgb2,
                    const CPoint2D &point3, CRGBA rgb3)
{
  CPoint2D p1(point1);
  CPoint2D p2(point2);
  CPoint2D p3(point3);

  int clip_x1, clip_y1, clip_x2, clip_y2;

  getPixelClip(&clip_x1, &clip_y1, &clip_x2, &clip_y2);

  if ((p1.x < clip_x1 && p2.x < clip_x1 && p3.x < clip_x1) ||
      (p1.y < clip_y1 && p2.y < clip_y1 && p3.y < clip_y1) ||
      (p1.x > clip_x2 && p2.x > clip_x2 && p3.x > clip_x2) ||
      (p1.y > clip_y2 && p2.y > clip_y2 && p3.y > clip_y2))
    return;

  if ((CMathGen::cmp(p1.x, p2.x) && CMathGen::cmp(p2.x, p3.x)) ||
      (CMathGen::cmp(p1.y, p2.y) && CMathGen::cmp(p2.y, p3.y)))
    return;

  if (p2.y < p1.y) {
    std::swap(p1  , p2  );
    std::swap(rgb1, rgb2);
  }

  if (p3.y < p1.y) {
    std::swap(p1  , p3  );
    std::swap(rgb1, rgb3);
  }

  if (p3.y < p2.y) {
    std::swap(p2  , p3  );
    std::swap(rgb2, rgb3);
  }

  if      (CMathGen::cmp(p1.y, p2.y))
    fillGouraudTriangleTop(p1, rgb1, p2, rgb2, p3, rgb3);
  else if (CMathGen::cmp(p2.y, p3.y))
    fillGouraudTriangleBot(p1, rgb1, p2, rgb2, p3, rgb2);
  else {
    double dy = (p2.y - p1.y)/(p3.y - p1.y);

    double x4 = p1.x + dy*(p3.x - p1.x);

    CRGBA rgb4 = rgb1 + dy*(rgb3 - rgb1);

    CPoint2D p4(x4, p2.y);

    fillGouraudTriangleBot(p1, rgb1, p4, rgb4, p2, rgb2);
    fillGouraudTriangleTop(p2, rgb2, p4, rgb4, p3, rgb3);
  }
}

// Flat Top (x1,y1) -> (p2.x, y2)

void
CPixelRenderer::
fillGouraudTriangleTop(const CPoint2D &point1, CRGBA rgb1, const CPoint2D &point2, CRGBA rgb2,
                       const CPoint2D &point3, CRGBA rgb3)
{
  CPoint2D p1(point1);
  CPoint2D p2(point2);
  CPoint2D p3(point3);

  assert(p1.y == p2.y);

  double iy1, iy3;
  int    clip_x1, clip_y1, clip_x2, clip_y2;

  getPixelClip(&clip_x1, &clip_y1, &clip_x2, &clip_y2);

  if (p1.y < clip_y1)
    iy1 = clip_y1;
  else
    iy1 = ceil(p1.y);

  if (p3.y > clip_y2)
    iy3 = clip_y2 - 1;
  else
    iy3 = ceil(p3.y) - 1;

  if (iy3 < iy1)
    return;

  if (p2.x < p1.x) {
    std::swap(p1.x, p2.x);
    std::swap(rgb1, rgb2);
  }

  double height = p3.y - p1.y;

  double dx_left  = (p3.x - p1.x)/height;
  double dx_right = (p3.x - p2.x)/height;

  CRGBA drgb_left  = (rgb3 - rgb1)/height;
  CRGBA drgb_right = (rgb3 - rgb2)/height;

  double xs = p1.x;
  double xe = p2.x;

  CRGBA rgbs = rgb1;
  CRGBA rgbe = rgb2;

  double dy = iy1 - p1.y;

  xs += dx_left *dy;
  xe += dx_right*dy;

  rgbs += drgb_left *dy;
  rgbe += drgb_right*dy;

  CRGBA  drgb, rgb;
  double ixs, ixe, dx, x, y;

  if (p1.x >= clip_x1 && p1.x <= clip_x2 &&
      p2.x >= clip_x1 && p2.x <= clip_x2 &&
      p3.x >= clip_x1 && p3.x <= clip_x2) {
    for (y = iy1; y <= iy3; ++y) {
      dx = xe - xs;

      drgb = (rgbe - rgbs)/dx;

      ixs = ceil(xs);
      ixe = ceil(xe) - 1;

      dx = ixs - xs;

      rgb = rgbs + drgb*dx;

      for (x = ixs; x <= ixe; ++x, rgb += drgb) {
        setForeground(rgb);

        drawPoint(CIPoint2D(int(x), int(y)));
      }

      xs += dx_left;
      xe += dx_right;

      rgbs += drgb_left;
      rgbe += drgb_right;
    }
  }
  else {
    for (y = iy1; y <= iy3; ++y) {
      dx = xe - xs;

      drgb = (rgbe - rgbs)/dx;

      if (xs < clip_x1) {
        if (xe < clip_x1)
          continue;

        ixs = clip_x1;
      }
      else
        ixs = ceil(xs);

      if (xe > clip_x2) {
        if (xs > clip_x2)
          continue;

        ixe = clip_x2 - 1;
      }
      else
        ixe = ceil(xe) - 1;

      dx = ixs - xs;

      rgb = rgbs + drgb*dx;

      for (x = ixs; x <= ixe; ++x, rgb += drgb) {
        setForeground(rgb);

        drawPoint(CIPoint2D(int(x), int(y)));
      }

      xs += dx_left;
      xe += dx_right;

      rgbs += drgb_left;
      rgbe += drgb_right;
    }
  }
}

// Flat Bottom (x2,y2) -> (x3, y3)

void
CPixelRenderer::
fillGouraudTriangleBot(const CPoint2D &point1, CRGBA rgb1, const CPoint2D &point2, CRGBA rgb2,
                       const CPoint2D &point3, CRGBA rgb3)
{
  CPoint2D p1(point1);
  CPoint2D p2(point2);
  CPoint2D p3(point3);

  assert(p2.y == p3.y);

  double iy1, iy3;
  int    clip_x1, clip_y1, clip_x2, clip_y2;

  getPixelClip(&clip_x1, &clip_y1, &clip_x2, &clip_y2);

  if (p1.y < clip_y1)
    iy1 = clip_y1;
  else
    iy1 = ceil(p1.y);

  if (p3.y > clip_y2)
    iy3 = clip_y2 - 1;
  else
    iy3 = ceil(p3.y) - 1;

  if (iy3 < iy1)
    return;

  if (p3.x < p2.x) {
    std::swap(p2.x, p3.x);
    std::swap(rgb2, rgb3);
  }

  double height = p3.y - p1.y;

  double dx_left  = (p2.x - p1.x)/height;
  double dx_right = (p3.x - p1.x)/height;

  CRGBA drgb_left  = (rgb2 - rgb1)/height;
  CRGBA drgb_right = (rgb3 - rgb1)/height;

  double xs = p1.x;
  double xe = p1.x;

  CRGBA rgbs = rgb1;
  CRGBA rgbe = rgb1;

  double dy = iy1 - p1.y;

  xs += dx_left *dy;
  xe += dx_right*dy;

  rgbs += drgb_left *dy;
  rgbe += drgb_right*dy;

  CRGBA  drgb, rgb;
  double ixs, ixe, dx, x, y;

  if (p1.x >= clip_x1 && p1.x <= clip_x2 &&
      p2.x >= clip_x1 && p2.x <= clip_x2 &&
      p3.x >= clip_x1 && p3.x <= clip_x2) {
    for (y = iy1; y <= iy3; ++y) {
      dx = xe - xs;

      drgb = (rgbe - rgbs)/dx;

      ixs = ceil(xs);
      ixe = ceil(xe) - 1;

      dx = ixs - xs;

      rgb = rgbs + drgb*dx;

      for (x = ixs; x <= ixe; ++x, rgb += drgb) {
        setForeground(rgb);

        drawPoint(CIPoint2D(int(x), int(y)));
      }

      xs += dx_left;
      xe += dx_right;

      rgbs += drgb_left;
      rgbe += drgb_right;
    }
  }
  else {
    for (y = iy1; y <= iy3; ++y) {
      dx = xe - xs;

      drgb = (rgbe - rgbs)/dx;

      if (xs < clip_x1) {
        if (xe < clip_x1)
          continue;

        ixs = clip_x1;
      }
      else
        ixs = ceil(xs);

      if (xe > clip_x2) {
        if (xs > clip_x2)
          continue;

        ixe = clip_x2 - 1;
      }
      else
        ixe = ceil(xe) - 1;

      dx = ixs - xs;

      rgb = rgbs + drgb*dx;

      for (x = ixs; x <= ixe; ++x, rgb += drgb) {
        setForeground(rgb);

        drawPoint(CIPoint2D(int(x), int(y)));
      }

      xs += dx_left;
      xe += dx_right;

      rgbs += drgb_left;
      rgbe += drgb_right;
    }
  }
}

void
CPixelRenderer::
textureTriangle(CImagePtr texture,
                const CPoint2D &point1, double tx1, double ty1, CRGBA rgb1,
                const CPoint2D &point2, double tx2, double ty2, CRGBA rgb2,
                const CPoint2D &point3, double tx3, double ty3, CRGBA rgb3)
{
  CPoint2D p1(point1);
  CPoint2D p2(point2);
  CPoint2D p3(point3);

  int clip_x1, clip_y1, clip_x2, clip_y2;

  getPixelClip(&clip_x1, &clip_y1, &clip_x2, &clip_y2);

  if ((p1.x < clip_x1 && p2.x < clip_x1 && p3.x < clip_x1) ||
      (p1.y < clip_y1 && p2.y < clip_y1 && p3.y < clip_y1) ||
      (p1.x > clip_x2 && p2.x > clip_x2 && p3.x > clip_x2) ||
      (p1.y > clip_y2 && p2.y > clip_y2 && p3.y > clip_y2))
    return;

  if ((CMathGen::cmp(p1.x, p2.x) && CMathGen::cmp(p2.x, p3.x)) ||
      (CMathGen::cmp(p1.y, p2.y) && CMathGen::cmp(p2.y, p3.y)))
    return;

  if (p2.y < p1.y) {
    std::swap(  p1,   p2);
    std::swap( tx1,  tx2);
    std::swap( ty1,  ty2);
    std::swap(rgb1, rgb2);
  }

  if (p3.y < p1.y) {
    std::swap(  p1,   p3);
    std::swap( tx1,  tx3);
    std::swap( ty1,  ty3);
    std::swap(rgb1, rgb3);
  }

  if (p3.y < p2.y) {
    std::swap(  p2,   p3);
    std::swap( tx2,  tx3);
    std::swap( ty2,  ty3);
    std::swap(rgb2, rgb3);
  }

  if      (CMathGen::cmp(p1.y, p2.y))
    textureTriangleTop(texture, p1, tx1, ty1, rgb1, p2, tx2, ty2, rgb2, p3, tx3, ty3, rgb3);
  else if (p2.y == p3.y)
    textureTriangleBot(texture, p1, tx1, ty1, rgb1, p2, tx2, ty2, rgb2, p3, tx3, ty3, rgb3);
  else {
    double dy = (p2.y - p1.y)/(p3.y - p1.y);

    double x4  =  p1.x + dy*( p3.x -  p1.x);
    double tx4 = tx1 + dy*(tx3 - tx1);
    double ty4 = ty1 + dy*(ty3 - ty1);

    CRGBA rgb4 = rgb1 + dy*(rgb3 - rgb1);

    CPoint2D p4(x4, p2.y);

    textureTriangleBot(texture, p1, tx1, ty1, rgb1, p4, tx4, ty4, rgb4, p2, tx2, ty2, rgb2);
    textureTriangleTop(texture, p2, tx2, ty2, rgb2, p4, tx4, ty4, rgb4, p3, tx3, ty3, rgb3);
  }
}

// Flat Top (x1,y1) -> (x2, y2)

void
CPixelRenderer::
textureTriangleTop(CImagePtr texture,
                   const CPoint2D &point1, double tx1, double ty1, CRGBA rgb1,
                   const CPoint2D &point2, double tx2, double ty2, CRGBA rgb2,
                   const CPoint2D &point3, double tx3, double ty3, CRGBA rgb3)
{
  CPoint2D p1(point1);
  CPoint2D p2(point2);
  CPoint2D p3(point3);

  assert(p1.y == p2.y);

  double iy1, iy3;
  int    clip_x1, clip_y1, clip_x2, clip_y2;

  getPixelClip(&clip_x1, &clip_y1, &clip_x2, &clip_y2);

  if (p1.y < clip_y1)
    iy1 = clip_y1;
  else
    iy1 = ceil(p1.y);

  if (p3.y > clip_y2)
    iy3 = clip_y2 - 1;
  else
    iy3 = ceil(p3.y) - 1;

  if (iy3 < iy1)
    return;

  if (p2.x < p1.x) {
    std::swap(p1.x, p2.x);
    std::swap( tx1,  tx2);
    std::swap( ty1,  ty2);
    std::swap(rgb1, rgb2);
  }

  double height = p3.y - p1.y;

  double dx_left  = (p3.x - p1.x)/height;
  double dx_right = (p3.x - p2.x)/height;

  double dtx_left  = (tx3 - tx1)/height;
  double dty_left  = (ty3 - ty1)/height;
  double dtx_right = (tx3 - tx2)/height;
  double dty_right = (ty3 - ty2)/height;

  CRGBA drgb_left  = (rgb3 - rgb1)/height;
  CRGBA drgb_right = (rgb3 - rgb2)/height;

  double xs = p1.x;
  double xe = p2.x;

  double txs = tx1;
  double tys = ty1;
  double txe = tx2;
  double tye = ty2;

  CRGBA rgbs = rgb1;
  CRGBA rgbe = rgb2;

  double dy = iy1 - p1.y;

  xs += dx_left *dy;
  xe += dx_right*dy;

  txs += dtx_left *dy;
  tys += dty_left *dy;
  txe += dtx_right*dy;
  tye += dty_right*dy;

  rgbs += drgb_left *dy;
  rgbe += drgb_right*dy;

  CRGBA  trgb, drgb, rgb;
  double ixs, ixe, dtx, dty, dx, tx, ty, x, y;

  if (p1.x >= clip_x1 && p1.x <= clip_x2 &&
      p2.x >= clip_x1 && p2.x <= clip_x2 &&
      p3.x >= clip_x1 && p3.x <= clip_x2) {
    for (y = iy1; y <= iy3; ++y) {
      dx = xe - xs;

      dtx = (txe - txs)/dx;
      dty = (tye - tys)/dx;

      drgb = (rgbe - rgbs)/dx;

      ixs = ceil(xs);
      ixe = ceil(xe) - 1;

      dx = ixs - xs;

      tx = txs + dtx*dx;
      ty = tys + dty*dx;

      rgb = rgbs + drgb*dx;

      for (x = ixs; x <= ixe; ++x, tx += dtx, ty += dty, rgb += drgb) {
        tx = std::min(std::max(tx, 0.0), 1.0);
        ty = std::min(std::max(ty, 0.0), 1.0);

        texture->sampleNearest(tx, ty, trgb);

        setForeground(trgb*rgb);

        drawPoint(CIPoint2D(int(x), int(y)));
      }

      xs += dx_left;
      xe += dx_right;

      txs += dtx_left;
      tys += dty_left;
      txe += dtx_right;
      tye += dty_right;

      rgbs += drgb_left;
      rgbe += drgb_right;
    }
  }
  else {
    for (y = iy1; y <= iy3; ++y) {
      dx = xe - xs;

      dtx = (txe - txs)/dx;
      dty = (tye - tys)/dx;

      drgb = (rgbe - rgbs)/dx;

      if (xs < clip_x1) {
        if (xe < clip_x1)
          continue;

        ixs = clip_x1;
      }
      else
        ixs = ceil(xs);

      if (xe > clip_x2) {
        if (xs > clip_x2)
          continue;

        ixe = clip_x2 - 1;
      }
      else
        ixe = ceil(xe) - 1;

      dx = ixs - xs;

      tx = txs + dtx*dx;
      ty = tys + dty*dx;

      rgb = rgbs + drgb*dx;

      for (x = ixs; x <= ixe; ++x, tx += dtx, ty += dty, rgb += drgb) {
        tx = std::min(std::max(tx, 0.0), 1.0);
        ty = std::min(std::max(ty, 0.0), 1.0);

        texture->sampleNearest(tx, ty, trgb);

        setForeground(trgb*rgb);

        drawPoint(CIPoint2D(int(x), int(y)));
      }

      xs += dx_left;
      xe += dx_right;

      txs += dtx_left;
      tys += dty_left;
      txe += dtx_right;
      tye += dty_right;

      rgbs += drgb_left;
      rgbe += drgb_right;
    }
  }
}

// Flat Bottom (x2,y2) -> (x3, y3)

void
CPixelRenderer::
textureTriangleBot(CImagePtr texture,
                   const CPoint2D &point1, double tx1, double ty1, CRGBA rgb1,
                   const CPoint2D &point2, double tx2, double ty2, CRGBA rgb2,
                   const CPoint2D &point3, double tx3, double ty3, CRGBA rgb3)
{
  CPoint2D p1(point1);
  CPoint2D p2(point2);
  CPoint2D p3(point3);

  assert(p2.y == p3.y);

  double iy1, iy3;
  int    clip_x1, clip_y1, clip_x2, clip_y2;

  getPixelClip(&clip_x1, &clip_y1, &clip_x2, &clip_y2);

  if (p1.y < clip_y1)
    iy1 = clip_y1;
  else
    iy1 = ceil(p1.y);

  if (p3.y > clip_y2)
    iy3 = clip_y2 - 1;
  else
    iy3 = ceil(p3.y) - 1;

  if (iy3 < iy1)
    return;

  if (p3.x < p2.x) {
    std::swap(p2.x, p3.x);
    std::swap( tx2,  tx3);
    std::swap( ty2,  ty3);
    std::swap(rgb2, rgb3);
  }

  double height = p3.y - p1.y;

  double dx_left  = (p2.x - p1.x)/height;
  double dx_right = (p3.x - p1.x)/height;

  double dtx_left  = (tx2 - tx1)/height;
  double dty_left  = (ty2 - ty1)/height;
  double dtx_right = (tx3 - tx1)/height;
  double dty_right = (ty3 - ty1)/height;

  CRGBA drgb_left  = (rgb2 - rgb1)/height;
  CRGBA drgb_right = (rgb3 - rgb1)/height;

  double xs = p1.x;
  double xe = p1.x;

  double txs = tx1;
  double tys = ty1;
  double txe = tx1;
  double tye = ty1;

  CRGBA rgbs = rgb1;
  CRGBA rgbe = rgb1;

  double dy = iy1 - p1.y;

  xs += dx_left *dy;
  xe += dx_right*dy;

  txs += dtx_left *dy;
  tys += dty_left *dy;
  txe += dtx_right*dy;
  tye += dty_right*dy;

  rgbs += drgb_left *dy;
  rgbe += drgb_right*dy;

  CRGBA  trgb, drgb, rgb;
  double ixs, ixe, dtx, dty, dx, tx, ty, x, y;

  if (p1.x >= clip_x1 && p1.x <= clip_x2 &&
      p2.x >= clip_x1 && p2.x <= clip_x2 &&
      p3.x >= clip_x1 && p3.x <= clip_x2) {
    for (y = iy1; y <= iy3; ++y) {
      dx = xe - xs;

      dtx = (txe - txs)/dx;
      dty = (tye - tys)/dx;

      drgb = (rgbe - rgbs)/dx;

      ixs = ceil(xs);
      ixe = ceil(xe) - 1;

      dx = ixs - xs;

      tx = txs + dtx*dx;
      ty = tys + dty*dx;

      rgb = rgbs + drgb*dx;

      for (x = ixs; x <= ixe; ++x, tx += dtx, ty += dty, rgb += drgb) {
        tx = std::min(std::max(tx, 0.0), 1.0);
        ty = std::min(std::max(ty, 0.0), 1.0);

        texture->sampleNearest(tx, ty, trgb);

        setForeground(trgb*rgb);

        drawPoint(CIPoint2D(int(x), int(y)));
      }

      xs += dx_left;
      xe += dx_right;

      txs += dtx_left;
      tys += dty_left;
      txe += dtx_right;
      tye += dty_right;

      rgbs += drgb_left;
      rgbe += drgb_right;
    }
  }
  else {
    for (y = iy1; y <= iy3; ++y) {
      dx = xe - xs;

      dtx = (txe - txs)/dx;
      dty = (tye - tys)/dx;

      drgb = (rgbe - rgbs)/dx;

      if (xs < clip_x1) {
        if (xe < clip_x1)
          continue;

        ixs = clip_x1;
      }
      else
        ixs = ceil(xs);

      if (xe > clip_x2) {
        if (xs > clip_x2)
          continue;

        ixe = clip_x2 - 1;
      }
      else
        ixe = ceil(xe) - 1;

      dx = ixs - xs;

      tx = txs + dtx*dx;
      ty = tys + dty*dx;

      rgb = rgbs + drgb*dx;

      for (x = ixs; x <= ixe; ++x, tx += dtx, ty += dty, rgb += drgb) {
        tx = std::min(std::max(tx, 0.0), 1.0);
        ty = std::min(std::max(ty, 0.0), 1.0);

        texture->sampleNearest(tx, ty, trgb);

        setForeground(trgb*rgb);

        drawPoint(CIPoint2D(int(x), int(y)));
      }

      xs += dx_left;
      xe += dx_right;

      txs += dtx_left;
      tys += dty_left;
      txe += dtx_right;
      tye += dty_right;

      rgbs += drgb_left;
      rgbe += drgb_right;
    }
  }
}

//---------------------

void
CPixelRenderer::
getPixelClip(int *clip_x1, int *clip_y1, int *clip_x2, int *clip_y2)
{
  *clip_x1 = 0;
  *clip_y1 = 0;
  *clip_x2 = getWidth () - 1;
  *clip_y2 = getHeight() - 1;
}

//---------------------

CPixelRenderer *
CPixelRenderer::
createImageRenderer() const
{
  return new CImagePixelRenderer;
}

//---------------------

void
CPixelRendererPath::
initPolygons()
{
  ipoly_points_list_.clear();
  rpoly_points_list_.clear();
}

void
CPixelRendererPath::
addPathPolygon(const IPointList &points)
{
  IPointList *poly_points = nextIPolygon();

  uint num_points = points.size();

  for (uint i = 0; i < num_points; ++i)
    poly_points->push_back(points[i]);
}

void
CPixelRendererPath::
addPathPolygon(const RPointList &points)
{
  RPointList *poly_points = nextRPolygon();

  uint num_points = points.size();

  for (uint i = 0; i < num_points; ++i)
    poly_points->push_back(points[i]);
}

CPixelRendererPath::IPointList *
CPixelRendererPath::
nextIPolygon()
{
  uint num_poly_points = ipoly_points_list_.size();

  ipoly_points_list_.push_back(new IPointList);

  IPointList *poly_points = ipoly_points_list_[num_poly_points];

  poly_points->clear();

  return poly_points;
}

CPixelRendererPath::RPointList *
CPixelRendererPath::
nextRPolygon()
{
  uint num_poly_points = rpoly_points_list_.size();

  rpoly_points_list_.push_back(new RPointList);

  RPointList *poly_points = rpoly_points_list_[num_poly_points];

  poly_points->clear();

  return poly_points;
}
