#ifndef CSVGTSpan_H
#define CSVGTSpan_H

#include <CSVGObject.h>

class CSVGTSpan : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("tspan", CSVGObjTypeId::TSPAN)

  CSVGTSpan(CSVG &svg);
  CSVGTSpan(const CSVGTSpan &span);

  CSVGTSpan *dup() const override;

  double getX() const { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  std::string getText() const override { return text_.getValue(""); }
  void setText(const std::string &text) override { text_ = text; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGTSpan &tspan);

 private:
  CSVGTSpan &operator=(const CSVGTSpan &rhs);

 private:
  COptValT<double>      x_;
  COptValT<double>      y_;
  COptValT<std::string> text_;
};

#endif
