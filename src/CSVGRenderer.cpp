#include <CSVGRenderer.h>
#include <CSVGImageData.h>

namespace {
  CSVGImageData *createImage(int w, int h) {
    auto *dst_image = new CSVGImageData();

    dst_image->setSize(w, h);

    dst_image->clear();

    return dst_image;
  }
}

//-----

CSVGRenderer::
CSVGRenderer()
{
}

CSVGRenderer::
~CSVGRenderer()
{
}

void
CSVGRenderer::
combine(int ix, int iy, CSVGRenderer *r)
{
  int iwidth1, iheight1, iwidth2, iheight2;

     getSize(&iwidth1, &iheight1);
  r->getSize(&iwidth2, &iheight2);

  int w = std::max(iwidth1 , ix + iwidth2 );
  int h = std::max(iheight1, iy + iheight2);

  auto *image1 = getImage();
  auto *image2 = r->getImage();

  if (r->opacity() < 1) {
    if (image2)
      image2->scaleAlpha(r->opacity());
  }

  if (w > iwidth1 || h > iheight1) {
    auto *image3 = createImage(w, h);

    if (image3) {
      image3->combine( 0,  0, image1);
      image3->combine(ix, iy, image2);

      setImage(image3);
    }
  }
  else {
    if (image1) {
      image1->combine(ix, iy, image2);

      setImage(image1);
    }
  }
}

void
CSVGRenderer::
addResizedImage(CSVGRenderer *src, double x, double y, double w, double h)
{
  auto *image2 = src->getImage();

  image2->reshape(w, h);

  auto *image1 = getImage();

  // TODO: handle out of bounds
  image1->combine(x, y, image2);

  setImage(image1);
}

void
CSVGRenderer::
addClippedImage(CSVGRenderer *src, int x, int y, int px1, int py1, int px2, int py2)
{
  auto *image = src->getImage();

  image->clipOutside(px1, py1, px2, py2);

  addImage(x - px1, y - py1, image);
}

void
CSVGRenderer::
setClippedImage(CSVGRenderer *src, int px1, int py1, int px2, int py2)
{
  auto *image = src->getImage();

  image->clipOutside(px1, py1, px2, py2);

  setImage(image);
}

void
CSVGRenderer::
addImage(int x, int y, CSVGImageData *image2)
{
  auto *image1 = getImage();

  image1->combine(x, y, image2);

  setImage(image1);
}

void
CSVGRenderer::
setOffsetImage(CSVGRenderer *src, int dx, int dy)
{
  auto *src_image = src->getImage();

  int w = src_image->getWidth () + dx;
  int h = src_image->getHeight() + dy;

  auto *dst_image = createImage(w, h);

  src_image->subCopyTo(dst_image, 0, 0, -1, -1, dx, dy);

  setImage(dst_image);
}

void
CSVGRenderer::
gaussianBlur(CSVGRenderer *dst, CBBox2D &bbox, double stdDevX, double stdDevY)
{
  auto *src_image = getImage();

  // set alpha
  if (isAlpha())
    src_image->setAlphaGray(0);

  auto *dst_image = src_image->dup();

  if (bbox.isSet()) {
    CPoint2D p1, p2;

    windowToPixel(bbox.getLL(), p1);
    windowToPixel(bbox.getUR(), p2);

    src_image->setWindow(p1.x, p1.y, p2.x, p2.y);
  }

  src_image->gaussianBlur(dst_image, stdDevX, stdDevY);

  dst->setImage(dst_image);
}

void
CSVGRenderer::
blend(CSVGRenderer *in2, CSVGBlendMode mode, CSVGRenderer *dst)
{
  auto *src_image1 =      getImage();
  auto *src_image2 = in2->getImage();

  // set alpha
  if (     isAlpha()) src_image1->setAlphaGray(0);
  if (dst->isAlpha()) src_image2->setAlphaGray(0);

  auto *dst_image = src_image1->dup();

  CRGBABlendMode mode1 = CRGBA_BLEND_NORMAL;

  if      (mode == CSVGBlendMode::NORMAL)
    mode1 = CRGBA_BLEND_NORMAL;
  else if (mode == CSVGBlendMode::MULTIPLY)
    mode1 = CRGBA_BLEND_MULTIPLY;
  else if (mode == CSVGBlendMode::SCREEN)
    mode1 = CRGBA_BLEND_SCREEN;
  else if (mode == CSVGBlendMode::DARKEN)
    mode1 = CRGBA_BLEND_DARKEN;
  else if (mode == CSVGBlendMode::LIGHTEN)
    mode1 = CRGBA_BLEND_LIGHTEN;

  dst_image->combine(src_image2, mode1);

  dst->setImage(dst_image);
}
