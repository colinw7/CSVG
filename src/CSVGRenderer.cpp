#include <CSVGRenderer.h>

namespace {
  CImagePtr createImage(int w, int h) {
    CImageNoSrc src;

    CImagePtr dst_image = CImageMgrInst->createImage(src);

    dst_image->setDataSize(w, h);

    dst_image->setRGBAData(CRGBA(0,0,0,0));

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

  CImagePtr image1 = getImage();
  CImagePtr image2 = r->getImage();

  if (r->opacity() < 1)
    image2->scaleAlpha(r->opacity());

  if (w > iwidth1 || h > iheight1) {
    CImagePtr image3 = createImage(w, h);

    image3->combine(image1);
    image3->combine(ix, iy, image2);

    setImage(image3);
  }
  else {
    image1->combine(ix, iy, image2);

    setImage(image1);
  }
}

void
CSVGRenderer::
addResizedImage(CSVGRenderer *src, int x, int y, int w, int h)
{
  CImagePtr image2 = src->getImage();

  image2->reshape(w, h);

  CImagePtr image1 = getImage();

  // TODO: handle out of bounds
  image1->combine(x, y, image2);

  setImage(image1);
}

void
CSVGRenderer::
addClippedImage(CSVGRenderer *src, int x, int y, int px1, int py1, int px2, int py2)
{
  CImagePtr image = src->getImage();

  image->clipOutside(px1, py1, px2, py2);

  addImage(x - px1, y - py1, image);
}

void
CSVGRenderer::
setClippedImage(CSVGRenderer *src, int px1, int py1, int px2, int py2)
{
  CImagePtr image = src->getImage();

  image->clipOutside(px1, py1, px2, py2);

  setImage(image);
}

void
CSVGRenderer::
addImage(int x, int y, CImagePtr image2)
{
  CImagePtr image1 = getImage();

  image1->combine(x, y, image2);

  setImage(image1);
}

void
CSVGRenderer::
setOffsetImage(CSVGRenderer *src, int dx, int dy)
{
  CImagePtr src_image = src->getImage();

  int w = src_image->getWidth () + dx;
  int h = src_image->getHeight() + dy;

  CImagePtr dst_image = createImage(w, h);

  dst_image->subCopyFrom(src_image, 0, 0, -1, -1, dx, dy);

  setImage(dst_image);
}

void
CSVGRenderer::
gaussianBlur(CSVGRenderer *dst, CBBox2D &bbox, double stdDevX, double stdDevY)
{
  CImagePtr src_image = getImage();

  // set alpha
  if (isAlpha())
    src_image->setAlphaGray(0);

  CImagePtr dst_image = src_image->dup();

  if (bbox.isSet())
    src_image->setWindow(bbox.getXMin(), bbox.getYMin(), bbox.getXMax(), bbox.getYMax());

  src_image->gaussianBlur(dst_image, stdDevX, stdDevY);

  dst->setImage(dst_image);
}
