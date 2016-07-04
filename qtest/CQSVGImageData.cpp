#include <CQSVGImageData.h>
#include <CTurbulenceUtil.h>
#include <CGaussianBlur.h>
#include <QColor>

CQSVGImageData::
CQSVGImageData() :
 locked_(false)
{
}

CQSVGImageData::
CQSVGImageData(const CQSVGImageData &data) :
 CSVGImageData(data)
{
  qimage_ = data.qimage_;
}

CSVGImageData *
CQSVGImageData::
dup() const
{
  return new CQSVGImageData(*this);
}

CImagePtr
CQSVGImageData::
image() const
{
  assert(false);

  return CSVGImageData::image();
}

void
CQSVGImageData::
setImage(CImagePtr &image)
{
  assert(false);

  CSVGImageData::setImage(image);
}

void
CQSVGImageData::
setQImage(const QImage &qimage)
{
  assert(! locked_);

  qimage_ = qimage;
}

void
CQSVGImageData::
setSize(int w, int h)
{
  if (w < 1) w= 1;
  if (h < 1) h= 1;

  if (w != qimage_.width() || h != qimage_.height()) {
    assert(! locked_);

    qimage_ = QImage(QSize(w, h), QImage::Format_ARGB32);

    qimage_.fill(0);
  }
}

int
CQSVGImageData::
getWidth() const
{
  return qimage_.width();
}

int
CQSVGImageData::
getHeight() const
{
  return qimage_.height();
}

bool
CQSVGImageData::
read(const std::string &filename)
{
  return qimage_.load(filename.c_str());
}

bool
CQSVGImageData::
write(const std::string &filename, CFileType type)
{
  if (type == CFILE_TYPE_IMAGE_PNG)
    return qimage_.save(filename.c_str(), "PNG");
  else
    return qimage_.save(filename.c_str());
}

void
CQSVGImageData::
clear()
{
  qimage_.fill(0);
}

void
CQSVGImageData::
set(const CRGBA &rgba)
{
  qimage_.fill(QColor(rgba.getRedI(), rgba.getGreenI(), rgba.getBlueI(), rgba.getAlphaI()));
}

void
CQSVGImageData::
setAlphaGray(double gray)
{
  int i = 0;

  for (int y = 0; y < getHeight(); ++y) {
    for (int x = 0; x < getWidth(); ++x, ++i) {
      CRGBA rgba = getPixel(x, y);

      CRGBA rgba1(gray, gray, gray, rgba.getAlpha());

      setPixel(x, y, rgba1);
    }
  }
}

CRGBA
CQSVGImageData::
getPixel(int x, int y) const
{
  //assert(validPixel(x, y));

  QRgb rgba = qimage_.pixel(x, y);

  return CRGBA(qRed(rgba)/255.0, qGreen(rgba)/255.0, qBlue(rgba)/255.0, qAlpha(rgba)/255.0);
}

void
CQSVGImageData::
getPixel(int x, int y, double *r, double *g, double *b, double *a) const
{
  QRgb rgba = qimage_.pixel(x, y);

  *r = qRed  (rgba)/255.0;
  *g = qGreen(rgba)/255.0;
  *b = qBlue (rgba)/255.0;
  *a = qAlpha(rgba)/255.0;
}

void
CQSVGImageData::
setPixel(int x, int y, const CRGBA &c)
{
  //assert(validPixel(x, y));

  QRgb rgba = qRgba(c.getRedI(), c.getGreenI(), c.getBlueI(), c.getAlphaI());

  qimage_.setPixel(x, y, rgba);
}

//--------------------

void
CQSVGImageData::
subCopyTo(CSVGImageData *dst, int src_x, int src_y, int width, int height, int dst_x, int dst_y)
{
  CQSVGImageData *qdst = dynamic_cast<CQSVGImageData *>(dst);
  assert(qdst);

  int src_width  = getWidth ();
  int src_height = getHeight();

  int dst_width  = qdst->getWidth ();
  int dst_height = qdst->getHeight();

  if (width  < 0) width  = src_width;
  if (height < 0) height = src_height;

  int width1  = width;
  int height1 = height;

  if (src_x +  width1 > src_width )  width1 = src_width  - src_x;
  if (src_y + height1 > src_height) height1 = src_height - src_y;
  if (dst_x +  width1 > dst_width )  width1 = dst_width  - dst_x;
  if (dst_y + height1 > dst_height) height1 = dst_height - dst_y;

  int ys = src_y;
  int yd = dst_y;

  for (int y = 0; y < height1; ++y, ++ys, ++yd) {
    int xs = src_x;
    int xd = dst_x;

    for (int x = 0; x < width1; ++x, ++xs, ++xd) {
      CRGBA rgba;

      if (validPixel(xs, ys))
        rgba = getPixel(xs, ys);
      else
        rgba = CRGBA(0,0,0,0);

      if (qdst->validPixel(xd, yd))
        qdst->setPixel(xd, yd, rgba);
    }
  }
}

