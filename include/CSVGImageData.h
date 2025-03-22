#ifndef CSVGImageData_H
#define CSVGImageData_H

#include <CSVGTypes.h>
#include <CImagePtr.h>
#include <CFileType.h>
#include <CRGBA.h>
#include <vector>

struct CImageTileData;
struct CImageConvolveData;
struct CRGBACombineDef;

class CSVGImageData {
 public:
  CSVGImageData();
  CSVGImageData(const CSVGImageData &data);

  virtual ~CSVGImageData();

  virtual CSVGImageData *dup() const;

  virtual CImagePtr image() const;
  virtual void setImage(CImagePtr &image);

  virtual void setSize(int w, int h);

  virtual int getWidth () const;
  virtual int getHeight() const;

  virtual bool read(const std::string &filename);
  virtual bool write(const std::string &filename, CFileType type);

  virtual void clear();

  virtual void set(const CRGBA &c);

  virtual void setAlphaGray(double gray);

  virtual CRGBA getPixel(int x, int y) const;
  virtual void setPixel(int x, int y, const CRGBA &c);

  virtual void setWindow(int x1, int y1, int x2, int y2) {
    x1_ = x1; y1_ = y1;
    x2_ = x2; y2_ = y2;
  }

  virtual void subCopyTo(CSVGImageData *dst, int src_x, int src_y, int width, int height,
                         int dst_x, int dst_y);

  virtual CSVGImageData *subImage(int x, int y, int w, int h);

  virtual void copyAlpha(CSVGImageData *dst, int x, int y);

  virtual void scaleAlpha(double alpha);

  virtual void reshape(int w, int h);

  virtual void reshapeKeepAspect(int w, int h);

  virtual void clipOutside(int x1, int y1, int x2, int y2);

  virtual void combine(CSVGImageData *in, CRGBABlendMode mode);
  virtual void combine(CSVGImageData *in, const CRGBACombineDef &def);
  virtual void combine(int x, int y, CSVGImageData *in);

  virtual void convolve(CSVGImageData *in, const CImageConvolveData &data);

  virtual void applyColorMatrix(const std::vector<double> &values);

  virtual void saturate(double value);

  virtual void rotateHue(double value);

  virtual void luminanceToAlpha();

  virtual void linearFunc(CRGBAComponent component, double slope, double intercept);

  virtual void gammaFunc(CRGBAComponent component, double amplitude,
                         double exponent, double offset);

  virtual void tableFunc(CRGBAComponent component, const std::vector<double> &table);

  virtual void discreteFunc(CRGBAComponent component, const std::vector<double> &table);

  virtual CSVGImageData *displacementMap(CSVGImageData *in, CRGBAComponent xcolor,
                                         CRGBAComponent ycolor, double scale);

  virtual void gaussianBlur(CSVGImageData *in, double stdDevX, double stdDevY);

  virtual CSVGImageData *erode(int r, bool isAlpha);

  virtual CSVGImageData *dilate(int r, bool isAlpha);

  virtual CSVGImageData *tile(int w, int h, const CImageTileData &tile);

  virtual void turbulence(bool fractalNoise, double baseFreqX, double baseFreqY,
                          int numOctaves, int seed);

  virtual void sobelPixelGradient(int x, int y, int dx, int dy,
                                  double &xgray, double &ygray, double &xf, double &yf);

  virtual CSVGImageData *createRGBAMask();

 protected:
  CImagePtr image_;
  int       x1_ { 0 };
  int       y1_ { 0 };
  int       x2_ { -1 };
  int       y2_ { -1 };
};

using CSVGImageDataP = std::shared_ptr<CSVGImageData>;

#endif
