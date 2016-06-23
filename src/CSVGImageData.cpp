#include <CSVGImageData.h>
#include <CImageMgr.h>

CSVGImageData::
CSVGImageData()
{
  CImageNoSrc src;

  image_ = CImageMgrInst->createImage(src);
}

CSVGImageData::
CSVGImageData(const CSVGImageData &data)
{
  image_ = data.image_->dup();
}

CSVGImageData *
CSVGImageData::
dup() const
{
  return new CSVGImageData(*this);
}

CImagePtr
CSVGImageData::
image() const
{
  return image_;
}

void
CSVGImageData::
setImage(CImagePtr &image)
{
  image_ = image;
}

bool
CSVGImageData::
read(const std::string &filename)
{
  return image_->read(filename);
}

bool
CSVGImageData::
write(const std::string &filename, CFileType type)
{
  return image_->write(filename, type);
}

void
CSVGImageData::
setSize(int w, int h)
{
  image_->setDataSize(w, h);
}

int
CSVGImageData::
getWidth() const
{
  return image_->getWidth();
}

int
CSVGImageData::
getHeight() const
{
  return image_->getHeight();
}

void
CSVGImageData::
clear()
{
  image_->setRGBAData(CRGBA(0,0,0,0));
}

void
CSVGImageData::
set(const CRGBA &c)
{
  image_->setRGBAData(c);
}

void
CSVGImageData::
setAlphaGray(double gray)
{
  image_->setAlphaGray(gray);
}

CRGBA
CSVGImageData::
getPixel(int x, int y) const
{
  CRGBA rgba;

  image_->getRGBAPixel(x, y, rgba);

  return rgba;
}

void
CSVGImageData::
setPixel(int x, int y, const CRGBA &c)
{
  image_->setRGBAPixel(x, y, c);
}

void
CSVGImageData::
subCopyTo(CSVGImageData *dst, int src_x, int src_y, int width, int height,
          int dst_x, int dst_y)
{
  image_->subCopyTo(dst->image_, src_x, src_y, width, height, dst_x, dst_y);
}

CSVGImageData *
CSVGImageData::
subImage(int x, int y, int w, int h)
{
  CImagePtr image = image_->subImage(x, y, w, h);

  CSVGImageData *data = dup();

  data->setImage(image);

  return data;
}

void
CSVGImageData::
copyAlpha(CSVGImageData *dst, int x, int y)
{
  image_->copyAlpha(dst->image_, x, y);
}

void
CSVGImageData::
scaleAlpha(double alpha)
{
  image_->scaleAlpha(alpha);
}

void
CSVGImageData::
reshape(int w, int h)
{
  image_->reshape(w, h);
}

void
CSVGImageData::
reshapeKeepAspect(int w, int h)
{
  image_->reshapeKeepAspect(w, h);
}

void
CSVGImageData::
clipOutside(int x1, int y1, int x2, int y2)
{
  image_->clipOutside(x1, y1, x2, y2);
}

void
CSVGImageData::
combine(CSVGImageData *in, CRGBABlendMode mode)
{
  image_->combine(in->image_, mode);
}

void
CSVGImageData::
combine(CSVGImageData *in, const CRGBACombineDef &def)
{
  image_->combine(in->image_, def);
}

void
CSVGImageData::
combine(int x, int y, CSVGImageData *in)
{
  image_->combine(x, y, in->image_);
}

void
CSVGImageData::
convolve(CSVGImageData *in, const CImageConvolveData &data)
{
  image_->convolve(in->image_, data);
}

void
CSVGImageData::
applyColorMatrix(const std::vector<double> &values)
{
  image_->applyColorMatrix(values);
}

void
CSVGImageData::
saturate(double value)
{
  image_->saturate(value);
}

void
CSVGImageData::
rotateHue(double value)
{
  image_->rotateHue(value);
}

void
CSVGImageData::
luminanceToAlpha()
{
  image_->luminanceToAlpha();
}

void
CSVGImageData::
linearFunc(CColorComponent component, double slope, double intercept)
{
  image_->linearFunc(component, slope, intercept);
}

void
CSVGImageData::
gammaFunc(CColorComponent component, double amplitude, double exponent, double offset)
{
  image_->gammaFunc(component, amplitude, exponent, offset);
}

void
CSVGImageData::
tableFunc(CColorComponent component, const std::vector<double> &table)
{
  image_->tableFunc(component, table);
}

void
CSVGImageData::
discreteFunc(CColorComponent component, const std::vector<double> &table)
{
  image_->discreteFunc(component, table);
}

CSVGImageData *
CSVGImageData::
displacementMap(CSVGImageData *in, CColorComponent xcolor, CColorComponent ycolor, double scale)
{
  CImagePtr image = image_->displacementMap(in->image_, xcolor, ycolor, scale);

  CSVGImageData *data = dup();

  data->setImage(image);

  return data;
}

void
CSVGImageData::
gaussianBlur(CSVGImageData *in, double stdDevX, double stdDevY)
{
  image_->gaussianBlur(in->image_, stdDevX, stdDevY);
}

CSVGImageData *
CSVGImageData::
erode(int r, bool isAlpha)
{
  CImagePtr image = image_->erode(r, isAlpha);

  CSVGImageData *data = dup();

  data->setImage(image);

  return data;
}

CSVGImageData *
CSVGImageData::
dilate(int r, bool isAlpha)
{
  CImagePtr image = image_->dilate(r, isAlpha);

  CSVGImageData *data = dup();

  data->setImage(image);

  return data;
}

CSVGImageData *
CSVGImageData::
tile(int w, int h, const CImageTile &tile)
{
  CImagePtr image = image_->tile(w, h, tile);

  CSVGImageData *data = dup();

  data->setImage(image);

  return data;
}

void
CSVGImageData::
turbulence(bool fractalNoise, double baseFreqX, double baseFreqY, int numOctaves, int seed)
{
  image_->turbulence(fractalNoise, baseFreqX, baseFreqY, numOctaves, seed);
}

void
CSVGImageData::
sobelPixelGradient(int x, int y, int dx, int dy, double &xgray, double &ygray,
                   double &xf, double &yf)
{
  image_->sobelPixelGradient(x, y, dx, dy, xgray, ygray, xf, yf);
}

CSVGImageData *
CSVGImageData::
createRGBAMask()
{
  CImagePtr image = image_->createRGBAMask();

  CSVGImageData *data = dup();

  data->setImage(image);

  return data;
}
