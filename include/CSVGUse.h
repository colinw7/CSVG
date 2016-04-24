#ifndef CSVGUse_H
#define CSVGUse_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGUse : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("use", CSVGObjTypeId::USE)

  CSVGUse(CSVG &svg);
  CSVGUse(const CSVGUse &use);

  CSVGUse *dup() const override;

  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  CSVGObject *getLinkObject() const;

  double getX() const { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  double getWidth() const { return width_.isValid() ? width_ .getValue().value() : 1; }
  void setWidth(double w) { width_ = w; }

  double getHeight() const { return height_.isValid() ? height_.getValue().value() : 1; }
  void setHeight(double h) { height_ = h; }

  bool processOption(const std::string &name, const std::string &value) override;

  void termParse();

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGUse &use);

 private:
  COptValT<CSVGXLink>       xlink_;
  COptValT<double>          x_;
  COptValT<double>          y_;
  COptValT<CSVGLengthValue> width_;
  COptValT<CSVGLengthValue> height_;
};

#endif
