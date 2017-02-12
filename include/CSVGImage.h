#ifndef CSVGImage_H
#define CSVGImage_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGImage : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("image", CSVGObjTypeId::IMAGE)

  CSVGImage(CSVG &svg);
  CSVGImage(const CSVGImage &image);

 ~CSVGImage();

  CSVGImage *dup() const override;

  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  double getX() const { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  double getWidth () const { return w_.getValue(CScreenUnits(100)).px().value(); }
  void setWidth(double w) { w_ = CScreenUnits(w); }

  double getHeight() const { return h_.getValue(CScreenUnits(100)).px().value(); }
  void setHeight(double h) { h_ = CScreenUnits(h); }

  CPoint2D getPosition() const { return CPoint2D(getX(), getY()); }

  bool getSize(CSize2D &size) const override;

  void setOrigin(const CPoint2D &point);
  void setSize(const CSize2D &size);

  CSVGPreserveAspect preserveAspect() const {
    return preserveAspect_.getValue(CSVGPreserveAspect()); }
  void setPreserveAspect(const CSVGPreserveAspect &a) { preserveAspect_ = a; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  bool initImage() const;

  CSVGObject *getObject() const;

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;
  void resizeTo(const CSize2D &size) override;

  CSVGBuffer *getImageBuffer() const;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGImage &image);

 private:
  CSize2D getSizeInternal() const { return CSize2D(getWidth(), getHeight()); }

 private:
  COptValT<CSVGXLink>          xlink_;
  COptReal                     x_;
  COptReal                     y_;
  COptValT<CScreenUnits>       w_;
  COptValT<CScreenUnits>       h_;
  COptValT<CSVGPreserveAspect> preserveAspect_;
  COptString                   colorProfile_;
};

#endif
