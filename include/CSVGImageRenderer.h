#ifndef CSVGImageRenderer_H
#define CSVGImageRenderer_H

#include <CSVGRenderer.h>

class CSVGImageData;
class CSVGFontDef;
class CImageRenderer2D;

class CSVGImageRenderer : public CSVGRenderer {
 public:
  CSVGImageRenderer(int w, int h);

  virtual ~CSVGImageRenderer();

  CSVGImageRenderer(const CSVGImageRenderer &) = delete;
  CSVGImageRenderer &operator=(const CSVGImageRenderer &) = delete;

  CSVGImageRenderer *dup() const override;

  void setSize(int width, int height) override;
  void getSize(int *width, int *height) const override;

  const CImagePtr &getImagePtr() const { return image_; }

  void setPixelRange(int w, int h) override;

  void setDataRange(double xmin, double ymin, double xmax, double ymax) override;
  void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const override;

  bool isAntiAlias() const override;
  void setAntiAlias(bool b) override;

  void setEqualScale(bool b) override;
  void setScaleMin(bool b) override;

  void beginDraw() override;
  void endDraw  () override;

  void setViewMatrix(const CMatrix2D &m) override;

  void setBackground(const CRGBA &) override;

  void clear(const CRGBA &c) override;

  void pathInit() override;
  void pathMoveTo(const CPoint2D &p) override;
  void pathRMoveTo(const CPoint2D &p) override;
  void pathLineTo(const CPoint2D &p) override;
  void pathRLineTo(const CPoint2D &p) override;
  void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2) override;
  void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2) override;
  void pathCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) override;
  void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) override;
  void pathArcTo(const CPoint2D &c, double rx, double ry, double a1, double a2) override;
  void pathText(const std::string &) override;
  void pathClose() override;

  bool pathGetCurrentPoint(CPoint2D &p) override;

  void pathStroke() override;
  void pathFill  () override;

  void savePath(const CMatrix2D &) override;

  void pathClip  (CSVGRenderer *) override;
  void pathEoclip(CSVGRenderer *) override;

  void initClip() override;

  void addClipPath(CSVGRenderer *) override;

  void pathBBox(CBBox2D &bbox) override;

  void drawImage(const CPoint2D &p, CSVGImageData *data) override;

  void setFont(const CSVGFontDef &fontDef) override;

  //---

  void resetStroke() override;

  void setStrokeColor(const CRGBA &c) override;

  void setLineWidth(double w) override;
  void setLineDash(const CLineDash &d) override;
  void setLineCap(const CLineCapType &c) override;
  void setLineJoin(const CLineJoinType &j) override;
  void setMitreLimit(double l) override;

  //---

  void resetFill() override;
  void setFillType(CFillType t) override;
  void setFillColor(const CRGBA &c) override;
  void setFillGradient(CGenGradient *) override;
  void setFillImage(double, double, CSVGImageData *) override;
  void setFillMatrix(const CMatrix2D &) override;

  //---

  void setStrokeFilled(bool) override;
  void setStrokeFillType(CFillType) override;
  void setStrokeFillGradient(CGenGradient *) override;
  void setStrokeFillImage(CSVGImageData *) override;

  //---

  void setAlign(CHAlignType halign, CVAlignType valign) override;

  void windowToPixel(const CPoint2D &w, CPoint2D &p) override;
  void pixelToWindow(const CPoint2D &p, CPoint2D &w) override;

  void textBounds(const std::string &text, CBBox2D &bbox) override;

  CISize2D getImageSize() const override;

  CSVGImageData *getImage() const override;

  void setImage(CSVGImageData *data) override;

  void setImage(CSVGRenderer *renderer) override;

 private:
  int               w_        { 100 };
  int               h_        { 100 };
  CImagePtr         image_;
  CImageRenderer2D *renderer_ { nullptr };
};

#endif
