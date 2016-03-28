#ifndef CSVGImageRenderer_H
#define CSVGImageRenderer_H

#include <CSVGRenderer.h>
#include <CImageRenderer2D.h>

class CSVGImageRenderer : public CSVGRenderer {
 public:
  CSVGImageRenderer(CImageRenderer2D *renderer) :
   renderer_(renderer) {
  }

  CSVGImageRenderer *dup() const {
    return new CSVGImageRenderer(renderer_);
  }

  void setSize(int width, int height) {
    renderer_->updateSize(width, height);
  }

  void getSize(int *width, int *height) const {
    *width  = renderer_->getPixelRenderer()->getWidth ();
    *height = renderer_->getPixelRenderer()->getHeight();
  }

  void setDataRange(double xmin, double ymin, double xmax, double ymax) {
    renderer_->setDataRange(xmin, ymin, xmax, ymax);
  }

  void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const {
    renderer_->getDataRange(xmin, ymin, xmax, ymax);
  }

  void setBackground(const CRGBA &) { }

  void setAntiAlias(bool b) { renderer_->setAntiAlias(b); }

  void setEqualScale(bool b) { renderer_->setEqualScale(b); }

  void setScaleMin(bool b) { renderer_->setScaleMin(b); }

  void beginDraw() { renderer_->beginDraw(); }
  void endDraw  () { renderer_->endDraw  (); }

  void setViewMatrix(const CMatrix2D &m) { renderer_->setViewMatrix(m); }

  void clear(const CRGBA &c) { renderer_->setBackground(c); renderer_->clear(); }

  void pathInit() { renderer_->pathInit(); }
  void pathMoveTo(const CPoint2D &p) { renderer_->pathMoveTo(p); }
  void pathRMoveTo(const CPoint2D &p) { renderer_->pathRMoveTo(p); }
  void pathLineTo(const CPoint2D &p) { renderer_->pathLineTo(p); }
  void pathRLineTo(const CPoint2D &p) { renderer_->pathRLineTo(p); }

  void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2) {
    renderer_->pathBezier2To(p1, p2); }
  void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2) {
    renderer_->pathRBezier2To(p1, p2); }
  void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    renderer_->pathBezier3To(p1, p2, p3); }
  void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) {
    renderer_->pathRBezier3To(p1, p2, p3); }

  void pathArcTo(const CPoint2D &c, double rx, double ry, double a1, double a2) {
    renderer_->pathArc(c, rx, ry, a1, a2);
  }

  void pathText(const std::string &) { std::cerr << "pathText" << std::endl; }

  void pathClose() { renderer_->pathClose(); }

  bool pathGetCurrentPoint(CPoint2D &p) { return renderer_->pathGetCurrentPoint(p); }

  void pathStroke() { renderer_->pathStroke(); }
  void pathFill  () { renderer_->pathFill  (); }
  void pathClip  () { renderer_->pathClip  (); }
  void pathEoclip() { renderer_->pathEoclip(); }

  void initClip() { renderer_->initClip(); }

  void pathBBox(CBBox2D &bbox) { renderer_->pathBBox(bbox); }

  void drawImage(const CPoint2D &p, CImagePtr image) { renderer_->drawImage(p, image); }

  void setFont(CFontPtr f) { renderer_->setFont(f); }

  void setStrokeColor(const CRGBA &c) { renderer_->setForeground(c); }
  void setLineWidth(double w) { renderer_->setLineWidth(w); }
  void setLineDash(const CLineDash &d) { renderer_->setLineDash(d); }
  void setLineCap(const CLineCapType &c) { renderer_->setLineCap(c); }
  void setLineJoin(const CLineJoinType &j) { renderer_->setLineJoin(j); }
  void setMitreLimit(double l) { renderer_->setMitreLimit(l); }

  void setFillType(CFillType t) { renderer_->setFillType(t); }

  void setFillColor(const CRGBA &c) { renderer_->setForeground(c); }
  void setFillGradient(CGenGradient *) { }
  void setFillImage(CImagePtr) { }

  void setAlign(CHAlignType halign, CVAlignType valign) { renderer_->setAlign(halign, valign); }

  void windowToPixel(const CPoint2D &w, CPoint2D &p) { renderer_->windowToPixel(w, p); }

  void textBounds(const std::string &text, CBBox2D &bbox) { renderer_->textBounds(text, bbox); }

  CImagePtr getImage() const { return renderer_->getImage(); }

  void setImage(CImagePtr image) { return renderer_->setImage(image); }

 private:
  CImageRenderer2D *renderer_;
};

#endif
