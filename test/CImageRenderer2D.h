#ifndef CIMAGE_RENDERER_2D_H
#define CIMAGE_RENDERER_2D_H

#include <CAutoPtr.h>

#define CRENDERER_2D_VIRTUAL

#include <CRenderer2D.h>
#include <CImagePixelRenderer.h>

class CImageGC;

class CImageRenderer2D : public CRenderer2D {
 private:
  CAutoPtr<CImagePixelRenderer> ipixel_renderer_;

 public:
  CImageRenderer2D(uint width=100, uint height=100);
  CImageRenderer2D(CImagePtr image);

  virtual ~CImageRenderer2D();

  virtual CImageRenderer2D *dup() const;

  virtual CImagePixelRenderer *getPixelRenderer() const;

  virtual void writePNG(const std::string &filename);

  virtual void updateSize(int width, int height);

  virtual CImagePtr getImage();

  virtual void drawLine(const CPoint2D &point1, const CPoint2D &point2);

  virtual void fillPolygon(const PointList &points);

  virtual void drawPolygon(const PointList &points);

  virtual void drawText(const CPoint2D &point, const std::string &str);

  virtual void fillText(const CPoint2D &point, const std::string &str);

  virtual void drawImage(const CPoint2D &point, CImagePtr image);

  virtual void drawAlphaImage(const CPoint2D &point, CImagePtr image);

  virtual void drawImageInBox(const CBBox2D &point, CImagePtr image);

  virtual void drawAlphaImageInBox(const CBBox2D &point, CImagePtr image);

  virtual void drawPoint(const CPoint2D &point);

  virtual void fillRectangle(const CBBox2D &bbox);

  virtual void gradientFillRectangle(const CBBox2D &bbox, CGenGradient *gradient);

  virtual void drawRectangle(const CBBox2D &bbox);
};

#endif
