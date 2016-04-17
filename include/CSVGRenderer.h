#ifndef CSVG_RENDERER_H
#define CSVG_RENDERER_H

#include <CMatrix2D.h>
#include <CBBox2D.h>
#include <CGenGradient.h>
#include <CFont.h>
#include <CImageLib.h>
#include <CRGBA.h>
#include <CLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CFillType.h>

class CSVGRenderer {
 public:
  CSVGRenderer();

  virtual ~CSVGRenderer();

  virtual CSVGRenderer *dup() const = 0;

  virtual void setSize(int width, int height) = 0;
  virtual void getSize(int *width, int *height) const = 0;

  virtual void setDataRange(double xmin, double ymin, double xmax, double ymax) = 0;
  virtual void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const = 0;

  virtual bool isAntiAlias() const = 0;
  virtual void setAntiAlias(bool aa) = 0;

  virtual void setEqualScale(bool eq) = 0;
  virtual void setScaleMin(bool sm) = 0;

  virtual void beginDraw() { }
  virtual void endDraw  () { }

  virtual void startDraw() { }
  virtual void stopDraw () { }

  virtual void setViewMatrix(const CMatrix2D &m) = 0;

  virtual void setBackground(const CRGBA &c) = 0;

  virtual void clear(const CRGBA &bg) = 0;

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

  virtual void pathClip  (CSVGRenderer *renderer=0) = 0;
  virtual void pathEoclip(CSVGRenderer *renderer=0) = 0;

  virtual void initClip() = 0;

  virtual void pathBBox(CBBox2D &bbox) = 0;

  virtual void drawImage(const CPoint2D &p, CImagePtr image) = 0;

  virtual void setFont(CFontPtr bg) = 0;

  virtual void setStrokeColor(const CRGBA &fg) = 0;

  virtual void setLineWidth(double w) = 0;
  virtual void setLineDash(const CLineDash &dash) = 0;
  virtual void setLineCap(const CLineCapType &cap) = 0;
  virtual void setLineJoin(const CLineJoinType &join) = 0;
  virtual void setMitreLimit(double limit) = 0;

  virtual void setFillType(CFillType fillType) = 0;
  virtual void setFillColor(const CRGBA &bg) = 0;
  virtual void setFillGradient(CGenGradient *g) = 0;
  virtual void setFillImage(CImagePtr image) = 0;

  virtual void setAlign(CHAlignType halign, CVAlignType valign) = 0;

  virtual void windowToPixel(const CPoint2D &w, CPoint2D &p) = 0;
  virtual void pixelToWindow(const CPoint2D &p, CPoint2D &w) = 0;

  virtual void textBounds(const std::string &str, CBBox2D &bbox) = 0;

  virtual CISize2D getImageSize() const = 0;

  virtual CImagePtr getImage() const = 0;
  virtual void      setImage(CImagePtr image) = 0;
};

#endif
