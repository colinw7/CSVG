#include <CImagePixelRenderer.h>
#include <CImageLib.h>

CImagePixelRenderer::
CImagePixelRenderer(int width, int height) :
 CPixelRenderer()
{
  image_ = CImageMgrInst->createImage();

  image_->setDataSize(width, height);
}

CImagePixelRenderer::
CImagePixelRenderer(CImagePtr image) :
 CPixelRenderer(), image_(image)
{
}

CImagePixelRenderer::
~CImagePixelRenderer()
{
  static uint id = 1;

  if (getenv("PIXEL_RENDERER_WRITE_IMAGE"))
    writePNG(CStrUtil::strprintf("CImagePixelRenderer%u.png", id++));
}

CImagePixelRenderer *
CImagePixelRenderer::
dup() const
{
  return new CImagePixelRenderer(getWidth(), getHeight());
}

void
CImagePixelRenderer::
setImage(CImagePtr image)
{
  image_ = image;
}

uint
CImagePixelRenderer::
getWidth() const
{
  return image_->getWidth();
}

uint
CImagePixelRenderer::
getHeight() const
{
  return image_->getHeight();
}

void
CImagePixelRenderer::
updateSize(int width, int height)
{
  if (width  != (int) image_->getWidth () || height != (int) image_->getHeight())
    image_->setDataSize(width, height);
}

void
CImagePixelRenderer::
clear()
{
  CRGBA bg;

  getBackground(bg);

  image_->setRGBAData(bg);
}

void
CImagePixelRenderer::
fill()
{
  CRGBA fg;

  getForeground(fg);

  image_->setRGBAData(fg);
}

void
CImagePixelRenderer::
drawClippedPoint(const CIPoint2D &point)
{
  CRGBA fg;

  getForeground(fg);

  image_->drawRGBAPoint(point.x, point.y, fg);
}

void
CImagePixelRenderer::
drawClippedImage(const CIPoint2D &point, CImagePtr image)
{
  image->copyTo(image_, point.x, point.y);
}

void
CImagePixelRenderer::
writePNG(const std::string &filename)
{
  image_->write(filename, CFILE_TYPE_IMAGE_PNG);
}
