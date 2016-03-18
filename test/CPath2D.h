#ifndef CPATH_2D_PATH_H
#define CPATH_2D_PATH_H

#include <CSingleton.h>
#include <CBuffer.h>
#include <CPoint2D.h>
#include <CMatrix2D.h>
#include <CBBox2D.h>
#include <C3Bezier2D.h>
#include <C2Bezier2D.h>
#include <CFillType.h>
#include <CImage.h>
#include <CFont.h>

#define CPath2DMgrInst CPath2DMgr::getInstancePtr()

class CGenGradient;
class CPath2DRenderer;
class CPath2DFlattener;

enum CPath2DPartType {
  CPATH_2D_PATH_PART_TYPE_NONE,
  CPATH_2D_PATH_PART_TYPE_MOVE_TO,
  CPATH_2D_PATH_PART_TYPE_LINE_TO,
  CPATH_2D_PATH_PART_TYPE_BEZIER2_TO,
  CPATH_2D_PATH_PART_TYPE_BEZIER3_TO,
  CPATH_2D_PATH_PART_TYPE_TEXT,
  CPATH_2D_PATH_PART_TYPE_CLOSE_PATH
};

struct CPath2DPart {
  CPath2DPart() :
   type(CPATH_2D_PATH_PART_TYPE_NONE), point() {
  }

  CPath2DPart(CPath2DPartType type1, const CPoint2D &point1) :
   type(type1), point(point1) {
  }

  CPath2DPart(const CPath2DPart &part) :
   type(part.type), point(part.point) {
  }

  const CPath2DPart &operator=(const CPath2DPart &part) {
    type  = part.type;
    point = part.point;

    return *this;
  }

  CPath2DPartType type;
  CPoint2D        point;
};

class CPath2DVisitor {
 public:
  virtual ~CPath2DVisitor() { }

  virtual void moveTo(const CPoint2D &point) = 0;
  virtual void lineTo(const CPoint2D &point) = 0;

  virtual void bezier2To(const CPoint2D &point1, const CPoint2D &point2) = 0;
  virtual void bezier3To(const CPoint2D &point1, const CPoint2D &point2,
                         const CPoint2D &point3) = 0;

  virtual void close() = 0;
};

class CPath2DStroker {
 public:
  virtual ~CPath2DStroker() { }

  virtual void adjustPoint(CPoint2D &p1) = 0;

  virtual void capLine(const CPoint2D &p1, const CPoint2D &p2) = 0;

  virtual void joinLines(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;

  virtual void drawLine(const CPoint2D &p1, const CPoint2D &p2) = 0;

  virtual void drawBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;

  virtual void drawBezier3(const CPoint2D &p1, const CPoint2D &p2,
                           const CPoint2D &p3, const CPoint2D &p4) = 0;
};

class CPath2DRendererStroker : public CPath2DStroker {
 public:
  CPath2DRendererStroker(CPath2DRenderer *renderer, CPath2DFlattener *flattener=NULL) :
   renderer_(renderer), flattener_(flattener) {
  }

  void adjustPoint(CPoint2D &p1);

  void capLine(const CPoint2D &p1, const CPoint2D &p2);

  void joinLines(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);

  void drawLine(const CPoint2D &p1, const CPoint2D &p2);

  void drawBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);

  void drawBezier3(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3, const CPoint2D &p4);

 private:
  CPath2DRendererStroker(const CPath2DRendererStroker &rhs);
  CPath2DRendererStroker &operator=(const CPath2DRendererStroker &rhs);

 private:
  CPath2DRenderer  *renderer_;
  CPath2DFlattener *flattener_;
};

class CPath2DFiller {
 public:
  virtual ~CPath2DFiller() { }

  virtual void addLine(const CPoint2D &p1, const CPoint2D &p2) = 0;

  virtual void addBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3) = 0;

  virtual void addBezier3(const CPoint2D &p1, const CPoint2D &p2,
                          const CPoint2D &p3, const CPoint2D &p4) = 0;

  virtual void fill(CFillType type) = 0;
};

class CPath2DRendererFiller : public CPath2DFiller {
 public:
  CPath2DRendererFiller(CPath2DRenderer *renderer) :
   renderer_(renderer) {
  }

  void addLine(const CPoint2D &p1, const CPoint2D &p2);

  void addBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);

  void addBezier3(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3, const CPoint2D &p4);

  void fill(CFillType type);

 private:
  CPath2DRendererFiller(const CPath2DRendererFiller &rhs);
  CPath2DRendererFiller &operator=(const CPath2DRendererFiller &rhs);

 private:
  CPath2DRenderer *renderer_;
};

class CPath2D {
 public:
  typedef std::vector<CPoint2D>    PointList;
  typedef std::vector<CPath2DPart> PartList;

 public:
  CPath2D();

  CPath2D(const CPath2D &path);

  virtual ~CPath2D();

  const CPath2D &operator=(const CPath2D &path);

  virtual CPath2D *dup();

  CPath2DRenderer *getRenderer() const { return renderer_; }

