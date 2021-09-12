#ifndef CSVGTSpan_H
#define CSVGTSpan_H

#include <CSVGObject.h>

class CSVGText;

class CSVGTSpan : public CSVGObject, public CSVGPrintBase<CSVGTSpan> {
 public:
  using Reals = std::vector<double>;

 public:
  CSVG_OBJECT_DEF("tspan", CSVGObjTypeId::TSPAN)

  CSVGTSpan(CSVG &svg);
  CSVGTSpan(const CSVGTSpan &span);

  CSVGTSpan *dup() const override;

  double getX() const { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  Reals getDX() const { return dx_.isValid() ? dx_.getValue() : Reals(); }
  void setDX(const Reals &x) { dx_ = x; }

  Reals getDY() const { return dy_.isValid() ? dy_.getValue() : Reals(); }
  void setDY(const Reals &y) { dy_ = y; }

  bool hasFont() const override { return true; }

  void setText(const std::string &text) override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  void getDrawPos(double &x, double &y, int i) const;

  CSVGText *getParentText() const;

  void moveDelta(const CVector2D &delta) override;

 private:
  CSVGTSpan &operator=(const CSVGTSpan &rhs);

 private:
  COptReal        x_;
  COptReal        y_;
  COptValT<Reals> dx_;
  COptValT<Reals> dy_;
};

#endif
