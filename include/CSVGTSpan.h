#ifndef CSVGTSpan_H
#define CSVGTSpan_H

#include <CSVGObject.h>

class CSVGTSpan : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("tspan", CSVGObjTypeId::TSPAN)

  CSVGTSpan(CSVG &svg);
  CSVGTSpan(const CSVGTSpan &span);

  CSVGTSpan *dup() const override;

  void setX(double x) { position_.x = x; }
  void setY(double y) { position_.y = y; }

  std::string getText() const override { return text_; }
  void setText(const std::string &text) override { text_ = text; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGTSpan &tspan);

 private:
  CSVGTSpan &operator=(const CSVGTSpan &rhs);

 private:
  CPoint2D    position_;
  std::string text_;
};

#endif
