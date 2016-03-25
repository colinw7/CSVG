#ifndef CSVGLogRenderer_H
#define CSVGLogRenderer_H

#include <CSVGRenderer.h>

class CSVGLogRenderer : public CSVGRenderer {
 public:
  CSVGLogRenderer() : os_(&std::cout) { }

  CSVGLogRenderer *dup() const {
    return new CSVGLogRenderer();
  }

  std::ostream &os() const { return *os_; }

  void setSize(int width, int height) {
    width_  = width;
    height_ = height;
  }

  void getSize(int *width, int *height) const {
    *width  = width_;
    *height = height_;
  }

  void setDataRange(double xmin, double ymin, double xmax, double ymax) {
    xmin_ = xmin; ymin_ = ymin;
    xmax_ = xmax; ymax_ = ymax;
  }

  void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const {
    *xmin = xmin_; *ymin = ymin_;
    *xmax = xmax_; *ymax = ymax_;
  }

  void setAntiAlias(bool) { logNL("setAntiAlias"); }

  void setEqualScale(bool) { logNL("setEqualScale"); }

  void setScaleMin(bool) { logNL("setScaleMin"); }

  void beginDraw() { logNL("beginDraw"); }
  void endDraw  () { logNL("endDraw"); }

  void setViewMatrix(const CMatrix2D &) { logNL("setViewMatrix"); }

  void clear(const CRGBA &) { logNL("clear"); }

  void pathInit() { logNL("pathInit"); }
  void pathMoveTo(const CPoint2D &p) { log("pathMoveTo "); logT(p); }
  void pathRMoveTo(const CPoint2D &p) { log("pathRMoveTo "); logT(p); }
  void pathLineTo(const CPoint2D &p) { log("pathLineTo "); logT(p); }
  void pathRLineTo(const CPoint2D &p) { log("pathRLineTo "); logT(p); }
  void pathCurveTo(const CPoint2D &, const CPoint2D &) { logNL("pathCurveTo"); }
  void pathRCurveTo(const CPoint2D &, const CPoint2D &) { logNL("pathRCurveTo"); }
  void pathCurveTo(const CPoint2D &, const CPoint2D &, const CPoint2D &) { logNL("pathCurveTo"); }
  void pathRCurveTo(const CPoint2D &, const CPoint2D &, const CPoint2D &) { logNL("pathRCurveTo"); }
  void pathArcTo(const CPoint2D &, double, double, double, double) { logNL("pathArcTo"); }
  void pathText(const std::string &) { logNL("pathText"); }
  void pathClose() { logNL("pathClose"); }

  bool pathGetCurrentPoint(CPoint2D &) {
    return false;
  }

  void pathStroke() { logNL("pathStroke"); }
  void pathFill() { logNL("pathFill"); }
  void pathClip() { logNL("pathClip"); }
  void pathEoclip() { logNL("pathEoclip"); }

  void initClip() { logNL("initClip"); }

  void pathBBox(CBBox2D &) { logNL("pathBBox"); }

  void drawImage(const CPoint2D &, CImagePtr) { logNL("drawImage"); }

  void setFont(CFontPtr) { logNL("setFont"); }

  void setStrokeColor(const CRGBA &) { logNL("setStrokeColor"); }
  void setLineWidth(double) { logNL("setLineWidth"); }
  void setLineDash(const CLineDash &) { logNL("setLineDash"); }
  void setLineCap(const CLineCapType &) { logNL("setLineCap"); }
  void setLineJoin(const CLineJoinType &) { logNL("setLineJoin"); }
  void setMitreLimit(double) { logNL("setMitreLimit"); }

  void setFillType(CFillType) { logNL("setFillType"); }

  void setFillColor(const CRGBA &) { logNL("setFillColor"); }
  void setFillGradient(CGenGradient *) { logNL("setFillGradient"); }
  void setFillImage(CImagePtr) { logNL("setFillImage"); }

  void setAlign(CHAlignType, CVAlignType) { logNL("setAlign"); }

  void windowToPixel(const CPoint2D &, CPoint2D &) { logNL("windowToPixel"); }

  void textBounds(const std::string &, CBBox2D &) { logNL("textBounds"); }

  CImagePtr getImage() const { logNL("getImage"); return CImagePtr(); }

  void setImage(CImagePtr) { logNL("setImage"); }


  template<typename T>
  void logT(const T &t) {
    std::stringstream ss;

    ss << t;

    logNL(ss.str());
  }

  void logNL(const std::string &str) const {
    os() << str << std::endl;
  }

  void log(const std::string &str) const {
    os() << str;
  }

 private:
  std::ostream *os_;

  int    width_  { 0 };
  int    height_ { 0 };
  double xmin_   { 0 };
  double ymin_   { 0 };
  double xmax_   { 0 };
  double ymax_   { 0 };
};

#endif