CSVGImageData *
CQSVGImageData::
subImage(int x, int y, int width, int height)
{
  if (width < 0)
    width = getWidth() - x;

  if (height < 0)
    height = getHeight() - y;

  CSVGImageData *image = dup();

  image->setSize(width, height);

  subCopyTo(image, x, y, width, height, 0, 0);

  return image;
}

void
CQSVGImageData::
copyAlpha(CSVGImageData *dst, int x, int y)
{
  CQSVGImageData *qdst = dynamic_cast<CQSVGImageData *>(dst);
  assert(qdst);

  int iwidth  = getWidth ();
  int iheight = getHeight();

  int iwidth1  = qdst->getWidth ();
  int iheight1 = qdst->getHeight();

  int x1 = x;
  int x2 = std::min(x + iwidth1  - 1, iwidth  - 1);
  int y1 = y;
  int y2 = std::min(y + iheight1 - 1, iheight - 1);

  // image is mask, get alpha from rgba of mask set alpha of image pixel from mask
  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      CRGBA rgba1 = getPixel(x, y);

      int xx = x - x1;
      int yy = y - y1;

      CRGBA rgba2 = qdst->getPixel(xx, yy);

      double a = rgba2.getAlpha();

      rgba1.setAlpha(rgba1.getAlpha()*a);

      setPixel(x, y, rgba1);
    }
  }
}

void
CQSVGImageData::
scaleAlpha(double alpha)
{
  for (int y = 0; y < getHeight(); ++y) {
    for (int x = 0; x < getWidth(); ++x) {
      CRGBA rgba = getPixel(x, y);

      rgba.setAlpha(alpha*rgba.getAlpha());

      setPixel(x, y, rgba);
    }
  }
}

void
CQSVGImageData::
reshape(int width, int height)
{
  CSVGImageData *image = dup();

  image->setSize(width, height);

  //------

  CImageResizeType resize_type = CIMAGE_RESIZE_NEAREST;

  if      (resize_type == CIMAGE_RESIZE_BILINEAR)
    reshapeBilinear(image);
  else if (resize_type == CIMAGE_RESIZE_AVERAGE)
    reshapeAverage(image);
  else if (resize_type == CIMAGE_RESIZE_NEAREST)
    reshapeNearest(image);
  else
    reshapeNearest(image);

  assert(! locked_);

  CQSVGImageData *qdata = dynamic_cast<CQSVGImageData *>(image);
  assert(qdata);

  qimage_ = qdata->qimage_;

  delete image;
}

void
CQSVGImageData::
reshapeBilinear(CSVGImageData *new_image) const
{
  int width1  = getWidth ();
  int height1 = getHeight();

  int width2  = new_image->getWidth ();
  int height2 = new_image->getHeight();

  if (width2 <= 0 || height2 <= 0)
    return;

  double ix = (1.0*width1 )/width2 ;
  double iy = (1.0*height1)/height2;

  double yy = 0.0;

  for (int y = 0; y < height2; ++y, yy += iy) {
    double xx = 0.0;

    int y1 = CMathGen::RoundDown(yy);
    int y2 = CMathGen::RoundUp  (yy);

    if (y2 >= height1) y2 = height1 - 1;

    for (int x = 0; x < width2; ++x, xx += ix) {
      int x1 = CMathGen::RoundDown(xx);
      int x2 = CMathGen::RoundUp  (xx);

      if (x2 >= width1) x2 = width1 - 1;

      CRGBA c = getBilinearPixel(xx, x1, x2, yy, y1, y2);

      new_image->setPixel(x, y, c);
    }
  }
}

CRGBA
CQSVGImageData::
getBilinearPixel(double xx, double yy) const
{
  int width1  = getWidth ();
  int height1 = getHeight();

  int x1 = CMathGen::RoundDown(xx);
  int x2 = CMathGen::RoundUp  (xx);

  if (x1 <  0     ) x1 = 0;
  if (x2 >= width1) x2 = width1 - 1;

  int y1 = CMathGen::RoundDown(yy);
  int y2 = CMathGen::RoundUp  (yy);

  if (y1 <  0      ) y2 = 0;
  if (y2 >= height1) y2 = height1 - 1;

  return getBilinearPixel(xx, x1, x2, yy, y1, y2);
}

CRGBA
CQSVGImageData::
getBilinearPixel(double xx, int x1, int x2, double yy, int y1, int y2) const
{
  if (x1 != x2 || y1 != y2) {
    double r1, g1, b1, a1;
    double r2, g2, b2, a2;
    double r3, g3, b3, a3;
    double r4, g4, b4, a4;

    getPixel(x1, y2, &r1, &g1, &b1, &a1);
    getPixel(x2, y2, &r2, &g2, &b2, &a2);
    getPixel(x1, y1, &r3, &g3, &b3, &a3);
    getPixel(x2, y1, &r4, &g4, &b4, &a4);

    double dx  = x2 - xx;
    double dx1 = 1.0 - dx;
    double dy  = yy - y1;
    double dy1 = 1.0 - dy;

    double r12 = dx*r1 + dx1*r2;
    double r34 = dx*r3 + dx1*r4;

    double r = (dy*r12 + dy1*r34);

    double g12 = dx*g1 + dx1*g2;
    double g34 = dx*g3 + dx1*g4;

    double g = (dy*g12 + dy1*g34);

    double b12 = dx*b1 + dx1*b2;
    double b34 = dx*b3 + dx1*b4;

    double b = (dy*b12 + dy1*b34);

    double a12 = dx*a1 + dx1*a2;
    double a34 = dx*a3 + dx1*a4;

    double a = (dy*a12 + dy1*a34);

    return CRGBA(r, g, b, a);
  }
  else {
    double r, g, b, a;

    getPixel(x1, y1, &r, &g, &b, &a);

    return CRGBA(r, g, b, a);
  }
}

