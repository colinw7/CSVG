#ifndef CSVG_BUFFER_H
#define CSVG_BUFFER_H

#include <CSVGTypes.h>
#include <CSVGLightData.h>
#include <CSVGPreserveAspect.h>
#include <CSVGConvolveData.h>
#include <CSVGFill.h>
#include <CSVGStroke.h>
#include <CSVGImageData.h>

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
class CSVGFontDef;
class CSVGObject;

class CGenGradient;
class CFile;

//------

class CSVGBufferMgr {
 public:
  CSVGBufferMgr(CSVG &svg);
 ~CSVGBufferMgr();

  bool isAntiAlias() const { return antiAlias_; }
  void setAntiAlias(bool flag) { antiAlias_ = flag; }

  const CBBox2D &paintBBox() const { return paintBBox_; }
  void setPaintBBox(const CBBox2D &v) { paintBBox_ = v; }

  const CSVGFill &paintFill() const { return paintFill_; }
  void setPaintFill(const CSVGFill &f) { paintFill_ = f; }

  const CSVGStroke &paintStroke() const { return paintStroke_; }
  void setPaintStroke(const CSVGStroke &s) { paintStroke_ = s; }

  void clear();

  CSVGBuffer *lookupBuffer(const std::string &name, bool create=false);
  CSVGBuffer *createBuffer(const std::string &name);

  CSVGBuffer *lookupAlphaBuffer(CSVGBuffer *refBuffer, bool create=false);
  CSVGBuffer *createAlphaBuffer(CSVGBuffer *refBuffer);

  void getBufferNames(std::vector<std::string> &names, bool includeAlpha=true) const;

 private:
  void addBuffer(CSVGBuffer *buffer);
  void addAlphaBuffer(CSVGBuffer *buffer);

 private:
  using BufferMap = std::map<std::string, CSVGBuffer*>;

  CSVG&      svg_;
  bool       antiAlias_ { true };
  BufferMap  bufferMap_;
  BufferMap  alphaBufferMap_;
  CBBox2D    paintBBox_;
  CSVGFill   paintFill_;
  CSVGStroke paintStroke_;
};

//------

class CSVGBuffer {
 public:
  using FeFuncs = std::vector<CSVGFeFunc *>;

 public:
  CSVGBuffer(CSVG &svg, const std::string &name);
  CSVGBuffer(CSVGBuffer *refBuffer);

 ~CSVGBuffer();

  CSVGBuffer *dup() const;

  const std::string &getName() const { return name_; }

  CSVGBuffer *parentBuffer() const { return parentBuffer_; }
  void setParentBuffer(CSVGBuffer *p) { parentBuffer_ = p; }

  CSVGRenderer *renderer() const { return renderer_; }

  bool isAntiAlias() const;
  void setAntiAlias(bool flag);

  bool isAlpha() const { return alpha_; }
  void setAlpha(bool b);

  double opacity() const { return opacity_.getValue(1); }
  void setOpacity(double r);

  //------

  // filter processing
  static void blendBuffers(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, const CBBox2D &bbox,
                           CSVGBlendMode mode, CSVGBuffer *outBuffer);

  static void colorMatrixBuffers(CSVGBuffer *inBuffer, const CBBox2D &bbox,
                                 CSVGColorMatrixType type, const std::vector<double> &values,
                                 CSVGBuffer *outBuffer);

  static void componentTransferBuffers(CSVGBuffer *inBuffer, const CBBox2D &bbox,
                                       const FeFuncs &funcs, CSVGBuffer *outBuffer);

  static void compositeBuffers(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, const CBBox2D &bbox,
                               CRGBACombineFunc func, double k1, double k2, double k3, double k4,
                               CSVGBuffer *outBuffer);

  static void convolveMatrixBuffers(CSVGBuffer *inBuffer, const CBBox2D &bbox,
                                    const CSVGConvolveData &data);

  static void displacementMapBuffers(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2,
                                     const CBBox2D &bbox, const std::string &xchannel,
                                     const std::string &ychannel, double scale,
                                     CSVGBuffer *outBuffer);

  static void floodBuffers(const CRGBA &c, const CBBox2D &bbox, CSVGBuffer *outBuffer);

  static void gaussianBlurBuffers(CSVGBuffer *inBuffer, const CBBox2D &inBBox,
                                  double stdDevX, double stdDevY, CSVGBuffer *outBuffer);

  static void imageBuffers(CSVGBuffer *inBuffer, const CBBox2D &inBBox,
                           CSVGPreserveAspect preserveAspect, CSVGBuffer *outBuffer);

