#ifndef CRENDERER_2D_H
#define CRENDERER_2D_H

#include <CDisplayRange2D.h>
#include <CPixelRenderer.h>
#include <C2Bezier2D.h>
#include <C3Bezier2D.h>
#include <CIPoint2D.h>
#include <CPoint2D.h>
#include <CMatrix2D.h>
#include <CPath2DRenderer.h>
#include <CPath2DFlattener.h>
#include <CPen.h>
#include <CBrush.h>
#include <COrientation.h>
#include <CFillType.h>
#include <CTransform2D.h>
#include <CPath2D.h>
#include <CAutoPtr.h>
#include <CRenderer2DFunc.h>

class CGenGradient;

class CRenderer2DFiller {
 public:
  virtual ~CRenderer2DFiller() { }

  virtual void getColor(const CPoint2D &point, CRGBA &rgba) const = 0;
};

class CRendererRegion2D;

class CRenderer2D : public CPath2DRenderer, public CPath2DFlattener {
 public:
  typedef std::vector<CPoint2D>   PointList;
  typedef std::vector<double>     RealList;
  typedef std::vector<CIPoint2D>  IPointList;
  typedef std::vector<CPoint2D>   RPointList;
  typedef std::vector<PointList>  PointListList;
  typedef std::vector<CPath2D *>  PathStack;
  typedef std::vector<C3Bezier2D> BezierList;

 protected:
  CRenderer2D();

 public:
  virtual ~CRenderer2D();

  CRenderer2D(CPixelRenderer *renderer);

  CRenderer2D(const CRenderer2D &renderer);

  const CRenderer2D &operator=(const CRenderer2D &renderer);

  virtual CRenderer2D *dup() const = 0;

 public:
  virtual CPixelRenderer *getPixelRenderer() const = 0;

  void setPixelRenderer(CPixelRenderer *renderer);

  bool getContentsChanged();
  void setContentsChanged(bool flag = true);

  uint getPixelWidth () const;
  uint getPixelHeight() const;

  CSymbolType getSymbol() const { return symbol_; }

  void setSymbol(CSymbolType symbol) { symbol_ = symbol; }

  virtual void setPixelOffset(const CIPoint2D &offset);

  const CIPoint2D &getPixelOffset() const;

  virtual void applyPixelOffset(CIPoint2D &point) const;

  virtual void unapplyPixelOffset(CIPoint2D &point) const;

  const CMatrix2D *getViewMatrix () const { return &view_matrix_ ; }
  const CMatrix2D *getIViewMatrix() const { return &view_imatrix_; }

  virtual void setViewMatrix(const CMatrix2D &view_matrix);
  virtual void unsetViewMatrix();

  const CPixelClip &getClip() const;

  virtual void setPixelRange(int xmin, int ymin, int xmax, int ymax);

  void getPixelRange(int *xmin, int *ymin, int *xmax, int *ymax);

  virtual void setDataRange(double xmin, double ymin, double xmax, double ymax);

  virtual void resetDataRange();