void
CQSVGImageData::
reshapeAverage(CSVGImageData *new_image) const
{
  int width1  = getWidth ();
  int height1 = getHeight();

  int width2  = new_image->getWidth ();
  int height2 = new_image->getHeight();

  if (width2 <= 0 || height2 <= 0)
    return;

  double dx = (1.0*width1 )/width2 ;
  double dy = (1.0*height1)/height2;

  int xx1, yy1, xx2, yy2;

  double y1 = 0.0;
  double y2 = dy;

  for (int y = 0; y < height2; ++y, y1 = y2, y2 += dy) {
    yy1 = std::min(std::max(CMathGen::Round(y1), 0), height1 - 1);
    yy2 = std::min(std::max(CMathGen::Round(y2), 0), height1 - 1);

    double x1 = 0.0;
    double x2 = dx;

    for (int x = 0; x < width2; ++x, x1 = x2, x2 += dx) {
      xx1 = std::min(std::max(CMathGen::Round(x1), 0), width1 - 1);
      xx2 = std::min(std::max(CMathGen::Round(x2), 0), width1 - 1);

      CRGBA rgba;

      for (int yy = yy1; yy <= yy2; ++yy) {
        for (int xx = xx1; xx <= xx2; ++xx) {
          CRGBA rgba1 = getPixel(xx, yy);

          rgba += rgba1;
        }
      }

      int n = (xx2 - xx1 + 1)*(yy2 - yy1 + 1);

      rgba /= n;

      new_image->setPixel(x, y, rgba);
    }
  }
}

void
CQSVGImageData::
reshapeNearest(CSVGImageData *new_image) const
{
  int width1  = getWidth ();
  int height1 = getHeight();

  if (width1 <= 0 || height1 <= 0)
    return;

  int width2  = new_image->getWidth ();
  int height2 = new_image->getHeight();

  if (width2 <= 0 || height2 <= 0)
    return;

  double dx = (1.0*width1 )/width2 ;
  double dy = (1.0*height1)/height2;

  double y1 = 0.0;

  for (int y = 0; y < height2; ++y, y1 += dy) {
    double x1 = 0.0;

    for (int x = 0; x < width2; ++x, x1 += dx) {
      int x2 = std::min(std::max(int(x1), 0), width1  - 1);
      int y2 = std::min(std::max(int(y1), 0), height1 - 1);

      CRGBA rgba = getPixel(x2, y2);

      new_image->setPixel(x, y, rgba);
    }
  }
}

void
CQSVGImageData::
reshapeKeepAspect(int width, int height)
{
  if (getWidth() <= 0 || getHeight() <= 0)
    return;

  double xfactor = (1.0*width )/getWidth ();
  double yfactor = (1.0*height)/getHeight();

  if (xfactor < yfactor)
    reshapeWidth(width);
  else
    reshapeHeight(height);
}

void
CQSVGImageData::
reshapeWidth(int width)
{
  if (getWidth() <= 0)
    return;

  int height = (width*getHeight())/getWidth();

  reshape(width, height);
}

void
CQSVGImageData::
reshapeHeight(int height)
{
  if (getHeight() <= 0)
    return;

  int width = (height*getWidth())/getHeight();

  reshape(width, height);
}

void
CQSVGImageData::
clipOutside(int x1, int y1, int x2, int y2)
{
  CRGBA a(0,0,0,0);

  for (int y = 0; y < getHeight(); ++y) {
    for (int x = 0; x < getWidth(); ++x) {
      if (y >= y1 && y <= y2 && x >= x1 && x <= x2)
        continue;

      setPixel(x, y, a);
    }
  }
}

void
CQSVGImageData::
combine(CSVGImageData *in, CRGBABlendMode mode)
{
  CQSVGImageData *image = dynamic_cast<CQSVGImageData *>(in);
  assert(image);

  int w = std::min(getWidth (), image->getWidth ());
  int h = std::min(getHeight(), image->getHeight());

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba1 =        getPixel(x, y);
      CRGBA rgba2 = image->getPixel(x, y);

      CRGBA rgba = CRGBA::blendCombine(rgba1, rgba2, mode);

      setPixel(x, y, rgba.clamp());
    }
  }
}

void
CQSVGImageData::
combine(CSVGImageData *in, const CRGBACombineDef &def)
{
  CQSVGImageData *image = dynamic_cast<CQSVGImageData *>(in);
  assert(image);

  int w = std::min(getWidth (), image->getWidth ());
  int h = std::min(getHeight(), image->getHeight());

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba1 =        getPixel(x, y);
      CRGBA rgba2 = image->getPixel(x, y);

      CRGBA rgba = def.combine(rgba1, rgba2);

      setPixel(x, y, rgba.clamp());
    }
  }
}

