#ifndef CSVGText_H
#define CSVGText_H

#include <CSVGObject.h>

class CSVGText : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("text", CSVGObjTypeId::TEXT)

  CSVGText(CSVG &svg);
  CSVGText(const CSVGText &text);

  CSVGText *dup() const override;

  double getX() const { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  std::string getText() const override { return text_.getValue(""); }
  void setText(const std::string &text) override;

  CPoint2D getPosition() const { return CPoint2D(getX(), getY()); }

  bool hasFont() const override { return true; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveTo(const CPoint2D &p) override;
  void moveBy(const CVector2D &delta) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGText &text);

 private:
  COptValT<double>      x_;
  COptValT<double>      y_;
  COptValT<std::string> text_;
};

#endif
