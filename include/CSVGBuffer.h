#ifndef CSVG_BUFFER_H
#define CSVG_BUFFER_H

#include <CSVGTypes.h>
#include <CSVGLightData.h>
#include <CSVGPreserveAspect.h>
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
class CSVGFeFunc;
class CSVGRenderer;
class CSVGBuffer;
class CSVGFilterBase;
class CSVGFeDistantLight;
class CSVGFePointLight;
class CSVGFeSpotLight;
class CSVGFeMergeNode;
class CSVGObject;
class CGenGradient;

//------

class CSVGBufferMgr {
 public:
  CSVGBufferMgr(CSVG &svg);
 ~CSVGBufferMgr();

  bool isAntiAlias() const { return antiAlias_; }
  void setAntiAlias(bool flag) { antiAlias_ = flag; }

  void clear();

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
  typedef std::vector<CSVGFeFunc *> FeFuncs;

 public:
  CSVGBuffer(CSVG &svg, const std::string &name);
  CSVGBuffer(CSVGBuffer *refBuffer);

 ~CSVGBuffer();

  CSVGBuffer *dup() const;

  const std::string &getName() const { return name_; }

  bool isAntiAlias() const;
  void setAntiAlias(bool flag);

  bool isAlpha() const { return refBuffer_; }

  static void blendBuffers(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2,
                           CSVGBlendMode mode, CSVGBuffer *outBuffer);
  static void colorMatrixBuffers(CSVGBuffer *inBuffer, CSVGColorMatrixType type,
                                 const std::vector<double> &values, CSVGBuffer *outBuffer);
  static void componentTransferBuffers(CSVGBuffer *inBuffer, const FeFuncs &funcs,
                                       CSVGBuffer *outBuffer);
  static void compositeBuffers(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, CRGBACombineFunc func,
                               double k1, double k2, double k3, double k4, CSVGBuffer *outBuffer);
  static void convolveMatrixBuffers(CSVGBuffer *inBuffer,
                                    const std::vector<double> &kernelMatrix);
  static void displacementMapBuffers(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2,
                                     const std::string &xchannel, const std::string &ychannel,
                                     double scale, CSVGBuffer *outBuffer);
  static void floodBuffers(const CRGBA &c, int w, int h, CSVGBuffer *outBuffer);
  static void gaussianBlurBuffers(CSVGBuffer *inBuffer, CSVGFilterBase *filter,
                                  double stdDev, CSVGBuffer *outBuffer);
  static void imageBuffers(CSVGBuffer *inBuffer, CSVGFilterBase *filter,
                           const CMatrixStack2D &transform, CSVGPreserveAspect preserveAspect,
                           CSVGBuffer *outBuffer);
  static void maskBuffers(CSVGBuffer *oldBuffer, CSVGBuffer *buffer,
                          const CSVGObject *object, double x, double y);
  static void mergeBuffers(CSVGFilterBase *filter, const std::vector<CSVGFeMergeNode *> &nodes,
                           int w, int h, CSVGBuffer *outBuffer);
  static void morphologyBuffers(CSVGBuffer *inBuffer, CSVGMorphologyOperator op, int r);
  static void offsetBuffers(CSVGBuffer *inBuffer, double dx, double dy, CSVGBuffer *outBuffer);
  static void tileBuffers(CSVGBuffer *inBuffer, CSVGFilterBase *filter,
                          const CMatrixStack2D &transform, CSVGBuffer *outBuffer);
  static void turbulenceBuffers(CSVGBuffer *inBuffer, bool fractalNoise, double baseFreq,
                                int numOctaves, int seed, CSVGBuffer *outBuffer);

  static void lightBuffers(CSVGBuffer *inBuffer, const std::vector<CSVGFilterBase *> &lights,
                           const CSVGLightData &lightData, CSVGBuffer *outBuffer);

  const CMatrixStack2D &transform() const { return transform_; }
  void setTransform(const CMatrixStack2D &v);
  void unsetTransform();

  const CPoint2D &origin() const { return origin_; }
  void setOrigin(const CPoint2D &o) { origin_ = o; }

  const CBBox2D &bbox() const { return bbox_; }
  void setBBox(const CBBox2D &b) { bbox_ = b; }

  bool isClip() const { return clip_; }
  void setClip(bool b) { clip_ = b; }

  bool hasClipPath() const { return hasClipPath_; }
  void setHasClipPath(bool b) { hasClipPath_ = b; }

  bool isDrawing() const;
  void setDrawing(bool b);

  void addClippedBuffer(CSVGBuffer *buffer, double x, double y,
                        double px1, double py1, double px2, double py2);

  void setClippedBuffer(CSVGBuffer *buffer, double px1, double py1, double px2, double py2);