void
CQSVGImageData::
combine(int x, int y, CSVGImageData *in)
{
  CQSVGImageData *image = dynamic_cast<CQSVGImageData *>(in);
  assert(image);

  int w = std::min(getWidth (), image->getWidth ());
  int h = std::min(getHeight(), image->getHeight());

  for (int y1 = 0; y1 < h; ++y1) {
    for (int x1 = 0; x1 < w; ++x1) {
      CRGBA rgba1 = image->getPixel(x1, y1);

      if (! rgba1.getAlphaI())
        continue;

      CRGBA rgba2 = getPixel(x1 + x, y1 + y);

      CRGBA rgba = rgba2.combined(rgba1);

      setPixel(x1 + x, y1 + y, rgba);
    }
  }
}

void
CQSVGImageData::
convolve(CSVGImageData *in, const CImageConvolveData &data)
{
  CQSVGImageData *dst = dynamic_cast<CQSVGImageData *>(in);
  assert(dst);

  int xsize = data.xsize;
  int ysize = data.ysize;

  if (xsize < 0)
    xsize = sqrt(data.kernel.size());

  if (ysize < 0)
    ysize = sqrt(data.kernel.size());

  //---

  int xborder = (xsize - 1)/2;
  int yborder = (ysize - 1)/2;

  double divisor = data.divisor;

  if (divisor < 0) {
    divisor = 0;

    for (const auto &k : data.kernel)
      divisor += k;

    if (divisor == 0)
      divisor = 1;
  }

  //---

  int wx1, wy1, wx2, wy2;

  getWindow(&wx1, &wy1, &wx2, &wy2);

  int y = wy1;

  for ( ; y < yborder; ++y) {
    int x = wx1;

    for ( ; x <= wx2 - xborder; ++x) {
      CRGBA rgba = getPixel(x, y);

      dst->setPixel(x, y, rgba);
    }
  }

  for ( ; y <= wy2 - yborder; ++y) {
    int x = wx1;

    for ( ; x < xborder; ++x) {
      CRGBA rgba = getPixel(x, y);

      dst->setPixel(x, y, rgba);
    }

    for ( ; x <= wx2 - xborder; ++x) {
      CRGBA sum;

      int k = 0;

      for (int yk = -yborder; yk <= yborder; ++yk) {
        for (int xk = -xborder; xk <= xborder; ++xk) {
          CRGBA rgba = getPixel(x + xk, y + yk);

          sum += rgba*data.kernel[k];

          ++k;
        }
      }

      sum /= divisor;

      sum.clamp();

      if (data.preserveAlpha) {
        CRGBA rgba = getPixel(x, y);

        sum.setAlpha(rgba.getAlpha());
      }

      dst->setPixel(x, y, sum);
    }

    for ( ; x <= wx2; ++x) {
      CRGBA rgba = getPixel(x, y);

      dst->setPixel(x, y, rgba);
    }
  }

  for ( ; y <= wy2; ++y) {
    int x = wx1;

    for ( ; x <= wx2; ++x) {
      CRGBA rgba = getPixel(x, y);

      dst->setPixel(x, y, rgba);
    }
  }
}

void
CQSVGImageData::
applyColorMatrix(const std::vector<double> &m)
{
  assert(m.size() == 20);

  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      CRGBA rgba = getPixel(x, y);

      double r, g, b, a;

      rgba.getRGBA(&r, &g, &b, &a);

      double a1 = m[15]*r + m[16]*g + m[17]*b + m[18]*a + m[19];

      if (a1 > 0) {
        double r1 = m[ 0]*r + m[ 1]*g + m[ 2]*b + m[ 3]*a + m[ 4];
        double g1 = m[ 5]*r + m[ 6]*g + m[ 7]*b + m[ 8]*a + m[ 9];
        double b1 = m[10]*r + m[11]*g + m[12]*b + m[13]*a + m[14];

        CRGBA rgba1(r1, g1, b1, a1);

        setPixel(x, y, rgba1);
      }
      else
        setPixel(x, y, CRGBA(0,0,0,0));
    }
  }
}

void
CQSVGImageData::
saturate(double ds)
{
  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      CRGBA rgba = getPixel(x, y);

      CHSV hsv = CRGBUtil::RGBtoHSV(rgba.getRGB());

      double s = hsv.getSaturation();

      s *= ds;

      hsv.setSaturation(s);

      CRGB rgb = CRGBUtil::HSVtoRGB(hsv);

      setPixel(x, y, CRGBA(rgb, rgba.getAlpha()));
    }
  }
}

void
CQSVGImageData::
rotateHue(double dh)
{
  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      CRGBA rgba = getPixel(x, y);

      CHSV hsv = CRGBUtil::RGBtoHSV(rgba.getRGB());

      double h = hsv.getHue();

      h += dh;

      while (h <  0    ) h += 360.0;
      while (h >= 360.0) h -= 360.0;

      hsv.setHue(h);

      CRGB rgb = CRGBUtil::HSVtoRGB(hsv);

      setPixel(x, y, CRGBA(rgb, rgba.getAlpha()));
    }
  }
}

