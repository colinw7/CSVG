#ifndef CPIXEL_RENDERER_H
#define CPIXEL_RENDERER_H

#include <CRGBA.h>
#include <CDirectionType.h>
#include <CThrow.h>
#include <CPen.h>
#include <CBrush.h>
#include <CFontMgr.h>
#include <CPoint2D.h>
#include <CIPoint2D.h>
#include <CISize2D.h>
#include <CIBBox2D.h>
#include <CSymbol2D.h>
#include <CBresenham.h>
#include <CSingleton.h>
#include <CPixelClip.h>
#include <CImagePtr.h>

//-------------

class CPixelRendererFiller {
 public:
  virtual ~CPixelRendererFiller() { }

  virtual void getColor(const CIPoint2D &point, CRGBA &rgba) const = 0;
};

class CPixelRendererDefFiller : public CPixelRendererFiller {
 private:
  CRGBA rgba_;

 public:
  CPixelRendererDefFiller(CRGBA &rgba) : rgba_(rgba) { }

  void getColor(const CIPoint2D &, CRGBA &rgba) const {
    rgba = rgba_;
  }
};

//-------------

class CPixelRendererPath {
 public:
  typedef std::vector<CIPoint2D>    IPointList;
  typedef std::vector<CPoint2D>     RPointList;
  typedef std::vector<IPointList *> IPointListList;
  typedef std::vector<RPointList *> RPointListList;

 private:
  IPointListList ipoly_points_list_;
  RPointListList rpoly_points_list_;

 public:
  CPixelRendererPath() : ipoly_points_list_(), rpoly_points_list_() { }
 ~CPixelRendererPath() { }

  //-----

 public:
  void initPolygons();

  void addPathPolygon(const IPointList &points);
  void addPathPolygon(const RPointList &points);

  IPointList *nextIPolygon();
  RPointList *nextRPolygon();

  IPointListList &getIPolygons() { return ipoly_points_list_; }
  RPointListList &getRPolygons() { return rpoly_points_list_; }
};

//-------------

class CPixelRenderer {
 public:
  typedef std::vector<CIPoint2D>    IPointList;
  typedef std::vector<CPoint2D>     RPointList;
  typedef std::vector<IPointList *> IPointListList;
  typedef std::vector<RPointList *> RPointListList;

 protected:
  CPixelClip clip_;
  CIPoint2D  offset_;
  CISize2D   vsize_;
  CRGBA      bg_;
  CImagePtr  bg_image_;
  CFontPtr   font_;
  bool       contentsChanged_;
  CIPen      pen_;
  CBrush     brush_;

 protected:
  CPixelRenderer();
  CPixelRenderer(const CPixelRenderer &renderer);

 public:
  virtual ~CPixelRenderer();

  virtual CPixelRenderer *dup() const = 0;

  const CPixelClip &getClip() const { return clip_; }

  virtual uint getWidth () const = 0;
  virtual uint getHeight() const = 0;

  const CIPoint2D &getOffset() const { return offset_; }
  virtual void setOffset(const CIPoint2D &offset);

  const CISize2D &getVSize() const { return vsize_; }
  virtual void setVSize(const CISize2D &vsize);

  bool getContentsChanged() { return contentsChanged_; }
  virtual void setContentsChanged(bool flag=true);

  virtual void applyOffset(CIPoint2D &point) const;

  virtual void unapplyOffset(CIPoint2D &point) const;

  virtual void updateSize(int, int) { }

  virtual void beginDraw() { }
  virtual void endDraw  () { }

  virtual void startDoubleBuffer(bool) { }
  virtual void endDoubleBuffer  (bool) { }
  virtual void copyDoubleBuffer () { }

  void getForeground(CRGBA &fg) const { fg = pen_.color_; }
  virtual void setForeground(const CRGBA &fg);

  void getBackground(CRGBA &bg) const { bg = bg_; }
  virtual void setBackground(const CRGBA &bg);

  void getBackgroundImage(CImagePtr &image) const { image = bg_image_; }
  virtual void setBackgroundImage(CImagePtr image);

  void getFont(CFontPtr &font) const { font = font_; }
  virtual void setFont(CFontPtr font);

  uint getLineWidth() const { return pen_.width_; }
  virtual void setLineWidth(uint width);