  static void maskBuffers(CSVGBuffer *oldBuffer, CSVGBuffer *buffer,
                          const CSVGObject *object, double x, double y);

  static void mergeBuffers(const std::vector<CSVGFeMergeNode *> &nodes, int w, int h,
                           CSVGBuffer *outBuffer);

  static void morphologyBuffers(CSVGBuffer *inBuffer, const CBBox2D &inBBox,
                                CSVGMorphologyOperator op, int r);

  static void offsetBuffers(CSVGBuffer *inBuffer, const CBBox2D &bbox, double dx, double dy,
                            CSVGBuffer *outBuffer);

  static void tileBuffers(CSVGBuffer *inBuffer, const CBBox2D &inBBox,
                          const CBBox2D &outBBox, CSVGBuffer *outBuffer);

  static void turbulenceBuffers(CSVGBuffer *inBuffer, const CBBox2D &inBBox, bool fractalNoise,
                                double baseFreqX, double baseFreqY, int numOctaves, int seed,
                                CSVGBuffer *outBuffer);

  static void lightBuffers(CSVGBuffer *inBuffer, const CBBox2D &bbox,
                           const std::vector<CSVGFilterBase *> &lights,
                           const CSVGLightData &lightData, CSVGBuffer *outBuffer);

  //------

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

  CSVGImageDataP getImage() const;

  void setImageFile(const std::string &filename);
  void setImageFile(CFile &file);

  void setFlatImageBuffer(CSVGBuffer *buffer);

  void setImageBuffer(double x, double y, CSVGBuffer *buffer);
  void setImageBuffer(CSVGBuffer *buffer);

  void addReshapeImage(CSVGBuffer *buffer, double x1, double y1, int pw, int ph);

  void addImageBuffer(double x, double y, CSVGBuffer *buffer);
  void addImageBuffer(CSVGBuffer *buffer);

  void addImage(double x, double y, CSVGImageDataP &image);

  void reset();

  void clear();

  void setup(const CBBox2D &bbox);

  CSVGRenderer *getRenderer() const;

  void beginDraw(double w, double h, const CBBox2D &bbox);
  void endDraw();

  void startDraw();
  void stopDraw ();

  void updateBBoxSize(const CBBox2D &bbox);

  void setAlign(CHAlignType halign, CVAlignType valign);
  void setEqualScale(bool equalScale);
  void setScaleMin(bool scale);

  void setViewMatrix(const CMatrixStack2D &matrix);

  void fill(const CRGBA &bg);

  void setStroke(const CSVGStroke &stroke);

  void resetStroke();
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

  void setFill(const CSVGFill &fill);

  void resetFill();
  void setFillColor(const CRGBA &color);
  void setFillType(CFillType type);
  void setFillGradient(CGenGradient *g);
  void setFillBuffer(double x, double y, CSVGBuffer *buffer);
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
  void pathText(const std::string &text, const CSVGFontDef &fontDef, CHAlignType align);
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

 private:
  static void distantLight(CSVGImageDataP &image, CSVGFeDistantLight *pl,
                           CSVGLightData &lightData);

  static void pointLight(CSVGImageDataP &image, CSVGFePointLight *pl,
                         CSVGLightData &lightData);

  static void spotLight(CSVGImageDataP &image, CSVGFeSpotLight *pl,
                        CSVGLightData &lightData);

  static CRGBA lightPoint(CSVGImageDataP &image, int x, int y,
                          const CSVGLightData &lightData);

  CSVGImageDataP subImage(const CBBox2D &bbox) const;

  void putImage(const CBBox2D &bbox, const CSVGImageDataP &image);

 private:
  CSVGBuffer(const CSVGBuffer &rhs);
  CSVGBuffer &operator=(const CSVGBuffer &rhs);

 private:
  using OptPoint2D = COptValT<CPoint2D>;

  CSVG&          svg_;
  std::string    name_;
  CSVGBuffer*    refBuffer_    { nullptr };
  CSVGBuffer*    parentBuffer_ { nullptr };
  CSVGRenderer*  renderer_     { nullptr };
  bool           alpha_        { false };
  COptReal       opacity_;
  CMatrixStack2D transform_;
  CLineDash      lineDash_;
  CPoint2D       origin_       { 0, 0 };
  CBBox2D        bbox_;
  OptPoint2D     pathLastControlPoint1_;
  OptPoint2D     pathLastControlPoint2_;
  bool           clip_         { false };
  bool           hasClipPath_  { false };
  bool           drawing_      { false };
};

#endif
