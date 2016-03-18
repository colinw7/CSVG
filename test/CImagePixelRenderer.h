#ifndef CIMAGE_PIXEL_RENDERER_H
#define CIMAGE_PIXEL_RENDERER_H

#include <CPixelRenderer.h>

class CImagePixelRenderer : public CPixelRenderer {
 private:
  CImagePtr image_;

 public:
  CImagePixelRenderer(int width=100, int height=100);
  CImagePixelRenderer(CImagePtr image);

  virtual ~CImagePixelRenderer();

  CImagePixelRenderer *dup() const;

  virtual void setImage(CImagePtr image);

  virtual CImagePtr getImage() { return image_; }

  virtual uint getWidth() const;
  virtual uint getHeight() const;

  virtual void updateSize(int width, int height);

  virtual void clear();
  virtual void fill();

  virtual void drawClippedPoint(const CIPoint2D &point);

  void drawClippedImage(const CIPoint2D &point, CImagePtr image);

  virtual void writePNG(const std::string &filename);
};

#endif
