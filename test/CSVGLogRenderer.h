#ifndef CSVGLogRenderer_H
#define CSVGLogRenderer_H

#include <CSVGRenderer.h>

class CSVGLogRenderer : public CSVGRenderer {
 public:
  CSVGLogRenderer() : os_(&std::cout) { }

  CSVGLogRenderer *dup() const override {
    return new CSVGLogRenderer();
  }

  std::ostream &os() const { return *os_; }

  void setSize(int width, int height) override {
    width_  = width;
    height_ = height;
  }

  void getSize(int *width, int *height) const override {
    *width  = width_;
    *height = height_;
  }

  void setDataRange(double xmin, double ymin, double xmax, double ymax) override {
    xmin_ = xmin; ymin_ = ymin;
    xmax_ = xmax; ymax_ = ymax;
  }

  void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const override {
    *xmin = xmin_; *ymin = ymin_;
    *xmax = xmax_; *ymax = ymax_;
  }

  void setBackground(const CRGBA &) override { logNL("setBackground"); }

  bool isAntiAlias() const override { logNL("isAntiAlias"); return false; }
  void setAntiAlias(bool) override { logNL("setAntiAlias"); }

  void setEqualScale(bool) override { logNL("setEqualScale"); }

  void setScaleMin(bool) override { logNL("setScaleMin"); }

  void beginDraw() override { logNL("beginDraw"); }
  void endDraw  () override { logNL("endDraw"); }

  void setViewMatrix(const CMatrix2D &) override { logNL("setViewMatrix"); }

  void clear(const CRGBA &) override { logNL("clear"); }

  void pathInit() override { logNL("pathInit"); }
  void pathMoveTo(const CPoint2D &p) override { log("pathMoveTo "); logT(p); }
  void pathRMoveTo(const CPoint2D &p) override { log("pathRMoveTo "); logT(p); }
  void pathLineTo(const CPoint2D &p) override { log("pathLineTo "); logT(p); }
  void pathRLineTo(const CPoint2D &p) override { log("pathRLineTo "); logT(p); }
  void pathCurveTo(const CPoint2D &, const CPoint2D &) override { logNL("pathCurveTo"); }
  void pathRCurveTo(const CPoint2D &, const CPoint2D &) override { logNL("pathRCurveTo"); }
  void pathCurveTo(const CPoint2D &, const CPoint2D &, const CPoint2D &) override {
    logNL("pathCurveTo"); }
  void pathRCurveTo(const CPoint2D &, const CPoint2D &, const CPoint2D &) override {
    logNL("pathRCurveTo"); }
  void pathArcTo(const CPoint2D &, double, double, double, double) override {
    logNL("pathArcTo"); }
  void pathText(const std::string &) override { logNL("pathText"); }
  void pathClose() override { logNL("pathClose"); }

  bool pathGetCurrentPoint(CPoint2D &) override {
    return false;
  }

  void pathStroke() override { logNL("pathStroke"); }
  void pathFill  () override { logNL("pathFill"); }

  void savePath(const CMatrix2D &) override { logNL("savePath"); }

  void pathClip  (CSVGRenderer *) override { logNL("pathClip"); }
  void pathEoclip(CSVGRenderer *) override { logNL("pathEoclip"); }

  void initClip() override { logNL("initClip"); }

  void addClipPath(CSVGRenderer *) override { logNL("addClipPath"); }

  void pathBBox(CBBox2D &) override { logNL("pathBBox"); }

  void drawImage(const CPoint2D &, CImagePtr) override { logNL("drawImage"); }

  void setFont(CFontPtr) override { logNL("setFont"); }

  void setStrokeColor(const CRGBA &) override { logNL("setStrokeColor"); }
  void setLineWidth(double) override { logNL("setLineWidth"); }
  void setLineDash(const CLineDash &) override { logNL("setLineDash"); }
  void setLineCap(const CLineCapType &) override { logNL("setLineCap"); }
  void setLineJoin(const CLineJoinType &) override { logNL("setLineJoin"); }
  void setMitreLimit(double) override { logNL("setMitreLimit"); }

  void setFillType(CFillType) override { logNL("setFillType"); }

  void setFillColor(const CRGBA &) override { logNL("setFillColor"); }
  void setFillGradient(CGenGradient *) override { logNL("setFillGradient"); }
  void setFillImage(CImagePtr) override { logNL("setFillImage"); }

  void setAlign(CHAlignType, CVAlignType) override { logNL("setAlign"); }

  void windowToPixel(const CPoint2D &, CPoint2D &) override { logNL("windowToPixel"); }
  void pixelToWindow(const CPoint2D &, CPoint2D &) override { logNL("pixelToWindow"); }

  void textBounds(const std::string &, CBBox2D &) override { logNL("textBounds"); }

  CISize2D getImageSize() const override { logNL("getImageSize"); return CISize2D(); }

  CImagePtr getImage() const override { logNL("getImage"); return CImagePtr(); }
  void setImage(CImagePtr) override { logNL("setImage"); }

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
