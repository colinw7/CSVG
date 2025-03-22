#ifndef CSVG_RENDERER_H
#define CSVG_RENDERER_H

#include <CSVGTypes.h>
#include <CMatrix2D.h>
#include <CBBox2D.h>
#include <CGenGradient.h>
#include <CFont.h>
#include <CRGBA.h>
#include <CLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CFillType.h>
#include <CAlignType.h>

class CSVGImageData;
class CSVGFontDef;

class CSVGRenderer {
 public:
  CSVGRenderer();

  virtual ~CSVGRenderer();

  virtual CSVGRenderer *dup() const = 0;

  //! get/set alpha
  bool isAlpha() const { return alpha_; }
  void setAlpha(bool b) { alpha_ = b; }

  //! get/set opacity
  double opacity() const { return opacity_; }
  void setOpacity(double r) { opacity_ = r; }

  //! get/set pixel size
  virtual void setSize(int width, int height) = 0;
  virtual void getSize(int *width, int *height) const = 0;

  //! set pixel range
  virtual void setPixelRange(int width, int height) = 0;

  //! get/set data range
  virtual void setDataRange(double xmin, double ymin, double xmax, double ymax) = 0;
  virtual void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const = 0;

  //! get/set anti alias
  virtual bool isAntiAlias() const = 0;
  virtual void setAntiAlias(bool aa) = 0;

  //! set equal scale
  virtual void setEqualScale(bool eq) = 0;
  //! set scale to minimum
  virtual void setScaleMin(bool sm) = 0;

  //! begin/end draw
  virtual void beginDraw() { }
  virtual void endDraw  () { }

  //! start/stop draw
  virtual void startDraw() { }
  virtual void stopDraw () { }

  //! set view matrix
  virtual void setViewMatrix(const CMatrix2D &m) = 0;

  //! set background color
  virtual void setBackground(const CRGBA &c) = 0;

  //! clear to background color
  virtual void clear(const CRGBA &bg) = 0;

  //--- path APIS

  virtual void pathInit() = 0;
  virtual void pathMoveTo(const CPoint2D &p) = 0;
  virtual void pathRMoveTo(const CPoint2D &p) = 0;
  virtual void pathLineTo(const CPoint2D &p) = 0;
  virtual void pathRLineTo(const CPoint2D &p) = 0;
  virtual void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2) = 0;
  virtual void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2) = 0;
  virtual void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;
  virtual void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;
  virtual void pathArcTo(const CPoint2D &c, double xr, double yr, double a1, double a2) = 0;
  virtual void pathText(const std::string &str) = 0;
  virtual void pathClose() = 0;

  virtual bool pathGetCurrentPoint(CPoint2D &p) = 0;

  virtual void pathStroke() = 0;
  virtual void pathFill() = 0;

  virtual void savePath(const CMatrix2D &m=CMatrix2D(CMatrix2D::Type::IDENTITY)) = 0;

  virtual void pathClip  (CSVGRenderer *renderer=nullptr) = 0;
  virtual void pathEoclip(CSVGRenderer *renderer=nullptr) = 0;

  //---

  virtual void initClip() = 0;

  virtual void addClipPath(CSVGRenderer *renderer) = 0;

  virtual void pathBBox(CBBox2D &bbox) = 0;

  virtual void drawImage(const CPoint2D &p, CSVGImageData *image) = 0;

  //! set font
  virtual void setFont(const CSVGFontDef &fontDef) = 0;

  //--- stroke APIS

  virtual void resetStroke() = 0;

  virtual void setStrokeColor(const CRGBA &fg) = 0;

  virtual void setLineWidth(double w) = 0;
  virtual void setLineDash(const CLineDash &dash) = 0;
  virtual void setLineCap(const CLineCapType &cap) = 0;
  virtual void setLineJoin(const CLineJoinType &join) = 0;
  virtual void setMitreLimit(double limit) = 0;

  //--- fill APIS

  virtual void resetFill() = 0;
  virtual void setFillType(CFillType fillType) = 0;
  virtual void setFillColor(const CRGBA &bg) = 0;
  virtual void setFillGradient(CGenGradient *g) = 0;
  virtual void setFillImage(double xo, double yo, CSVGImageData *image) = 0;
  virtual void setFillMatrix(const CMatrix2D &m) = 0;

  //--- stroke fill APIS

  virtual void setStrokeFilled(bool b) = 0;
  virtual void setStrokeFillType(CFillType fillType) = 0;
  virtual void setStrokeFillGradient(CGenGradient *g) = 0;
  virtual void setStrokeFillImage(CSVGImageData *image) = 0;

  //---

  virtual void setAlign(CHAlignType halign, CVAlignType valign) = 0;

  //---

  //--- coordinate conversion APIS

  virtual void windowToPixel(const CPoint2D &w, CPoint2D &p) = 0;
  virtual void pixelToWindow(const CPoint2D &p, CPoint2D &w) = 0;

  void lengthToPixel(const CPoint2D &w, CPoint2D &p) {
    CPoint2D o(0, 0);

    CPoint2D p1, p2;

    windowToPixel(o, p1);
    windowToPixel(w, p2);

    p = CPoint2D(fabs(p2.x - p1.x), fabs(p2.y - p1.y));
  }

  //---

  virtual void textBounds(const std::string &str, CBBox2D &bbox) = 0;

  //---

  virtual CISize2D getImageSize() const = 0;

  virtual CSVGImageData *getImage() const = 0;

  virtual void setImage(CSVGRenderer *renderer) = 0;
  virtual void setImage(CSVGImageData *image) = 0;

  //---

  virtual void addResizedImage(CSVGRenderer *src, double x, double y, double w, double h);

  virtual void addClippedImage(CSVGRenderer *src, int x, int y,
                               int px1, int py1, int px2, int py2);
  virtual void setClippedImage(CSVGRenderer *src, int px1, int py1, int px2, int py2);

  virtual void setOffsetImage(CSVGRenderer *src, int dx, int dy);

  virtual void addImage(int x, int y, CSVGImageData *image);

  //--

  virtual void combine(int x, int y, CSVGRenderer *r);

  //---

  virtual void gaussianBlur(CSVGRenderer *dst, CBBox2D &bbox, double stdDevX, double stdDevY);

  virtual void blend(CSVGRenderer *in2, CSVGBlendMode mode, CSVGRenderer *dst);

 protected:
  bool   alpha_   { false };
  double opacity_ { 1.0 };
};

#endif