  const CILineDash &getLineDash() const { return pen_.dash_; }
  virtual void setLineDash(const CILineDash &dash);

  CLineCapType getLineCap() const { return pen_.cap_; }
  virtual void setLineCap(CLineCapType cap);

  CLineJoinType getLineJoin() const { return pen_.join_; }
  virtual void setLineJoin(CLineJoinType join);

  virtual void clear();

  virtual void fill();

  virtual void drawPoint(const CIPoint2D &point);

  virtual void drawClippedPoint(const CIPoint2D &) = 0;

  virtual void drawSymbol(const CIPoint2D &point, CSymbolType symbol);

  virtual uint getSymbolSize() const;

  virtual void drawChar(const CIPoint2D &point, char c);

  virtual void drawClippedChar(const CIPoint2D &point, char c);

  virtual void drawString(const CIPoint2D &point, const std::string &str);

  virtual void drawClippedString(const CIPoint2D &point, const std::string &str);

  virtual void drawStringInRect(const CIBBox2D &bbox, const std::string &str);

  virtual void drawLine(const CIPoint2D &point1, const CIPoint2D &point2);

  virtual void drawFilledLine(const CIPoint2D &point1, const CIPoint2D &point2,
                              const CPixelRendererFiller &filler);

  virtual void drawAALine(const CIPoint2D &point1, const CIPoint2D &point2);

 private:
  void drawLine1(const CIPoint2D &point1, const CIPoint2D &point2);

  void drawFilledLine1(const CIPoint2D &point1, const CIPoint2D &point2,
                       const CPixelRendererFiller &filler);

 public:
  virtual void drawClippedLine(const CIPoint2D &point1, const CIPoint2D &point2);

  virtual void drawFilledClippedLine(const CIPoint2D &point1,
                                     const CIPoint2D &point2,
                                     const CPixelRendererFiller &filler);

 private:
  void lineToPolygon(const CIPoint2D &point1, const CIPoint2D &point2,
                     double width, double *xp, double *yp);

 public:
  virtual void drawRectangle(const CIBBox2D &bbox);

  virtual void drawClippedRectangle(const CIBBox2D &bbox);

  virtual void fillRectangle(const CIBBox2D &bbox);

  virtual void fillClippedRectangle(const CIBBox2D &bbox);

  virtual void fillTriangle(const CIPoint2D &point1, const CIPoint2D &point2,
                            const CIPoint2D &point3);

  virtual void fillClippedTriangle(const CIPoint2D &point1,
                                   const CIPoint2D &point2,
                                   const CIPoint2D &point3);

  virtual void drawPolygon(const IPointList &points);

  virtual void drawClippedPolygon(const IPointList &points);

  virtual void fillPolygon(const IPointList &points);

  virtual void fillClippedPolygon(const IPointList &points);

  virtual void fillFilledPolygon(const IPointList &points,
                                 const CPixelRendererFiller &filler);

  virtual void fillFilledClippedPolygon(const IPointList &points,
                                        const CPixelRendererFiller &filler);

  virtual void fillImageRectangle(const CIBBox2D &bbox, CImagePtr image);

  virtual void fillImagePolygon(const IPointList &points, CImagePtr image);

  virtual void fillImageClippedRectangle(const CIBBox2D &bbox, CImagePtr image);
  virtual void fillImageClippedPolygon(const IPointList &points, CImagePtr image);

  virtual void fillPatternRectangle(const CIBBox2D &bbox,
                                    CBrushPattern pattern);
  virtual void fillPatternPolygon(const IPointList &ipoints,
                                  CBrushPattern pattern);
  virtual void fillPatternClippedRectangle(const CIBBox2D &bbox,
                                           CBrushPattern pattern);
  virtual void fillPatternClippedPolygon(const IPointList &ipoints,
                                         CBrushPattern pattern);

  virtual void fillGradientPolygon(const IPointList &ipoints,
                                   CRefPtr<CGenGradient> gradient);
  virtual void fillGradientClippedPolygon(const IPointList &ipoints,
                                          CRefPtr<CGenGradient> gradient);

  static CImagePtr getPatternImage(CBrushPattern pattern);

