#ifndef CSVGStop_H
#define CSVGStop_H

#include <CSVGObject.h>
#include <CSVGColor.h>

class CSVGStop : public CSVGObject, public CSVGPrintBase<CSVGStop> {
 public:
  CSVG_OBJECT_DEF("stop", CSVGObjTypeId::STOP)

  CSVGStop(CSVG &svg);
  CSVGStop(const CSVGStop &stop);

  CSVGStop *dup() const override;

  bool hasOffset() const { return offset_.isValid(); }
  CScreenUnits getOffset() const { return offset_.getValue(CScreenUnits(0)); }
  void setOffset(double offset) { offset_ = CScreenUnits(offset); }

  bool hasColor() const { return color_.isValid(); }
  Color getColor() const { return color_.getValue(Color()); }
  void setColor(const Color &color) { color_ = color; }

  bool hasOpacity() const { return opacity_.isValid(); }
  double getOpacity() const { return opacity_.getValue(1); }
  void setOpacity(double opacity) { opacity_ = opacity; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptValT<CScreenUnits> offset_;
  COptValT<Color>        color_;
  COptReal               opacity_;
};

#endif