void
CQSVGImageData::
luminanceToAlpha()
{
  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      CRGBA rgba = getPixel(x, y);

      //CHSV hsv = CRGBUtil::RGBtoHSV(rgba.getRGB()();
      //double a1 = hsv.getValue();
      //double a1 = rgba.getGray();
      double a = 0.2125*rgba.getRed() + 0.7154*rgba.getGreen() + 0.0721*rgba.getBlue();
      double a1 = a*rgba.getAlpha();

      //rgba.setAlpha(a1);
      //setPixel(x, y, rgba);

      CRGBA rgba1(a1, a1, a1, a1);
      setPixel(x, y, rgba1);
    }
  }
}

void
CQSVGImageData::
linearFunc(CColorComponent component, double scale, double offset)
{
  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      CRGBA rgba = getPixel(x, y);

      double value = rgba.getComponent(component);

      value = value*scale + offset;

      rgba.setComponent(component, value);

      setPixel(x, y, rgba.clamp());
    }
  }
}

void
CQSVGImageData::
gammaFunc(CColorComponent component, double amplitude, double exponent, double offset)
{
  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      CRGBA rgba = getPixel(x, y);

      double value = rgba.getComponent(component);

      value = amplitude*pow(value, exponent) + offset,

      rgba.setComponent(component, value);

      setPixel(x, y, rgba.clamp());
    }
  }
}

void
CQSVGImageData::
tableFunc(CColorComponent component, const std::vector<double> &values)
{
  int num_ranges = values.size() - 1;

  if (num_ranges < 1) return;

  double delta = 1.0/num_ranges;

  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      // get component color value
      CRGBA rgba = getPixel(x, y);

      double value = rgba.getComponent(component);

      // get associated range index
      int    i = 0;
      double value1, value2;

      for ( ; i < num_ranges; ++i) {
        value1 = i*delta;
        value2 = value1 + delta;

        if (value >= value1 && value < value2) break;
      }

      if (i >= num_ranges) continue;

      // remap to new range
      double m = (values[i + 1] - values[i])/(value2 - value1);

      value = (value - value1)*m + values[i];

      // update color
      rgba.setComponent(component, value);

      setPixel(x, y, rgba.clamp());
    }
  }
}

void
CQSVGImageData::
discreteFunc(CColorComponent component, const std::vector<double> &values)
{
  uint num_ranges = values.size();

  if (num_ranges < 1) return;

  double delta = 1.0/num_ranges;

  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  for (int y = y1; y <= y2; ++y) {
    for (int x = x1; x <= x2; ++x) {
      // get component color value
      CRGBA rgba = getPixel(x, y);

      double value = rgba.getComponent(component);

      // get associated range index
      uint i = 0;

      for ( ; i < num_ranges; ++i) {
        double value1 = i*delta;
        double value2 = value1 + delta;

        if (value >= value1 && value < value2) break;
      }

      if (i >= num_ranges) continue;

      // set to new value
      value = values[i];

      // update color
      rgba.setComponent(component, value);

      setPixel(x, y, rgba.clamp());
    }
  }
}

CSVGImageData *
CQSVGImageData::
displacementMap(CSVGImageData *in, CColorComponent xcolor, CColorComponent ycolor, double scale)
{
  CQSVGImageData *dispImage = dynamic_cast<CQSVGImageData *>(in);
  assert(dispImage);

  CSVGImageData *dst = dup();

  int wx1, wy1, wx2, wy2;

  getWindow(&wx1, &wy1, &wx2, &wy2);

  for (int y = wy1; y <= wy2; ++y) {
    for (int x = wx1; x <= wx2; ++x) {
      CRGBA rgba1(0,0,0,0);

      // get displacement from dispImage color components
      if (dispImage->validPixel(x, y)) {
        CRGBA rgba2 = dispImage->getPixel(x, y);

        double rx = rgba2.getComponent(xcolor);
        double ry = rgba2.getComponent(ycolor);

        double x1 = x + scale*(rx - 0.5);
        double y1 = y + scale*(ry - 0.5);

        // TODO: interp pixel
        int ix1 = int(x1 + 0.5);
        int iy1 = int(y1 + 0.5);

        if (validPixel(ix1, iy1))
          rgba1 = getPixel(ix1, iy1);
        else {
          //rgba1 = getPixel(x, y);
        }
      }
      else {
        //rgba1 = getPixel(x, y);
      }

      dst->setPixel(x, y, rgba1);
    }
  }

  return dst;
}

