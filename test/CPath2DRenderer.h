#ifndef CPATH2D_RENDERER_H
#define CPATH2D_RENDERER_H

#include <vector>

class CPath2DRenderer {
 public:
  CPath2DRenderer() { }

  virtual ~CPath2DRenderer() { }

  virtual void setTransform(bool set);

  virtual void adjustPoint(CPoint2D &p);

  virtual void transformPoint  (const CPoint2D &p1, CPoint2D &p2);
  virtual void untransformPoint(const CPoint2D &p1, CPoint2D &p2);

  virtual void drawLine(const CPoint2D &p1, const CPoint2D &p2) = 0;

  virtual void drawBezier2(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);
  virtual void drawBezier3(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3,
                           const CPoint2D &p4);

  virtual void drawText(const CPoint2D &p, const std::string &text);

  virtual void setTextSize(double w, double h);

  virtual void getTextLimits(const std::string &str, double *w, double *h);

  virtual void capLine  (const CPoint2D &p1, const CPoint2D &p2);
  virtual void joinLines(const CPoint2D &p1, const CPoint2D &p2, const CPoint2D &p3);

  virtual void initPolygons();

  virtual void addPolygon(const std::vector<CPoint2D> &points);

  virtual void fillPolygons  () = 0;
  virtual void eofillPolygons();

  virtual void imageFillPolygons  (CImagePtr image);
  virtual void imageEoFillPolygons(CImagePtr image);

  virtual uint getNumClipPolygons() const;

  virtual const std::vector< std::vector<CPoint2D> > &getClipPolygons(uint ind);

  virtual void clipPolygons  ();
  virtual void eoclipPolygons();

  virtual const CMatrix2D *getIViewMatrix();

  virtual void getPixelSize(double *px, double *py);

 protected:
  typedef std::vector<CPoint2D>   PointArray;
  typedef std::vector<PointArray> Polygons;

  Polygons polygons_;
};

#endif