  void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax);

  virtual void setEqualScale(bool flag);

  bool getEqualScale() const;

  virtual void setScaleMin(bool flag);

  bool getScaleMin() const;

  virtual void setAlign(CHAlignType halign, CVAlignType valign);

  CHAlignType getHAlign() const;
  CVAlignType getVAlign() const;

  //------

  virtual void disableCanvas();
  virtual void enableCanvas();

  //------

  virtual void updateSize(int width, int heigh);

  virtual void beginDraw();
  virtual void endDraw  ();

  virtual void startDoubleBuffer(bool clear=true);
  virtual void endDoubleBuffer  (bool copy=true);
  virtual void copyDoubleBuffer ();

  //------

  // Pen

  virtual void setLineColor(const CRGBA &rgba);

  const CRGBA &getLineColor() const;

  virtual void setLineWidth(double width);

  double getLineWidth() const;

  virtual void setLineDash(const CLineDash &dash=CLineDash());

  const CLineDash &getLineDash() const;

  virtual void setLineCap(CLineCapType cap);

  CLineCapType getLineCap() const;

  virtual void setLineJoin(CLineJoinType join);

  CLineJoinType getLineJoin() const;

  virtual void setMitreLimit(double limit);

  double getMitreLimit() const;

  virtual void setFlatness(double flatness);

  double getFlatness() const;

  //------

  virtual void setFillType(CFillType fill_type);

  CFillType getFillType() const;

  CPath2D *getPath() const { return path_; }

  //------

  // Brush

  virtual void setBrushColor(const CRGBA &rgba);

  const CRGBA &getBrushColor() const;

  virtual void setBrushStyle(CBrushStyle style);

  CBrushStyle getBrushStyle() const;

  void setBrushTexture(CImagePtr texture);

  CImagePtr getBrushTexture() const;

  void setBrushGradient(CRefPtr<CGenGradient> gradient);

  CRefPtr<CGenGradient> getBrushGradient() const;

  //------

  virtual void setBackground(const CRGBA &rgba);
  virtual void setForeground(const CRGBA &rgba);

  //------

  virtual CImagePtr getImage();
  virtual CImagePtr getAlphaImage();

  virtual void setImage(CImagePtr image);
  virtual void setAlphaImage(CImagePtr image);

  virtual void setImage(CRenderer2D *renderer);

  //------

  void clipPolygons();
  void eoclipPolygons();

  uint getNumClipPolygons();

  const PointListList &getClipPolygons(uint i);

  void initPolygons();

  void addPolygon(const RPointList &points);

  void fillPolygons();
  void fillPolygons(CImagePtr image);

  void eofillPolygons();
  void eofillPolygons(CImagePtr image);

  //------

 protected:
  void initPath() const;

 public:
  virtual void pathInit();
  virtual void pathTerm();

  virtual void pathRoundedRectangle(const CPoint2D &p1, const CPoint2D &p2, double rx, double ry);

  virtual void pathMoveTo (const CPoint2D &p);
  virtual bool pathRMoveTo(const CPoint2D &p);

  virtual bool pathLineTo (const CPoint2D &p);
  virtual bool pathRLineTo(const CPoint2D &p);

  virtual bool pathArcTo(const CPoint2D &p1, const CPoint2D &p2, double xr, double yr);

  virtual bool pathBezier2To (const CPoint2D &p1, const CPoint2D &p2);
  virtual bool pathRBezier2To(const CPoint2D &p1, const CPoint2D &p2);

  virtual bool pathBezier3To (const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  virtual bool pathRBezier3To(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);

  virtual void pathClose();

  virtual bool pathGetCurrentPoint(CPoint2D &p);

  virtual void pathStroke();
  virtual void pathStrokePath();

  virtual void pathFill();
  virtual void pathEofill();

  virtual void pathPatternFill(CBrushPattern pattern);
  virtual void pathImageFill(CImagePtr image);
  virtual void pathGradientFill(CRefPtr<CGenGradient> gradient);

  virtual void pathClip();
  virtual void pathEoclip();

  virtual void pathClipPath();
  virtual void pathFlatten();
  virtual void pathReverse();
  virtual void pathProcess(CPath2DVisitor &visitor);

  virtual void pathShowChar(int c, CMatrix2D *matrix, double *x, double *y);

  virtual void pathGetBounds(double *xmin, double *ymin, double *xmax, double *ymax);

  virtual void pathSetStrokeAdjust(bool stroke_adjust);

  virtual void pathBBox(CBBox2D &bbox);

  virtual void pathPrint();

  //------

  //void drawLine(const CLine2D &line);

  virtual void drawLine(const CPoint2D &point1, const CPoint2D &point2);

  void drawLineG2(const CPoint2D &point1, const CPoint2D &point2, double g1, double g2);

 protected:
  void drawDashLine(const CPoint2D &point1, const CPoint2D &point2);

  void drawDashLineG2(const CPoint2D &point1, const CPoint2D &point2, double g1, double g2);

  void drawSolidLine(const CPoint2D &point1, const CPoint2D &point2);

  void drawSolidLineG2(const CPoint2D &point1, const CPoint2D &point2, double g1, double g2);

  void drawWideSolidLine(const CPoint2D &point1, const CPoint2D &point2);

  void drawWideSolidLineG2(const CPoint2D &point1, const CPoint2D &point2, double g1, double g2);

  void drawThinSolidLine(const CPoint2D &point1, const CPoint2D &point2);

 public:
  virtual void fillPolygon(const PointList &points);

  virtual void fillFilledPolygon(const PointList &points, const CRenderer2DFiller &filler);

 protected:
  void fillBrushPolygon(const IPointList &ipoints, const CBrush &brush);

 public:
  virtual void fillPatternPolygon(const PointList &ipoints, CBrushPattern pattern);
  virtual void fillPatternPolygon(const IPointList &ipoints, CBrushPattern pattern);

  virtual void fillGradientPolygon(const PointList &ipoints, CRefPtr<CGenGradient> gradient);
  virtual void fillGradientPolygon(const IPointList &ipoints, CRefPtr<CGenGradient> gradient);

  virtual void fillImagePolygon(const PointList &ipoints, CImagePtr image);
  virtual void fillImagePolygon(const IPointList &ipoints, CImagePtr image);

 public:
  virtual void drawPolygon(const PointList &points);

  void drawCircle (const CPoint2D &center, double r);
  void drawEllipse(const CPoint2D &center, double xs, double ys);

  virtual void drawArc(const CPoint2D &center, double rx, double ry, double angle1, double angle2);
  virtual void drawArcN(const CPoint2D &center, double rx, double ry, double angle1, double angle2);

  void fillCircle(const CPoint2D &center, double r);
  void fillEllipse(const CPoint2D &center, double xs, double ys);

  void fillImageCircle(const CPoint2D &center, double r, CImagePtr image);
  void fillImageEllipse(const CPoint2D &center, double xs, double ys, CImagePtr image);

  void fillPatternCircle(const CPoint2D &center, double r, CBrushPattern pattern);
  void fillPatternEllipse(const CPoint2D &center, double xs, double ys, CBrushPattern pattern);

  void fillGradientCircle(const CPoint2D &center, double r, CRefPtr<CGenGradient> gradient);
  void fillGradientEllipse(const CPoint2D &center, double xs, double ys,
                           CRefPtr<CGenGradient> gradient);

  virtual void fillArc(const CPoint2D &center, double rx, double ry,
                       double angle1, double angle2);
  virtual void fillArcN(const CPoint2D &center, double rx, double ry,
                        double angle1, double angle2);

  virtual void fillImageArc(const CPoint2D &center, double rx, double ry,
                            double angle1, double angle2, CImagePtr image);

  virtual void fillPatternArc(const CPoint2D &center, double rx, double ry,
                              double angle1, double angle2, CBrushPattern pattern);

  virtual void fillGradientArc(const CPoint2D &center, double rx, double ry, double angle1,
                               double angle2, CRefPtr<CGenGradient> gradient);

 protected:
  void arcToPoints(const CPoint2D &center, double xr, double yr,
                   double angle1, double angle2, PointList &fill_points);

 public:
  virtual void drawBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  virtual void drawBezier3(const CPoint2D &p1, const CPoint2D &p2,
                           const CPoint2D &p3, const CPoint2D &p4);

  virtual void drawBezier(C3Bezier2D &bezier);
  virtual void drawBezier(C2Bezier2D &bezier);

 public:
  virtual bool image(char *image_data, int width, int height, int bits_per_sample,
                     CMatrix2D *matrix, CMatrix2D *ctm_matrix, double *decode_array,
                     int decode_size);

  virtual bool imageMask(char *image_data, int width, int height, int polarity,
                         CMatrix2D *matrix, CMatrix2D *ctm_matrix);

  virtual void debugPrintImage(char *image_data, int width, int height, int bits_per_sample,
                               CMatrix2D *matrix, CMatrix2D *ctm_matrix, double *decode_array,
                               int decode_size);
  virtual void debugPrintImageMask(char *image_data, int width, int height, int polarity,
                                   CMatrix2D *matrix, CMatrix2D *ctm_matrix);

 public:
  virtual void arcToBeziers(const CPoint2D &center, double rx, double ry, double angle1,
                            double angle2, BezierList &arc_beziers);
  virtual void arcNToBeziers(const CPoint2D &center, double rx, double ry, double angle1,
                             double angle2, BezierList &arc_beziers);

 protected:
  uint calcNumArcBeziers(const CPoint2D &center, double rx, double ry,
                         double angle1, double angle2);

 public:
  virtual void bezierToLines(const C3Bezier2D &bezier, PointList &points, RealList &gradients);
  virtual void bezierToLines(const C2Bezier2D &bezier, PointList &points, RealList &gradients);

 protected:
  void bezierToLines1(const C3Bezier2D &bezier, uint depth, PointList &points, RealList &gradients);
  void bezierToLines1(const C2Bezier2D &bezier, uint depth, PointList &points, RealList &gradients);

 public:
  virtual void joinLines(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);

 protected:
  void mitreJoinLines(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  void roundJoinLines(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  void bevelJoinLines(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);

 public:
  virtual void capLine(const CPoint2D &p1, const CPoint2D &p2);

 protected:
  void buttCapLine  (const CPoint2D &p1, const CPoint2D &p2);
  void roundCapLine (const CPoint2D &p1, const CPoint2D &p2);
  void squareCapLine(const CPoint2D &p1, const CPoint2D &p2);

 public:
  virtual void lineToPolygon(const CPoint2D &p1, const CPoint2D &p2, PointList &points);

  virtual void lineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2,
                                 const CPoint2D &p3, PointList &points);

 protected:
  void mitreLineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2,
                              const CPoint2D &p3, PointList &points);
  void roundLineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2,
                              const CPoint2D &p3, PointList &points);
  void bevelLineJoinToPolygon(const CPoint2D &p1, const CPoint2D &p2,
                              const CPoint2D &p3, PointList &points);

 protected:
  void addWidthToPointG1(const CPoint2D &point, double g, CPoint2D &point1, CPoint2D &point2);
  void addWidthToPointG2(const CPoint2D &point, double g1, double g2,
                         CPoint2D &point1, CPoint2D &point2);

 public:
  virtual bool isAntiAlias() const;
  virtual void setAntiAlias(bool flag);

  virtual void adjustPoint(CPoint2D &p);

  virtual void setTransform(bool flag);

  virtual void clear();
  virtual void fill();

  virtual void textBounds(const std::string &text, CBBox2D &box);

  virtual void drawText(const CPoint2D &point, const std::string &text);

  virtual void fillText(const CPoint2D &point, const std::string &text);

  virtual void drawTextInRect(const CBBox2D &rect, const std::string &text);

  virtual void setTextSize(double x_size, double y_size);

  virtual void getTextLimits(const std::string &str, double *text_x, double *text_y);

 protected:
  void getTransformedLineWidth(double *xw, double *yw) const;

  void transformPointToPixel(const CPoint2D &w, CPoint2D &p) const;
  void transformPointToPixel(const CPoint2D &w, CIPoint2D &p) const;

  void transformPixelToPoint(const CPoint2D &p, CPoint2D &w) const;
  void transformPixelToPoint(const CIPoint2D &p, CPoint2D &w) const;

 public:
  virtual void transformPoint  (const CPoint2D &p1, CPoint2D &p2) const;
  virtual void untransformPoint(const CPoint2D &p1, CPoint2D &p2) const;

 protected:
  void transformPoint1  (const CPoint2D &p1, CPoint2D &p2) const;
  void untransformPoint1(const CPoint2D &p1, CPoint2D &p2) const;

 public:
  virtual void pixelLengthToWindowLength(double pl, double *wl) const;
  virtual void pixelWidthToWindowWidth  (double pw, double *ww) const;
  virtual void pixelHeightToWindowHeight(double ph, double *wh) const;

  virtual void windowLengthToPixelLength(double wl, double *pl) const;
  virtual void windowLengthToPixelLength(double wl, int    *pl) const;

 public:
  virtual void windowToPixel(const CPoint2D &w, CPoint2D &p) const;
  virtual void windowToPixel(const CPoint2D &w, CIPoint2D &p,
                             CMathGen::Rounding rounding=CMathGen::ROUND_NEAREST) const;

  virtual void pixelToWindow(const CIPoint2D &pixel, CPoint2D &window) const;
  virtual void pixelToWindow(const CPoint2D &p, CPoint2D &w) const;

 public:
  virtual void initClip();

 public:
  virtual void drawImage(const CPoint2D &point, CImagePtr image);

  virtual void drawAlphaImage(const CPoint2D &point, CImagePtr image);

  virtual void drawImageInBox(const CBBox2D &bbox, CImagePtr image);

  virtual void drawAlphaImageInBox(const CBBox2D &bbox, CImagePtr image);

  //------

  virtual void drawPoint(const CPoint2D &point);

  //------

  virtual void drawSymbol(const CPoint2D &point, CSymbolType symbol);

  //------

  virtual void drawRoundedRectangle(const CPoint2D &p1, const CPoint2D &p2, double rx, double ry);
  virtual void fillRoundedRectangle(const CPoint2D &p1, const CPoint2D &p2, double rx, double ry);

  //------

  virtual void fillRectangle(const CBBox2D &bbox);

  virtual void gradientFillRectangle(const CBBox2D &bbox, CGenGradient *gradient);

  virtual void fillFilledRectangle(const CBBox2D &bbox, const CRenderer2DFiller &filler);

  //------

  virtual void drawRectangle(const CBBox2D &bbox);

  //------

 public:
  virtual void setFont(CFontPtr font);
  virtual void getFont(CFontPtr &font) const;

  virtual double getCharWidth  ();
  virtual double getCharAscent ();
  virtual double getCharDescent();
  virtual double getCharHeight ();

  virtual double getCharWidth(char c) {
    std::string str(&c, 1);

    return getStringWidth(str);
  }

  virtual double getStringWidth(const std::string &str);

 public:
  virtual void pathDrawPolygon(const PointList &points);

  virtual void pathFillPolygon(const PointList &points);

  virtual void pathPolygon(const PointList &points);

  virtual void pathDrawCircle(const CPoint2D &center, double r);
  virtual void pathFillCircle(const CPoint2D &center, double r);
  virtual void pathCircle(const CPoint2D &center, double r);

  virtual void pathDrawEllipse(const CPoint2D &center, double xr, double yr);
  virtual void pathFillEllipse(const CPoint2D &center, double xr, double yr);

  virtual void pathEllipse(const CPoint2D &center, double xr, double yr);

  virtual void pathDrawArc(const CPoint2D &center, double rx, double ry,
                           double angle1, double angle2);
  virtual void pathFillArc(const CPoint2D &center, double rx, double ry,
                           double angle1, double angle2);

  virtual void pathArc(const CPoint2D &center, double rx, double ry,
                       double angle1, double angle2);

  virtual void pathDrawArcN(const CPoint2D &center, double rx, double ry,
                            double angle1, double angle2);
  virtual void pathFillArcN(const CPoint2D &center, double rx, double ry,
                            double angle1, double angle2);

  virtual void pathArcN(const CPoint2D &center, double rx, double ry,
                        double angle1, double angle2);

  virtual void pathDrawBezier(C3Bezier2D &bezier);
  virtual void pathDrawBezier(C2Bezier2D &bezier);

  virtual void pathBezier(C3Bezier2D &bezier);
  virtual void pathBezier(C2Bezier2D &bezier);

  virtual void pathFillRectangle(const CBBox2D &bbox);
  virtual void pathDrawRectangle(const CBBox2D &bbox);

  virtual void pathRectangle(const CBBox2D &bbox);

  //------

 public:
  virtual void setRegion(CRendererRegion2D *region);

  virtual CRendererRegion2D *getRegion();

  //------

 public:
  virtual CPath2D *createPath() const;

  virtual CRenderer2D *createImageRenderer() const;

  virtual CPath2DRenderer *createPathRenderer() const;

 public:
  void drawFunction(const CRenderer2DFunc &func);

 protected:
  bool                         enabled_ { false };
  CPixelRenderer              *pixel_renderer_ { 0 };
  CDisplayRange2D              display_range_;
  CMatrix2D                    view_matrix_;
  CMatrix2D                    view_imatrix_;
  bool                         transform_flag_ { false };
  bool                         anti_alias_ { false };
  CPen                         pen_;
  CBrush                       brush_;
  CAutoPtr<CPath2D>            path_;
  PathStack                    path_stack_;
  CAutoPtr<CRendererRegion2D>  region_;
  CSymbolType                  symbol_ { CSYMBOL_NONE };
  CPixelRendererPath           ppath_;
};

#endif