void
CQSVGImageData::
gaussianBlur(CSVGImageData *in, double stdDevX, double stdDevY)
{
  class CImageWrapper {
   public:
    CImageWrapper(CQSVGImageData *image) :
     image_(image) {
    }

    void getPixelRange(int *x1, int *y1, int *x2, int *y2) const {
      *x1 = 0;
      *y1 = 0;
      *x2 = image_->getWidth () - 1;
      *y2 = image_->getHeight() - 1;
    }

    void getWindow(int *x1, int *y1, int *x2, int *y2) const {
      image_->getWindow(x1, y1, x2, y2);
    }

    void getRGBA(int x, int y, double *r, double *g, double *b, double *a) const {
      CRGBA rgba = image_->getPixel(x, y);

      *r = rgba.getRed  ();
      *g = rgba.getGreen();
      *b = rgba.getBlue ();
      *a = rgba.getAlpha();
    }

    void setRGBA(int x, int y, double r, double g, double b, double a) {
      image_->setPixel(x, y, CRGBA(r, g, b, a));
    }

   private:
    CQSVGImageData *image_;
  };

  //---

  CQSVGImageData *qin = dynamic_cast<CQSVGImageData *>(in);
  assert(qin);

  CGaussianBlur<CImageWrapper> blur;

  CImageWrapper wsrc(this);
  CImageWrapper wdst(qin);

  blur.blur(wsrc, wdst, stdDevX, stdDevY, 0, 0);
}

CSVGImageData *
CQSVGImageData::
erode(int r, bool isAlpha)
{
  int r1 = 2*r + 1;
  int r2 = r1*r1;

  std::vector<int> mask;

  mask.resize(r2);

  for (int i = 0, iy = -r; iy <= r; ++iy) {
    for (int ix = -r; ix <= r; ++ix, ++i) {
      double r1 = hypot(ix, iy);

      mask[i] = (r1 <= r ? 1 : 0);
    }
  }

  return erode(mask, isAlpha);
}

CSVGImageData *
CQSVGImageData::
erode(const std::vector<int> &mask, bool isAlpha) const
{
  return erodeDilate(mask, isAlpha, true);
}

CSVGImageData *
CQSVGImageData::
dilate(int r, bool isAlpha)
{
  int r1 = 2*r + 1;
  int r2 = r1*r1;

  std::vector<int> mask;

  mask.resize(r2);

  for (int i = 0, iy = -r; iy <= r; ++iy) {
    for (int ix = -r; ix <= r; ++ix, ++i) {
      double r1 = hypot(ix, iy);

      mask[i] = (r1 <= r ? 1 : 0);
    }
  }

  return dilate(mask, isAlpha);
}

CSVGImageData *
CQSVGImageData::
dilate(const std::vector<int> &mask, bool isAlpha) const
{
  return erodeDilate(mask, isAlpha, false);
}

CSVGImageData *
CQSVGImageData::
erodeDilate(const std::vector<int> &mask, bool isAlpha, bool isErode) const
{
  int r = (sqrt(mask.size()) - 1)/2;

  // count mask bits
  int num_hits = 0;

  for (const auto &m : mask)
    num_hits += m;

  //---

  int width  = getWidth ();
  int height = getHeight();

  CSVGImageData *image = dup();

  image->setSize(width, height);

  int x1, y1, x2, y2;

  getWindow(&x1, &y1, &x2, &y2);

  for (int y = y1; y <= y2; ++y) {
    bool yinside = (y >= y1 + r && y <= y2 - r);

    for (int x = x1; x <= x2; ++x) {
      bool xinside = (x >= x1 + r && x <= x2 - r);

      bool isSet = (xinside && yinside);

      CRGBA rgba;

      if (isSet) {
        // count mask hits
        int hits = 0;

        for (int i = 0, ix = -r; ix <= r; ++ix)
          for (int iy = -r; iy <= r; ++iy, ++i)
            if (mask[i] && isErodePixel(x + ix, y + iy, isAlpha, rgba))
              ++hits;

        if (isErode)
          isSet = (hits == num_hits);
        else
          isSet = (hits > 0);
      }

      int x2 = x - x1;
      int y2 = y - y1;

      if (isAlpha) {
        if (isSet) {
          rgba /= num_hits;

          image->setPixel(x2, y2, rgba);
        }
        else
          image->setPixel(x2, y2, CRGBA(0, 0, 0, 0));
      }
      else {
        if (isSet)
          image->setPixel(x2, y2, CRGBA(1, 1, 1));
        else
          image->setPixel(x2, y2, CRGBA(0, 0, 0));
      }
    }
  }

  return image;
}

bool
CQSVGImageData::
isErodePixel(int x, int y, bool isAlpha, CRGBA &rgba) const
{
  CRGBA rgba1 = getPixel(x, y);

  if (isAlpha) {
    if (rgba1.getAlpha() > 0.5) {
      rgba += rgba1;

      return true;
    }
  }
  else {
    if (rgba1.getGray() > 0.5)
      return true;
  }

  return false;
}

