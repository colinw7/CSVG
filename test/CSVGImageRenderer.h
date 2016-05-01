#ifndef CSVGImageRenderer_H
#define CSVGImageRenderer_H

#include <CSVGRenderer.h>
#include <CImageRenderer2D.h>

class CSVGImageRenderer : public CSVGRenderer {
 public:
  CSVGImageRenderer(CImageRenderer2D *renderer) :
   renderer_(renderer) {
  }

  CSVGImageRenderer *dup() const override {
    return new CSVGImageRenderer(renderer_);
  }

  void setSize(int width, int height) override {
    renderer_->updateSize(width, height);
  }

  void getSize(int *width, int *height) const override {
    *width  = renderer_->getPixelRenderer()->getWidth ();
    *height = renderer_->getPixelRenderer()->getHeight();
  }

  void setDataRange(double xmin, double ymin, double xmax, double ymax) override {
    renderer_->setDataRange(xmin, ymin, xmax, ymax);
  }

  void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const override {
    renderer_->getDataRange(xmin, ymin, xmax, ymax);
  }

  void setBackground(const CRGBA &) override { }

  bool isAntiAlias() const override { return renderer_->isAntiAlias(); }
  void setAntiAlias(bool b) override { renderer_->setAntiAlias(b); }

  void setEqualScale(bool b) override { renderer_->setEqualScale(b); }

  void setScaleMin(bool b) override { renderer_->setScaleMin(b); }

  void beginDraw() override { renderer_->beginDraw(); }
  void endDraw  () override { renderer_->endDraw  (); }

  void setViewMatrix(const CMatrix2D &m) override { renderer_->setViewMatrix(m); }

  void clear(const CRGBA &c) override { renderer_->setBackground(c); renderer_->clear(); }

  void pathInit() override { renderer_->pathInit(); }
  void pathMoveTo(const CPoint2D &p) override { renderer_->pathMoveTo(p); }
  void pathRMoveTo(const CPoint2D &p) override { renderer_->pathRMoveTo(p); }
  void pathLineTo(const CPoint2D &p) override { renderer_->pathLineTo(p); }
  void pathRLineTo(const CPoint2D &p) override { renderer_->pathRLineTo(p); }

  void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2) override {
    renderer_->pathBezier2To(p1, p2); }
  void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2) override {
    renderer_->pathRBezier2To(p1, p2); }
  void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) override {
    renderer_->pathBezier3To(p1, p2, p3); }
  void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) override {
    renderer_->pathRBezier3To(p1, p2, p3); }

  void pathArcTo(const CPoint2D &c, double rx, double ry, double a1, double a2) override {
    renderer_->pathArc(c, rx, ry, a1, a2);
  }

  void pathText(const std::string &) override { std::cerr << "pathText" << std::endl; }

  void pathClose() override { renderer_->pathClose(); }

  bool pathGetCurrentPoint(CPoint2D &p) override { return renderer_->pathGetCurrentPoint(p); }

  void pathStroke() override { renderer_->pathStroke(); }
  void pathFill  () override { renderer_->pathFill  (); }

  void savePath(const CMatrix2D &) override { std::cerr << "savePath" << std::endl; }

  void pathClip  (CSVGRenderer *) override { renderer_->pathClip  (); }
  void pathEoclip(CSVGRenderer *) override { renderer_->pathEoclip(); }

  void initClip() override { renderer_->initClip(); }

  void addClipPath(CSVGRenderer *) override { std::cerr << "addClipPath" << std::endl; }

  void pathBBox(CBBox2D &bbox) override { renderer_->pathBBox(bbox); }

  void drawImage(const CPoint2D &p, CImagePtr image) override { renderer_->drawImage(p, image); }

  void setFont(CFontPtr f) override { renderer_->setFont(f); }

  void setStrokeColor(const CRGBA &c) override { renderer_->setForeground(c); }
  void setLineWidth(double w) override { renderer_->setLineWidth(w); }
  void setLineDash(const CLineDash &d) override { renderer_->setLineDash(d); }
  void setLineCap(const CLineCapType &c) override { renderer_->setLineCap(c); }
  void setLineJoin(const CLineJoinType &j) override { renderer_->setLineJoin(j); }
  void setMitreLimit(double l) override { renderer_->setMitreLimit(l); }

  void setFillType(CFillType t) override { renderer_->setFillType(t); }
  void setFillColor(const CRGBA &c) override { renderer_->setForeground(c); }
  void setFillGradient(CGenGradient *) override { }
  void setFillImage(CImagePtr) override { }

  void setAlign(CHAlignType halign, CVAlignType valign) override {
    renderer_->setAlign(halign, valign); }

  void windowToPixel(const CPoint2D &w, CPoint2D &p) override { renderer_->windowToPixel(w, p); }
  void pixelToWindow(const CPoint2D &p, CPoint2D &w) override { renderer_->pixelToWindow(p, w); }

  void textBounds(const std::string &text, CBBox2D &bbox) override {
    renderer_->textBounds(text, bbox); }

  CISize2D getImageSize() const override {
    return CISize2D(renderer_->getPixelWidth(), renderer_->getPixelHeight());
  }

  CImagePtr getImage() const override { return renderer_->getImage(); }
  void setImage(CImagePtr image) override { return renderer_->setImage(image); }

 private:
  CImageRenderer2D *renderer_ { 0 };
};

#endif
