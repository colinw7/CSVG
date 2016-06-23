#ifndef CQSVGImageData_H
#define CQSVGImageData_H

#include <CSVGImageData.h>
#include <QImage>

class CQSVGImageData : public CSVGImageData {
 public:
  CQSVGImageData();
  CQSVGImageData(const CQSVGImageData &data);

 ~CQSVGImageData() { }

  CSVGImageData *dup() const override;

  CImagePtr image() const override;
  void setImage(CImagePtr &image) override;

  const QImage &qimage() const { return qimage_; }
  void setQImage(const QImage &qimage);

  QImage *lockImage() { locked_ = true; return &qimage_; }
  void unlockImage() { assert(locked_); locked_ = false; }

  void setSize(int w, int h) override;

  int getWidth () const override;
  int getHeight() const override;

  bool read(const std::string &filename) override;
  bool write(const std::string &filename, CFileType type) override;

  void clear() override;

  void set(const CRGBA &c) override;

  void setAlphaGray(double gray) override;

  CRGBA getPixel(int x, int y) const override;
  void setPixel(int x, int y, const CRGBA &c) override;

  void subCopyTo(CSVGImageData *dst, int src_x, int src_y, int width, int height,
                 int dst_x, int dst_y) override;

  CSVGImageData *subImage(int x, int y, int w, int h) override;

  void copyAlpha(CSVGImageData *dst, int x, int y) override;

  void scaleAlpha(double alpha);

  void reshape(int w, int h) override;

  void reshapeKeepAspect(int w, int h) override;

  void clipOutside(int x1, int y1, int x2, int y2) override;

  void combine(CSVGImageData *in, CRGBABlendMode mode) override;
  void combine(CSVGImageData *in, const CRGBACombineDef &def) override;
  void combine(int x, int y, CSVGImageData *in) override;

  void convolve(CSVGImageData *in, const CImageConvolveData &data) override;

  void applyColorMatrix(const std::vector<double> &values) override;

  void saturate(double value) override;

  void rotateHue(double value) override;

  void luminanceToAlpha() override;

  void linearFunc(CColorComponent component, double slope, double intercept) override;

  void gammaFunc(CColorComponent component, double amplitude,
                 double exponent, double offset) override;

  void tableFunc(CColorComponent component, const std::vector<double> &table) override;

  void discreteFunc(CColorComponent component, const std::vector<double> &table) override;

  CSVGImageData *displacementMap(CSVGImageData *in, CColorComponent xcolor,
                                 CColorComponent ycolor, double scale) override;

  void gaussianBlur(CSVGImageData *in, double stdDevX, double stdDevY) override;

  CSVGImageData *erode (int r, bool isAlpha) override;
  CSVGImageData *dilate(int r, bool isAlpha) override;

  CSVGImageData *tile(int w, int h, const CImageTile &tile) override;

  void turbulence(bool fractalNoise, double baseFreqX, double baseFreqY,
                  int numOctaves, int seed) override;

  void sobelPixelGradient(int x, int y, int dx, int dy,
                          double &xgray, double &ygray, double &xf, double &yf) override;

  CSVGImageData *createRGBAMask() override;

 private:
  void getPixel(int x, int y, double *r, double *g, double *b, double *a) const;

  void reshapeBilinear(CSVGImageData *new_image) const;
  void reshapeAverage (CSVGImageData *new_image) const;
  void reshapeNearest (CSVGImageData *new_image) const;

  void reshapeWidth (int width );
  void reshapeHeight(int height);

  CRGBA getBilinearPixel(double xx, double yy) const;
  CRGBA getBilinearPixel(double xx, int x1, int x2, double yy, int y1, int y2) const;

  CSVGImageData *erode (const std::vector<int> &mask, bool isAlpha) const;
  CSVGImageData *dilate(const std::vector<int> &mask, bool isAlpha) const;

  CSVGImageData *erodeDilate(const std::vector<int> &mask, bool isAlpha, bool isErode) const;

  bool isErodePixel(int x, int y, bool isAlpha, CRGBA &rgba) const;

  void getGrayPixel(int x, int y, double *gray) const;

  void getWindow(int *x1, int *y1, int *x2, int *y2) const;

  bool validPixel(int x, int y) const;

 private:
  QImage qimage_;
  bool   locked_ { false };
};

#endif
