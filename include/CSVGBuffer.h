#ifndef CSVG_BUFFER_H
#define CSVG_BUFFER_H

#include <CImage.h>
#include <CFont.h>
#include <CBBox2D.h>
#include <CMatrixStack2D.h>
#include <CLineDash.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CFillType.h>
#include <map>

class CSVG;
class CSVGRenderer;
class CSVGBuffer;
class CGenGradient;

//------

class CSVGBufferMgr {
 public:
  CSVGBufferMgr(CSVG &svg);
 ~CSVGBufferMgr();

  bool isAntiAlias() const { return antiAlias_; }
  void setAntiAlias(bool flag) { antiAlias_ = flag; }

  CSVGBuffer *lookupBuffer(const std::string &name, bool create=false);
  CSVGBuffer *createBuffer(const std::string &name);

  CSVGBuffer *lookupAlphaBuffer(CSVGBuffer *refBuffer, bool create=false);
  CSVGBuffer *createAlphaBuffer(CSVGBuffer *refBuffer);

  void getBufferNames(std::vector<std::string> &names) const;

 private:
  void addBuffer(CSVGBuffer *buffer);
  void addAlphaBuffer(CSVGBuffer *buffer);

 private:
  typedef std::map<std::string,CSVGBuffer*> BufferMap;

  CSVG&     svg_;
  bool      antiAlias_ { true };
  BufferMap bufferMap_;
  BufferMap alphaBufferMap_;
};

//------

class CSVGBuffer {
 public:
  CSVGBuffer(CSVG &svg, const std::string &name);
  CSVGBuffer(CSVGBuffer *refBuffer);

 ~CSVGBuffer();

  const std::string &getName() const { return name_; }

  bool isAntiAlias() const;
  void setAntiAlias(bool flag);

  const CMatrixStack2D &transform() const { return transform_; }
  void setTransform(const CMatrixStack2D &v);
  void unsetTransform();

  const CPoint2D &origin() const { return origin_; }
  void setOrigin(const CPoint2D &o) { origin_ = o; }

  const CBBox2D &bbox() const { return bbox_; }
  void setBBox(const CBBox2D &b) { bbox_ = b; }

  bool isDrawing() const { return drawing_; }
  void setDrawing(bool b) { drawing_ = b; }

  CISize2D getImageSize() const;

  CImagePtr getImage() const;

  void setImage(CImagePtr image);

  void addImage(CImagePtr image);
  void addImage(double x, double y, CImagePtr image);

  void reset();

  void clear();

  void setup(const CBBox2D &bbox);

  CSVGRenderer *getRenderer() const;

  void beginDraw(double w, double h, const CBBox2D &bbox);
  void endDraw();

  void startDraw();
  void stopDraw ();

  void setAlign(CHAlignType halign, CVAlignType valign);
  void setEqualScale(bool equalScale);
  void setScaleMin(bool scale);

  void setViewMatrix(const CMatrixStack2D &matrix);

  void fill(const CRGBA &bg);

  void setStrokeColor(const CRGBA &color);
  void setLineWidth(double width);
  void setLineDash(const CLineDash &dash);
  void setLineDashOffset(double offset);
  void setLineCap(CLineCapType line_cap);
  void setLineJoin(CLineJoinType line_join);
  void setLineMitreLimit(double limit);

  void setFillColor(const CRGBA &color);
  void setFillType(CFillType type);
  void setFillGradient(CGenGradient *g);
  void setFillImage(CImagePtr image);

  void drawImage(double x, double y, CImagePtr image);
  void drawImage(const CBBox2D &bbox, CImagePtr image);

  void pathInit();
  void pathTerm();
  void pathMoveTo(double x, double y);
  void pathRMoveTo(double x, double y);
  void pathLineTo(double x, double y);
  void pathRLineTo(double dx, double dy);
  void pathArcTo(double cx, double cy, double rx, double ry, double theta1, double theta2);
  void pathBezier2To(double x1, double y1, double x2, double y2);
  void pathRBezier2To(double x1, double y1, double x2, double y2);
  void pathBezier3To(double x1, double y1, double x2, double y2, double x3, double y3);
  void pathRBezier3To(double x1, double y1, double x2, double y2, double x3, double y3);
  void pathText(const std::string &text, CFontPtr font, CHAlignType align);
  void pathClose();

  bool pathGetCurrentPoint(double *x, double *y);

  void pathStroke();
  void pathFill();

  void pathClip  (CSVGBuffer *buffer=0);
  void pathEoClip(CSVGBuffer *buffer=0);

  void pathBBox(CBBox2D &bbox);

  void initClip();

  bool mmToPixel(double mm, double *pixel);

  void lengthToPixel(double xi, double yi, double *xo, double *yo);
  void windowToPixel(double xi, double yi, double *xo, double *yo);

 private:
  CSVGBuffer(const CSVGBuffer &rhs);
  CSVGBuffer &operator=(const CSVGBuffer &rhs);

 private:
  CSVG&          svg_;
  std::string    name_;
  CSVGRenderer*  renderer_ { 0 };
  CMatrixStack2D transform_;
  CLineDash      lineDash_;
  CPoint2D       origin_ { 0, 0 };
  CBBox2D        bbox_;
  bool           drawing_ { false };
  CSVGBuffer*    refBuffer_ { 0 };
};

#endif
