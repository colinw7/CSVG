#ifndef CQSVG_RENDERER_H
#define CQSVG_RENDERER_H

#include <CSVGRenderer.h>
#include <CDisplayTransform2D.h>

#include <QImage>
#include <QPainter>
#include <QBrush>
#include <QPen>

class CQSVGImageData;

class CQSVGRenderer : public CSVGRenderer {
 public:
  CQSVGRenderer();

 ~CQSVGRenderer();

  CQSVGRenderer *dup() const override;

  QPainter *painter() const { return painter_; }

  const CRGBA &background() const { return background_; }
  void setBackground(const CRGBA &v) { background_ = v; }

  bool isDrawing() const { return drawing_; }
  void setDrawing(bool b) { drawing_ = b; }

  void setSize(int width, int height) override;
  void getSize(int *width, int *height) const override;

  void setPixelRange(int width, int height);

  void setDataRange(double xmin, double ymin, double xmax, double ymax) override;
  void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const override;

  bool isAntiAlias() const override { return antiAlias_; }
  void setAntiAlias(bool aa) override;

  void setEqualScale(bool eq) override;
  void setScaleMin  (bool sm) override;

  void beginDraw() override;
  void endDraw  () override;

  void startDraw() override;
  void stopDraw () override;

  void setViewMatrix(const CMatrix2D &m) override;

  void clear(const CRGBA &bg) override;

  void pathInit() override;
  void pathMoveTo  (const CPoint2D &p) override;
  void pathRMoveTo (const CPoint2D &p) override;
  void pathLineTo  (const CPoint2D &p) override;
  void pathRLineTo (const CPoint2D &p) override;
  void pathCurveTo (const CPoint2D &p1, const CPoint2D &p2) override;
  void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2) override;
  void pathCurveTo (const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) override;
  void pathRCurveTo(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) override;
  void pathArcTo   (const CPoint2D &c, double xr, double yr, double a1, double a2) override;
  void pathText    (const std::string &str) override;
  void pathClose() override;

  bool pathGetCurrentPoint(CPoint2D &p) override;

  void pathStroke() override;
  void pathFill  () override;

  void savePath(const CMatrix2D &m) override;

  void pathClip  (CSVGRenderer *renderer) override;
  void pathEoclip(CSVGRenderer *renderer) override;

  void initClip() override;

  void addClipPath(CSVGRenderer *renderer) override;

  void pathBBox(CBBox2D &bbox) override;

  void drawImage(const CPoint2D &p, CSVGImageData *image) override;

  void setFont(const CSVGFontDef &fontDef) override;

  void setStrokeColor(const CRGBA &fg) override;

  void setLineWidth (double w) override;
  void setLineDash  (const CLineDash &dash) override;
  void setLineCap   (const CLineCapType &cap) override;
  void setLineJoin  (const CLineJoinType &join) override;
  void setMitreLimit(double limit) override;

  void resetFill      () override;
  void setFillType    (CFillType fillType) override;
  void setFillColor   (const CRGBA &bg) override;
  void setFillGradient(CGenGradient *g) override;
  void setFillImage   (double xo, double yo, CSVGImageData *image) override;
  void setFillMatrix  (const CMatrix2D &m) override;

  void setStrokeFilled      (bool b) override;
  void setStrokeFillType    (CFillType fillType) override;
  void setStrokeFillGradient(CGenGradient *g) override;
  void setStrokeFillImage   (CSVGImageData *image) override;

  void setAlign(CHAlignType halign, CVAlignType valign) override;

  void windowToPixel(const CPoint2D &w, CPoint2D &p) override;
  void pixelToWindow(const CPoint2D &p, CPoint2D &w) override;

  void textBounds(const std::string &str, CBBox2D &bbox) override;

  CISize2D getImageSize() const override;

  CSVGImageData *getImage() const override;

  void setImage(CSVGRenderer *renderer) override;
  void setImage(CSVGImageData *image) override;

  void addImage(int x, int y, CSVGImageData *image) override;

  void combine(int x, int y, CSVGRenderer *r) override;

  void addResizedImage(CSVGRenderer *src, double x, double y, double w, double h) override;

  void addClippedImage(CSVGRenderer *src, int x, int y,
                       int px1, int py1, int px2, int py2) override;

  void setClippedImage(CSVGRenderer *src, int px1, int py1, int px2, int py2) override;

  void setOffsetImage(CSVGRenderer *src, int dx, int dy) override;

  //---

  const QImage &qimage() const;
  void setQImage(const QImage &image);

  CDisplayRange2D &getRange() { return range_; }

  const QTransform &getTransform () const { return transform_ ; }
  const QTransform &getITransform() const { return itransform_; }

  void paint(QPainter *painter);

 private:
  static void combineImage(QImage &image1, int x, int y, const QImage &image2);

 private:
  CQSVGImageData* imageData_;
  QPainter*       painter_ { 0 };
  QPainterPath*   path_ { 0 };
  bool            pathEmpty_ { true };
  QPainterPath*   savePath_ { 0 };
  QPen            pen_;
  QFont           qfont_;
  CMatrix2D       viewMatrix_;
  bool            antiAlias_ { true };
  CDisplayRange2D range_;
  QTransform      transform_;
  QTransform      itransform_;
  CRGBA           background_ { 1, 1, 1 };
  bool            drawing_ { false };

  // fill
  QBrush          fillBrush_;
  CFillType       fillType_ { FILL_TYPE_EVEN_ODD };

  // stroke
  QBrush          strokeBrush_;
  bool            strokeFilled_ { false };
  CFillType       strokeFillType_ { FILL_TYPE_EVEN_ODD };
};

#endif