  virtual void setRenderer(CPath2DRenderer *renderer) { renderer_ = renderer; }

  CPath2DFlattener *getFlattener() const { return flattener_; }

  virtual void setFlattener(CPath2DFlattener *flattener) { flattener_ = flattener; }

  virtual bool getStrokeAdjust() const { return stroke_adjust_; }
  virtual void setStrokeAdjust(bool flag) { stroke_adjust_ = flag; }

  virtual void init();

  virtual void ellipse(const CPoint2D &center, double xr, double yr);

  virtual void text(const std::string &text, CFontPtr font);

  virtual void bezier(C3Bezier2D &bezier);
  virtual void bezier(C2Bezier2D &bezier);

  virtual void polygon(const PointList &points);

  virtual void roundedRectangle(const CPoint2D &point1, const CPoint2D &point2,
                                double rx, double ry);
  virtual void rectangle(const CPoint2D &point1, const CPoint2D &point2);

  virtual void arc (const CPoint2D &center, double xr, double yr, double angle1, double angle2);
  virtual void arcN(const CPoint2D &center, double xr, double yr, double angle1, double angle2);

  virtual bool arcTo(const CPoint2D &point1, const CPoint2D &point2, double xr, double yr);

  virtual bool bezier2To (const CPoint2D &point1, const CPoint2D &point2);
  virtual bool rbezier2To(const CPoint2D &point1, const CPoint2D &point2);

  virtual bool bezier3To (const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3);
  virtual bool rbezier3To(const CPoint2D &point1, const CPoint2D &point2, const CPoint2D &point3);

  virtual bool moveTo (const CPoint2D &point);
  virtual bool rmoveTo(const CPoint2D &point);

  virtual bool lineTo (const CPoint2D &point);
  virtual bool rlineTo(const CPoint2D &point);

  virtual bool getCurrentPoint (CPoint2D &point);
  virtual bool getCurrentPoint1(CPoint2D &point);

  virtual void showChar(int, CMatrix2D *, double *, double *);

  virtual void close();

  virtual void stroke();
  virtual void stroke(CPath2DRenderer *renderer);
  virtual void stroke(CPath2DStroker &stroker);

  virtual void strokePath();
  virtual void strokePath(CPath2DRenderer *renderer);

  virtual void setFillType(CFillType fill_type);

  virtual CFillType getFillType() const;

  virtual void fill();
  virtual void fill(CPath2DRenderer *renderer);
  virtual void fill(CPath2DFiller &filler);

  virtual void fillImage(CImagePtr image);
  virtual void fillImage(CImagePtr image, CPath2DRenderer *renderer);
  virtual void fillGradient(CRefPtr<CGenGradient> gradient);

  virtual void reverse();

  virtual void flatten();
  virtual void flatten(CPath2DFlattener *flattener);

  virtual void clip();
  virtual void eoclip();

  virtual void clipPath();

  virtual void getBounds(double *, double *, double *, double *);

  virtual void process(CPath2DVisitor &visitor) const;

  virtual void bbox(CBBox2D &bbox) const;

  virtual void transform(const CMatrix2D &matrix);

  virtual void print();

 protected:
  void strokeBezier2(CPath2DRenderer *renderer, const CPoint2D &point1, const CPoint2D &point2,
                     const CPoint2D &point3);
  void strokeBezier3(CPath2DRenderer *renderer, const CPoint2D &point1, const CPoint2D &point2,
                     const CPoint2D &point3, const CPoint2D &point4);

  void strokeLine     (CPath2DRenderer *renderer, const CPoint2D &point1, const CPoint2D &point2);
  void strokeCapLine  (CPath2DRenderer *renderer, const CPoint2D &point1, const CPoint2D &point2);
  void strokeJoinLines(CPath2DRenderer *renderer, const CPoint2D &point1, const CPoint2D &point2,
                       const CPoint2D &point3);

  void subFill(CImagePtr image, CFillType type, CPath2DRenderer *renderer);
  void subFill1(CImagePtr image, CFillType type, CPath2DRenderer *renderer);

  void subFlatFill(CImagePtr image, CFillType type, CPath2DRenderer *renderer);
  void subShapeFill(CFillType type, CPath2DRenderer *renderer);

  void subShapeFill(CFillType type, CPath2DFiller &filler);

  void subClip(CFillType type);
  void subClip(CFillType type, CPath2DRenderer *renderer);

  void transformPoint  (const CPoint2D &point, CPoint2D &point1);
  void untransformPoint(const CPoint2D &point, CPoint2D &point1);

 protected:
  typedef std::vector<CPoint2D> PointArray;

  CPath2DRenderer  *renderer_;
  CPath2DFlattener *flattener_;
  PartList          parts_;
  bool              closed_;
  CPoint2D          start1_;
  CPoint2D          start2_;
  CPoint2D          current_;
  bool              current_set_;
  bool              stroke_adjust_;
  bool              flat_fill_;
  CFillType         fill_type_;
  PointArray        fill_points_;

};

#endif