CSVGImageData *
CQSVGImageData::
tile(int width, int height, const CImageTile &tile)
{
  CSVGImageData *image = dup();

  image->setSize(width, height);

  //------

  int x_offset = 0;

  if      (tile.halign == CHALIGN_TYPE_CENTER)
    x_offset = (width - getWidth())/2;
  else if (tile.halign == CHALIGN_TYPE_RIGHT)
    x_offset = width - getWidth();

  while (x_offset > 0)
    x_offset -= getWidth();

  x_offset = -x_offset;

  //------

  int y_offset = 0;

  if      (tile.valign == CVALIGN_TYPE_CENTER)
    y_offset = (height - getHeight())/2;
  else if (tile.valign == CVALIGN_TYPE_BOTTOM)
    y_offset = height - getHeight();

  while (y_offset > 0)
    y_offset -= getHeight();

  y_offset = -y_offset;

  //------

  for (int y = 0; y < height; ++y) {
    int y1 = (y + y_offset) % getHeight();

    for (int x = 0; x < width; ++x) {
      int x1 = (x + x_offset) % getWidth();

      double r, g, b, a;

      getPixel(x1, y1, &r, &g, &b, &a);

      image->setPixel(x, y, CRGBA(r, g, b, a));
    }
  }

  //------

  return image;
}

void
CQSVGImageData::
turbulence(bool fractal, double baseFreqX, double baseFreqY, int numOctaves, int seed)
{
  CTurbulenceUtil turbulence(seed);

  double r, g, b, a, point[2];

  int wx1, wy1, wx2, wy2;

  getWindow(&wx1, &wy1, &wx2, &wy2);

  for (int y = wy1; y <= wy2; ++y) {
    for (int x = wx1; x <= wx2; ++x) {
      CRGBA rgba = getPixel(x, y);

      if (rgba.isTransparent())
        continue;

      //TODO: keep alpha ?
      //double a = rgba.getAlpha();

      point[0] = x;
      point[1] = y;

      r = turbulence.turbulence(0, point, baseFreqX, baseFreqY, numOctaves, fractal);
      g = turbulence.turbulence(1, point, baseFreqX, baseFreqY, numOctaves, fractal);
      b = turbulence.turbulence(2, point, baseFreqX, baseFreqY, numOctaves, fractal);
      a = turbulence.turbulence(3, point, baseFreqX, baseFreqY, numOctaves, fractal);

      if (fractal) {
        r = (r + 1.0) / 2.0;
        g = (g + 1.0) / 2.0;
        b = (b + 1.0) / 2.0;
        a = (a + 1.0) / 2.0;
      }

      r = std::min(std::max(r, 0.0), 1.0);
      g = std::min(std::max(g, 0.0), 1.0);
      b = std::min(std::max(b, 0.0), 1.0);
      a = std::min(std::max(a, 0.0), 1.0);

      setPixel(x, y, CRGBA(r, g, b, a));
    }
  }
}