 private:
  static CImagePtr getHorizontalImage();
  static CImagePtr getVerticalImage();
  static CImagePtr getCrossImage();
  static CImagePtr getUpImage();
  static CImagePtr getDownImage();
  static CImagePtr getDiagonalCrossImage();
  static CImagePtr getDotted1Image();
  static CImagePtr getDotted2Image();
  static CImagePtr getDotted3Image();
  static CImagePtr getDotted4Image();
  static CImagePtr getDotted5Image();
  static CImagePtr getDotted6Image();
  static CImagePtr getDotted7Image();

 public:
  virtual void fillAAPolygon(double *x, double *y, uint num_xy);
  virtual void fillFilledAAPolygon(double *x, double *y, uint num_xy,
                                   const CPixelRendererFiller &filler);
  virtual void fillAAPolygon(const CPoint2D *points, uint num_points);
  virtual void fillFilledAAPolygon(const CPoint2D *points, uint num_points,
                                   const CPixelRendererFiller &filler);
  virtual void fillAAPolygon(const std::vector<CPoint2D> &points);
  // all routines eventually call this one
  virtual void fillFilledAAPolygon(const std::vector<CPoint2D> &points,
                                   const CPixelRendererFiller &filler);

  virtual void drawHLine(int x1, int x2, int y);
  virtual void drawClippedHLine(int x1, int x2, int y);

  virtual void drawVLine(int x, int y1, int y2);
  virtual void drawClippedVLine(int x, int y1, int y2);

  virtual void drawCircle(const CIPoint2D &center, int r);
  virtual void fillCircle(const CIPoint2D &center, int r);
  virtual void fillImageCircle(const CIPoint2D &center, int r,
                               CImagePtr image);

  virtual void drawEllipse(const CIPoint2D &center, int xr, int yr);
  virtual void fillEllipse(const CIPoint2D &center, int xr, int yr);
  virtual void fillImageEllipse(const CIPoint2D &center, int xr, int yr,
                                CImagePtr image);

  virtual void drawArc(const CIPoint2D &center, int xr, int yr,
                       double a1, double a2);

  virtual void drawClippedArc(const CIPoint2D &center, int xr, int yr,
                              double a1, double a2);

 private:
  void drawClippedArc1(const CIPoint2D &center, int xr, int yr,
                       double a1, double a2);
  void drawClippedArc2(const CIPoint2D &center, int xr, int yr,
                       int x1, int y1, int x2, int y2, double a1, double a2);

 public:
  virtual void fillArc(const CIPoint2D &center, int xr, int yr,
                       double a1, double a2);
  virtual void fillClippedArc(const CIPoint2D &center, int xr, int yr,
                              double a1, double a2);

  virtual void fillImageArc(const CIPoint2D &center, int xr, int yr,
                            double a1, double a2, CImagePtr image);
  virtual void fillImageClippedArc(const CIPoint2D &center, int xr, int yr,
                                   double a1, double a2, CImagePtr image);

 private:
  void fillClippedArc1(const CIPoint2D &center, int xr, int yr,
                       double a1, double a2);
  void fillClippedArc2(const CIPoint2D &center, int xr, int yr,
                       int x1, int y1, int x2, int y2, double a1, double a2);

  void fillImageClippedArc1(const CIPoint2D &center, int xr, int yr,
                            double a1, double a2, CImagePtr image);
  void fillImageClippedArc2(const CIPoint2D &center, int xr, int yr,
                            int x1, int y1, int x2, int y2,
                            double a1, double a2, CImagePtr image);

 public:
  virtual void drawImage(const CIPoint2D &point, CImagePtr image);

  virtual void drawClippedImage(const CIPoint2D &point, CImagePtr image);

  virtual void drawAlphaImage(const CIPoint2D &point, CImagePtr image);

  virtual void drawClippedAlphaImage(const CIPoint2D &point, CImagePtr image);

  virtual void drawImageInBox(const CIBBox2D &bbox, CImagePtr image);

  virtual void drawAlphaImageInBox(const CIBBox2D &bbox, CImagePtr image);

  virtual void drawScaledImage(const CIPoint2D &point, CImagePtr image,
                               double sx, double sy);
  virtual void drawClippedScaledImage(const CIPoint2D &point, CImagePtr image,
                                      double sx, double sy);

  virtual int getCharWidth  ();
  virtual int getCharAscent ();
  virtual int getCharDescent();

  virtual int getCharHeight();

