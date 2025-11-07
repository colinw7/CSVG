#ifndef CSVGImage_H
#define CSVGImage_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGImage : public CSVGObject, public CSVGPrintBase<CSVGImage> {
 public:
  CSVG_OBJECT_DEF("image", CSVGObjTypeId::IMAGE)

  CSVGImage(CSVG &svg);
  CSVGImage(const CSVGImage &image);

 ~CSVGImage();

  CSVGImage *dup() const override;

  const CSVGXLink &xlink() const { return xlink_.value(); }

  std::string getLinkName() const { return (xlink_ ? xlink_.value().str() : ""); }
  void setLinkName(const std::string &str);

  double getX() const { return x_.value_or(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.value_or(0); }
  void setY(double y) { y_ = y; }

  double getWidth () const { return w_.value_or(CScreenUnits(100)).px().value(); }
  void setWidth(double w) { w_ = CScreenUnits(w); }

  double getHeight() const { return h_.value_or(CScreenUnits(100)).px().value(); }
  void setHeight(double h) { h_ = CScreenUnits(h); }

  CPoint2D getPosition() const { return CPoint2D(getX(), getY()); }

  bool getSize(CSize2D &size) const override;

  void setOrigin(const CPoint2D &point);
  void setSize(const CSize2D &size);

  CSVGPreserveAspect preserveAspect() const {
    return preserveAspect_.value_or(CSVGPreserveAspect()); }
  void setPreserveAspect(const CSVGPreserveAspect &a) { preserveAspect_ = a; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  bool initImage() const;

  CSVGObject *getObject() const;

  bool getBBox(CBBox2D &bbox) const override;

  void resizeTo(const CSize2D &size) override;

  CSVGBuffer *getImageBuffer() const;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  void init();

  CSize2D getSizeInternal() const { return CSize2D(getWidth(), getHeight()); }

  void moveDelta(const CVector2D &delta) override;

 private:
  std::optional<CSVGXLink>          xlink_;
  std::optional<double>             x_;
  std::optional<double>             y_;
  std::optional<CScreenUnits>       w_;
  std::optional<CScreenUnits>       h_;
  std::optional<CSVGPreserveAspect> preserveAspect_;
  std::optional<std::string>        colorProfile_;
};

#endif
