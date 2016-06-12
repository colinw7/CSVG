#ifndef CSVGBlockData_H
#define CSVGBlockData_H

#include <CSVGPreserveAspect.h>
#include <iostream>

// block offset and scale
class CSVGBlockData {
 public:
  CSVGBlockData() {
    updateViewMatrix();
  }

  CSVGBlockData(const CBBox2D &pixelBBox, const CBBox2D &viewBBox, const CPoint2D &offset,
                double xs, double ys, const CSVGPreserveAspect &preserveAspect) :
   pixelBBox_(pixelBBox), viewBBox_(viewBBox), offset_(offset),
   xscale_(xs), yscale_(ys), preserveAspect_(preserveAspect) {
    updateViewMatrix();
  }

  const CBBox2D &pixelBBox() const { return pixelBBox_; }
  void setPixelBBox(const CBBox2D &v) { pixelBBox_ = v; }

  const CBBox2D &viewBBox() const { return viewBBox_; }
  void setViewBBox(const CBBox2D &v) { viewBBox_ = v; }

  const CPoint2D &offset() const { return offset_; }
  void setOffset(const CPoint2D &v) { offset_ = v; updateViewMatrix(); }

  double xscale() const { return xscale_; }
  void setXScale(double r) { xscale_ = r; updateViewMatrix(); }

  double yscale() const { return yscale_; }
  void setYScale(double r) { yscale_ = r; updateViewMatrix(); }

  const CSVGPreserveAspect &preserveAspect() const { return preserveAspect_; }
  void setPreserveAspect(const CSVGPreserveAspect &v) { preserveAspect_ = v; }

  const CMatrixStack2D &viewMatrix() const { return viewMatrix_; }
  void setViewMatrix(const CMatrixStack2D &v) { viewMatrix_ = v; }

  void reset() {
    pixelBBox_      = CBBox2D();
    viewBBox_       = CBBox2D();
    offset_         = CPoint2D();
    xscale_         = 1;
    yscale_         = 1;
    preserveAspect_ = CSVGPreserveAspect();

    updateViewMatrix();
  }

  void updateViewMatrix() {
    CMatrixStack2D matrix;

    matrix.translate(offset_.x, offset_.y);
    matrix.scale(xscale_, yscale_);

    viewMatrix_ = matrix;
  }

  void print(std::ostream &os) const {
    os << "Pixel BBox: " << pixelBBox_ << std::endl;
    os << "View  BBox: " << viewBBox_ << std::endl;
    os << "Offset    : " << offset_ << std::endl;
    os << "Scale     : " << xscale_ << " " << yscale_ << std::endl;
    os << "Aspect    : " << preserveAspect_ << std::endl;
  }

  friend std::ostream &operator<<(std::ostream &os, const CSVGBlockData &d) {
    d.print(os);

    return os;
  }

 private:
  CBBox2D            pixelBBox_;
  CBBox2D            viewBBox_;
  CPoint2D           offset_;
  double             xscale_ { 1 };
  double             yscale_ { 1 };
  CSVGPreserveAspect preserveAspect_;
  CMatrixStack2D     viewMatrix_;
};

#endif
