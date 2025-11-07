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

  bool hasOffset() const { return !!offset_; }
  CScreenUnits getOffset() const { return offset_.value_or(CScreenUnits(0)); }
  void setOffset(double offset) { offset_ = CScreenUnits(offset); }

  bool hasColor() const { return !!color_; }
  Color getColor() const { return color_.value_or(Color()); }
  void setColor(const Color &color) { color_ = color; }

  bool hasOpacity() const { return !!opacity_; }
  double getOpacity() const { return opacity_.value_or(1); }
  void setOpacity(double opacity) { opacity_ = opacity; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<CScreenUnits> offset_;
  std::optional<Color>        color_;
  std::optional<double>       opacity_;
};

#endif
