#ifndef CSVG_BUFFER_H
#define CSVG_BUFFER_H

#include <CImage.h>

class CSVGRenderer;
class CSVGBuffer;

class CSVGBufferMgr {
 private:
  typedef std::map<std::string,CSVGBuffer*> BufferMap;

  CSVG      &svg_;
  bool       anti_alias_;
  BufferMap  buffer_map_;

 public:
  CSVGBufferMgr(CSVG &svg);
 ~CSVGBufferMgr();

  void setAntiAlias(bool flag) { anti_alias_ = flag; }

  CSVGBuffer *lookupBuffer(const std::string &name);
  CSVGBuffer *createBuffer(const std::string &name);

 private:
  void addBuffer(CSVGBuffer *buffer);
};

class CSVGBuffer {
 private:
  CSVG         &svg_;
  std::string   name_;
  CSVGRenderer *renderer_;
  CLineDash     lineDash_;

 public:
  CSVGBuffer(CSVG &svg, const std::string &name);
 ~CSVGBuffer();

  const std::string &getName() const { return name_; }

  void setAntiAlias(bool flag);

  CImagePtr getImage() const;
  CImagePtr getAlphaImage() const;

  void setImage(CImagePtr image);
  void setAlphaImage(CImagePtr image);

  void reset();

  void setup(const CBBox2D &bbox);

  CSVGRenderer *getRenderer() const;

  void beginDraw(int w, int h, const CBBox2D &bbox);
  void endDraw();

  void setAlign(CHAlignType halign, CVAlignType valign);
  void setEqualScale(bool equalScale);
  void setScaleMin(bool scale);

  void setViewMatrix(const CMatrix2D &matrix);

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

  void pathClip();
  void pathEoClip();

  void pathBBox(CBBox2D &bbox);

  void initClip();

  bool mmToPixel(double mm, double *pixel);
  void windowToPixel(double xi, double yi, int *xo, int *yo);

 private:
  CSVGBuffer(const CSVGBuffer &rhs);
  CSVGBuffer &operator=(const CSVGBuffer &rhs);
};

#endif