  void addBuffer(CSVGBuffer *buffer, double x, double y);

  CISize2D getImageSize() const;

  CImagePtr getImage() const;

  void setImageFile(const std::string &filename);
  void setImageFile(CFile &file);

  void setImage(double x, double y, CSVGBuffer *buffer);
  void setImage(CSVGBuffer *buffer);

  void addReshapeImage(CSVGBuffer *buffer, double x1, double y1, int pw, int ph);

  void addImage(double x, double y, CSVGBuffer *buffer);
  void addImage(double x, double y, const CImagePtr &image);
  void addImage(CSVGBuffer *buffer);

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
  void setStrokeFilled(bool b);
  void setStrokeFillType(CFillType type);
  void setStrokeFillGradient(CGenGradient *g);
  void setStrokeFillBuffer(CSVGBuffer *buffer);
  void setLineWidth(double width);
  void setLineDash(const CLineDash &dash);
  void setLineDashOffset(double offset);
  void setLineCap(CLineCapType line_cap);
  void setLineJoin(CLineJoinType line_join);
  void setLineMitreLimit(double limit);

  void resetFill();
  void setFillColor(const CRGBA &color);
  void setFillType(CFillType type);
  void setFillGradient(CGenGradient *g);
  void setFillBuffer(CSVGBuffer *buffer);
  void setFillMatrix(const CMatrix2D &m);

  void drawImage(double x, double y, CSVGBuffer *buffer);

  void drawRoundedRectangle(const CBBox2D &bbox, double rx, double ry);
  void fillRoundedRectangle(const CBBox2D &bbox, double rx, double ry);

  void drawRectangle(const CBBox2D &bbox);
  void fillRectangle(const CBBox2D &bbox);

  void pathInit();
  void pathTerm();
  void pathMoveTo(double x, double y);
  void pathRMoveTo(double dx, double dy);
  void pathLineTo(double x, double y);
  void pathRLineTo(double dx, double dy);
  void pathArcTo(double cx, double cy, double rx, double ry, double theta1, double theta2);
  void pathArcSegment(double xc, double yc, double angle1, double angle2,
                      double rx, double ry, double phi);
  void pathBezier2To(double x1, double y1, double x2, double y2);
  void pathRBezier2To(double x1, double y1, double x2, double y2);
  void pathBezier3To(double x1, double y1, double x2, double y2, double x3, double y3);
  void pathRBezier3To(double x1, double y1, double x2, double y2, double x3, double y3);
  void pathText(const std::string &text, CFontPtr font, CHAlignType align);
  void pathClose();

  bool pathGetCurrentPoint(double *x, double *y) const;
  bool pathGetCurrentPoint(CPoint2D &point) const;

  bool pathGetLastControlPoint(CPoint2D &p) const;
  bool pathGetLastMControlPoint(CPoint2D &p) const;
  bool pathGetLastMRControlPoint(CPoint2D &p) const;
  void pathSetLastControlPoint1(const CPoint2D &p);
  void pathSetLastControlPoint2(const CPoint2D &p);

  void pathStroke();
  void pathFill();

  void pathClip  (CSVGBuffer *buffer=0);
  void pathEoClip(CSVGBuffer *buffer=0);

  void addClipPath(CSVGBuffer *buffer);

  void pathBBox(CBBox2D &bbox);

  void initClip();

  CPoint2D pathMirrorPoint(const CPoint2D &p) const;

  //bool mmToPixel(double mm, double *pixel);

  //void lengthToPixel(double xi, double yi, double *xo, double *yo);
  //void windowToPixel(double xi, double yi, double *xo, double *yo);

 private:
  static void distantLight(CImagePtr image, CSVGFeDistantLight *pl, CSVGLightData &lightData);
  static void pointLight  (CImagePtr image, CSVGFePointLight *pl, CSVGLightData &lightData);
  static void spotLight   (CImagePtr image, CSVGFeSpotLight *pl, CSVGLightData &lightData);

  static CRGBA lightPoint(CImagePtr image, int x, int y, const CSVGLightData &lightData);

 private:
  CSVGBuffer(const CSVGBuffer &rhs);
  CSVGBuffer &operator=(const CSVGBuffer &rhs);

 private:
  CSVG&              svg_;
  std::string        name_;
  CSVGRenderer*      renderer_ { 0 };
  CMatrixStack2D     transform_;
  CLineDash          lineDash_;
  CPoint2D           origin_ { 0, 0 };
  CBBox2D            bbox_;
  COptValT<CPoint2D> pathLastControlPoint1_;
  COptValT<CPoint2D> pathLastControlPoint2_;
  bool               clip_ { false };
  bool               hasClipPath_ { false };
  bool               drawing_ { false };
  CSVGBuffer*        refBuffer_ { 0 };
};

#endif
