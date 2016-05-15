#ifndef CSVGText_H
#define CSVGText_H

#include <CSVGObject.h>

class CSVGText : public CSVGObject {
 public:
  typedef std::vector<double> Reals;

 public:
  CSVG_OBJECT_DEF("text", CSVGObjTypeId::TEXT)

  CSVGText(CSVG &svg);
  CSVGText(const CSVGText &text);

  CSVGText *dup() const override;

  double getX() const { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  CScreenUnits getDX() const { return dx_.getValue(CScreenUnits(0)); }
  void setDX(const CScreenUnits x) { dx_ = x; }

  CScreenUnits getDY() const { return dy_.getValue(CScreenUnits(0)); }
  void setDY(const CScreenUnits y) { dy_ = y; }

  Reals getRotate() const { return rotate_.getValue(Reals()); }
  void setRotate(const Reals &r) { rotate_ = r; }

  std::string getTextLength() const { return textLength_.getValue(""); }
  void setTextLength(const std::string &s) { textLength_ = s; }

  std::string getLengthAdjust() const { return lengthAdjust_.getValue("spacing"); }
  void setLengthAdjust(const std::string &s) { lengthAdjust_ = s; }

  CPoint2D getPosition() const { return CPoint2D(getX(), getY()); }

  bool hasFont() const override { return true; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveTo(const CPoint2D &p) override;
  void moveBy(const CVector2D &delta) override;

  const CPoint2D &lastPos() const { return lastPos_; }
  void setLastPos(const CPoint2D &p) { lastPos_ = p; }

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGText &text);

 private:
  COptValT<double>       x_;
  COptValT<double>       y_;
  COptValT<CScreenUnits> dx_;
  COptValT<CScreenUnits> dy_;
  COptValT<Reals>        rotate_;
  COptValT<std::string>  textLength_;
  COptValT<std::string>  lengthAdjust_;
  CPoint2D               lastPos_;
};

#endif