void
CQSVGImageData::
sobelPixelGradient(int x, int y, int dx, int dy, double &xgray, double &ygray,
                   double &xf, double &yf)
{
  int w = getWidth ();
  int h = getHeight();

  int wx1, wy1, wx2, wy2;

  getWindow(&wx1, &wy1, &wx2, &wy2);

  if (x < dx || x > w - dx - 1 || y < dy || y > h - dy - 1) {
    getGrayPixel(x, y, &xgray);

    ygray = xgray;

    xf = 1;
    yf = 1;

    return;
  }

  double gray1, gray2, gray3, gray4, gray5, gray6, gray7, gray8, gray9;

  bool left = (x < dx), right  = (x > w - dx - 1);
  bool top  = (y < dy), bottom = (y > h - dy - 1);

  if      (top && left) {
    getGrayPixel(x - dx, y - dy, &gray1);
    getGrayPixel(x     , y - dy, &gray2);
    getGrayPixel(x - dx, y     , &gray3);
    getGrayPixel(x     , y     , &gray4);

    xgray = -2*gray1 +2*gray2 -1*gray3 +1*gray4;
    ygray = -2*gray1 -1*gray2 +2*gray3 +1*gray4;

    xf = 2.0/(3*dx);
    yf = 2.0/(3*dy);
  }
  else if (top && right) {
    getGrayPixel(x     , y - dy, &gray1);
    getGrayPixel(x + dx, y - dy, &gray2);
    getGrayPixel(x     , y     , &gray3);
    getGrayPixel(x + dx, y     , &gray4);

    xgray = -2*gray1 +2*gray2 -1*gray3 +1*gray4;
    ygray = -1*gray1 -2*gray2 +1*gray3 +2*gray4;

    xf = 2.0/(3*dx);
    yf = 2.0/(3*dy);
  }
  else if (bottom && left) {
    getGrayPixel(x - dx, y     , &gray1);
    getGrayPixel(x     , y     , &gray2);
    getGrayPixel(x - dx, y + dy, &gray3);
    getGrayPixel(x     , y + dy, &gray4);

    xgray = -1*gray1 +1*gray2 -2*gray3 +2*gray4;
    ygray = -2*gray1 -1*gray2 +2*gray3 +1*gray4;

    xf = 2.0/(3*dx);
    yf = 2.0/(3*dy);
  }
  else if (bottom && right) {
    getGrayPixel(x     , y     , &gray1);
    getGrayPixel(x + dx, y     , &gray2);
    getGrayPixel(x     , y + dy, &gray3);
    getGrayPixel(x + dx, y + dy, &gray4);

    xgray = -1*gray1 +1*gray2 -2*gray3 +2*gray4;
    ygray = -1*gray1 -2*gray2 +1*gray3 +2*gray4;

    xf = 2.0/(3*dx);
    yf = 2.0/(3*dy);
  }
  else if (top) {
    getGrayPixel(x - dx, y - dy, &gray1);
    getGrayPixel(x     , y - dy, &gray2);
    getGrayPixel(x + dx, y - dy, &gray3);
    getGrayPixel(x - dx, y     , &gray4);
    getGrayPixel(x     , y     , &gray5);
    getGrayPixel(x + dx, y     , &gray6);

    xgray = -2*gray1          +2*gray3 -1*gray4          +1*gray6;
    ygray = -1*gray1 -2*gray2 -1*gray3 +1*gray4 +2*gray5 +1*gray6;

    xf = 1.0/(3*dx);
    yf = 1.0/(2*dy);
  }
  else if (left) {
    getGrayPixel(x - dx, y - dy, &gray1);
    getGrayPixel(x     , y - dy, &gray2);
    getGrayPixel(x - dx, y     , &gray3);
    getGrayPixel(x     , y     , &gray4);
    getGrayPixel(x - dx, y + dy, &gray5);
    getGrayPixel(x     , y + dy, &gray6);

    xgray = -1*gray1 +1*gray2 -2*gray3 +2*gray4 -1*gray5 +1*gray6;
    ygray = -2*gray1 -1*gray2                   +2*gray5 +1*gray6;

    xf = 1.0/(2*dx);
    yf = 1.0/(3*dy);
  }
  else if (right) {
    getGrayPixel(x     , y - dy, &gray1);
    getGrayPixel(x + dx, y - dy, &gray2);
    getGrayPixel(x     , y     , &gray3);
    getGrayPixel(x + dx, y     , &gray4);
    getGrayPixel(x     , y + dy, &gray5);
    getGrayPixel(x + dx, y + dy, &gray6);

    xgray = -1*gray1 +1*gray2 -2*gray3 +2*gray4 -1*gray5 +1*gray6;
    ygray = -1*gray1 -2*gray2                   +1*gray5 +2*gray6;

    xf = 1.0/(2*dx);
    yf = 1.0/(3*dy);
  }
  else if (bottom) {
    getGrayPixel(x - dx, y     , &gray1);
    getGrayPixel(x     , y     , &gray2);
    getGrayPixel(x + dx, y     , &gray3);
    getGrayPixel(x - dx, y + dy, &gray4);
    getGrayPixel(x     , y + dy, &gray5);
    getGrayPixel(x + dx, y + dy, &gray6);

    xgray = -1*gray1          +1*gray3 -2*gray4          +2*gray6;
    ygray = -1*gray1 -2*gray2 -1*gray3 +1*gray4 +2*gray5 +1*gray6;

    xf = 1.0/(3*dx);
    yf = 1.0/(2*dy);
  }
  else {
    getGrayPixel(x - dx, y - dy, &gray1);
    getGrayPixel(x     , y - dy, &gray2);
    getGrayPixel(x + dx, y - dy, &gray3);
    getGrayPixel(x - dx, y     , &gray4);
  //getGrayPixel(x     , y     , &gray5);
    getGrayPixel(x + dx, y     , &gray6);
    getGrayPixel(x - dx, y + dy, &gray7);
    getGrayPixel(x     , y + dy, &gray8);
    getGrayPixel(x + dx, y + dy, &gray9);

    xgray = -1*gray1          +1*gray3 -2*gray4          +2*gray6 -1*gray7          +1*gray9;
    ygray = -1*gray1 -2*gray2 -1*gray3                            +1*gray7 +2*gray8 +1*gray9;

    xf = 1.0/(4*dx);
    yf = 1.0/(4*dy);
  }
}

CSVGImageData *
CQSVGImageData::
createRGBAMask()
{
  CSVGImageData *image = dup();

  image->setSize(getWidth(), getHeight());

  //---

  // rgba contains r, g, b factors (default gray factors)
  CRGBA rgba(0.2125, 0.7154, 0.0721);

  double r, g, b, a;

  rgba.getRGBA(&r, &g, &b, &a);

  // normalize (default is already normalized)
  double sum = r + g + b;

  r /= sum;
  g /= sum;
  b /= sum;

  //---

  for (int y = 0; y < getHeight(); ++y) {
    for (int x = 0; x < getWidth(); ++x) {
      double r1, g1, b1, a1;

      getPixel(x, y, &r1, &g1, &b1, &a1);

      a = a1*(r*r1 + g*g1 + b*b1);

      image->setPixel(x, y, CRGBA(r1, g1, b1, a));
    }
  }

  return image;
}

void
CQSVGImageData::
getGrayPixel(int x, int y, double *gray) const
{
  CRGBA rgba = getPixel(x, y);

  *gray = rgba.getGray();
}

void
CQSVGImageData::
getWindow(int *x1, int *y1, int *x2, int *y2) const
{
  *x1 = 0;
  *y1 = 0;
  *x2 = getWidth () - 1;
  *y2 = getHeight() - 1;
}

bool
CQSVGImageData::
validPixel(int x, int y) const
{
  return (x >= 0 && x < getWidth() && y >= 0 && y < getHeight());
}
