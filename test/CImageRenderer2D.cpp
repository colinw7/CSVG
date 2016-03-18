#include <CImageRenderer2D.h>
#include <CImagePixelRenderer.h>
#include <CImageLib.h>

CImageRenderer2D::
CImageRenderer2D(uint width, uint height) :
 CRenderer2D()
{
  ipixel_renderer_ = new CImagePixelRenderer(width, height);

  setPixelRenderer(ipixel_renderer_);

  setDataRange(0, height - 1, width - 1, 0);
}

CImageRenderer2D::
CImageRenderer2D(CImagePtr image) :
 CRenderer2D(NULL)
{
  ipixel_renderer_ = new CImagePixelRenderer(image);

  setPixelRenderer(ipixel_renderer_);

  setDataRange(0, image->getHeight() - 1, image->getWidth() - 1, 0);
}

CImageRenderer2D::
~CImageRenderer2D()
{
  static uint id = 1;

  if (getenv("PIXEL_RENDERER_WRITE_IMAGE"))
    writePNG(CStrUtil::strprintf("CImageRenderer2D%u.png", id++));
}

CImageRenderer2D *
CImageRenderer2D::
dup() const
{
  return new CImageRenderer2D(ipixel_renderer_->getWidth(), ipixel_renderer_->getHeight());
}

CImagePixelRenderer *
CImageRenderer2D::
getPixelRenderer() const
{
  return ipixel_renderer_;
}

void
CImageRenderer2D::
writePNG(const std::string &filename)
{
  getPixelRenderer()->writePNG(filename);
}

void
CImageRenderer2D::
updateSize(int width, int height)
{
  getPixelRenderer()->updateSize(width, height);
}

CImagePtr
CImageRenderer2D::
getImage()
{
  return getPixelRenderer()->getImage();
}

void
CImageRenderer2D::
drawLine(const CPoint2D &point1, const CPoint2D &point2)
{
  CRenderer2D::drawLine(point1, point2);
}

void
CImageRenderer2D::
fillPolygon(const PointList &points)
{
  CRenderer2D::fillPolygon(points);
}

void
CImageRenderer2D::
drawPolygon(const PointList &points)
{
  CRenderer2D::drawPolygon(points);
}

void
CImageRenderer2D::
drawText(const CPoint2D &point, const std::string &str)
{
  CRenderer2D::drawText(point, str);
}

void
CImageRenderer2D::
fillText(const CPoint2D &point, const std::string &str)
{
  CRenderer2D::fillText(point, str);
}

void
CImageRenderer2D::
drawImage(const CPoint2D &point, CImagePtr image)
{
  CRenderer2D::drawImage(point, image);
}

void
CImageRenderer2D::
drawAlphaImage(const CPoint2D &point, CImagePtr image)
{
  CRenderer2D::drawAlphaImage(point, image);
}

void
CImageRenderer2D::
drawImageInBox(const CBBox2D &point, CImagePtr image)
{
  CRenderer2D::drawImageInBox(point, image);
}

void
CImageRenderer2D::
drawAlphaImageInBox(const CBBox2D &point, CImagePtr image)
{
  CRenderer2D::drawAlphaImageInBox(point, image);
}

void
CImageRenderer2D::
drawPoint(const CPoint2D &point)
{
  CRenderer2D::drawPoint(point);
}

void
CImageRenderer2D::
fillRectangle(const CBBox2D &bbox)
{
  CRenderer2D::fillRectangle(bbox);
}

void
CImageRenderer2D::
gradientFillRectangle(const CBBox2D &bbox, CGenGradient *gradient)
{
  CRenderer2D::gradientFillRectangle(bbox, gradient);
}

void
CImageRenderer2D::
drawRectangle(const CBBox2D &bbox)
{
  CRenderer2D::drawRectangle(bbox);
}