  virtual int getCharWidth(char c);

  virtual int getStringWidth(const std::string &str);

  virtual void flush();

 public:
  virtual CImagePtr getImage();
  virtual void setImage(CImagePtr image);

  virtual CImagePtr getAlphaImage();
  virtual void setAlphaImage(CImagePtr image);

 public:
  void setClip(int *x, int *y, uint num_xy);

  void resetClip();

  void clipPolygons(IPointListList &point_list_list);

  void eoclipPolygons(IPointListList &point_list_list);

  const IPointListList &getClipPolygons(uint i) const;

  uint getNumClipPolygons() const;

#if 0
  void fillPolygons(const CPixelRendererPath &path);
  void fillPolygons(const CPixelRendererPath &path, CImagePtr image);
  void eofillPolygons(const CPixelRendererPath &path);
  void eofillPolygons(const CPixelRendererPath &path, CImagePtr image);
#endif

  void fillPathPolygons(IPointListList &poly_points_list, CImagePtr image,
                        CFillType type);
  void fillPathPolygons(RPointListList &poly_points_list, CImagePtr image,
                        CFillType type);

 public:
  virtual bool mmToPixel(double mm, double *pixel) {
    *pixel = mm; return false;
  }

 public:
  void fillFlatTriangle(const CIPoint2D &p1, const CIPoint2D &p2,
                        const CIPoint2D &p3);

  void fillGouraudTriangle(const CPoint2D &p1, CRGBA rgb1,
                           const CPoint2D &p2, CRGBA rgb2,
                           const CPoint2D &p3, CRGBA rgb3);

  void textureTriangle(CImagePtr texture,
                       const CPoint2D &p1, double tx1, double ty1, CRGBA rgb1,
                       const CPoint2D &p2, double tx2, double ty2, CRGBA rgb2,
                       const CPoint2D &p3, double tx3, double ty3, CRGBA rgb3);

  void getPixelClip(int *clip_x1, int *clip_y1,
                    int *clip_x2, int *clip_y2);

 private:
  void fillTriangleTop(const CIPoint2D &p1, const CIPoint2D &p2,
                       const CIPoint2D &p3);
  void fillTriangleBot(const CIPoint2D &p1, const CIPoint2D &p2,
                       const CIPoint2D &p3);

  void fillGouraudTriangleTop(const CPoint2D &p1, CRGBA rgb1,
                              const CPoint2D &p2, CRGBA rgb2,
                              const CPoint2D &p3, CRGBA rgb3);
  void fillGouraudTriangleBot(const CPoint2D &p1, CRGBA rgb1,
                              const CPoint2D &p2, CRGBA rgb2,
                              const CPoint2D &p3, CRGBA rgb3);

  void textureTriangleTop(CImagePtr texture,
                          const CPoint2D &p1, double tx1, double ty1,
                          CRGBA rgb1,
                          const CPoint2D &p2, double tx2, double ty2,
                          CRGBA rgb2,
                          const CPoint2D &p3, double tx3, double ty3,
                          CRGBA rgb3);
  void textureTriangleBot(CImagePtr texture,
                          const CPoint2D &p1, double tx1, double ty1,
                          CRGBA rgb1,
                          const CPoint2D &p2, double tx2, double ty2,
                          CRGBA rgb2,
                          const CPoint2D &p3, double tx3, double ty3,
                          CRGBA rgb3);

 public:
  virtual CPixelRenderer *createImageRenderer() const;

  // notifications
 public:
  virtual void updatePen() { }

  // not to be implemented
 private:
  CPixelRenderer &operator=(const CPixelRenderer &renderer);
};

//-------------

class CPixelRendererBresenham : public CBresenham {
 private:
  CPixelRenderer *renderer_;
  CRGBA           rgba_;

 public:
  CPixelRendererBresenham(CPixelRenderer *renderer) :
   renderer_(renderer), rgba_(0,0,0) {
  }

  void drawPoint(int x, int y) {
    renderer_->drawClippedPoint(CIPoint2D(x, y));
  }

  const CILineDash &getLineDash() const {
    return renderer_->getLineDash();
  }

 private:
  CPixelRendererBresenham(const CPixelRendererBresenham &renderer);
  CPixelRendererBresenham &operator=(const CPixelRendererBresenham &renderer);
};

#endif
