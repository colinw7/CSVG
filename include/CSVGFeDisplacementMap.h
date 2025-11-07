#ifndef CSVGFeDisplacementMap_H
#define CSVGFeDisplacementMap_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeDisplacementMap : public CSVGFilterBase, public CSVGPrintBase<CSVGFeDisplacementMap> {
 public:
  using Reals = std::vector<double>;

 public:
  CSVG_OBJECT_DEF("feDisplacementMap", CSVGObjTypeId::FE_DISPLACEMENT_MAP)

  CSVGFeDisplacementMap(CSVG &svg);
  CSVGFeDisplacementMap(const CSVGFeDisplacementMap &merge);

  CSVGFeDisplacementMap *dup() const override;

  std::string getFilterIn1() const;
  void setFilterIn1(const std::string &s) { filterIn1_ = s; }

  std::string getFilterIn2() const;
  void setFilterIn2(const std::string &s) { filterIn2_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  double getScale() const { return scale_.value_or(1); }
  void setScale(double r) { scale_ = r; }

  std::string getXChannel() const { return xChannelSelector_.value_or("R"); }
  void setXChannel(const std::string &c) { xChannelSelector_ = c; }

  std::string getYChannel() const { return yChannelSelector_.value_or("R"); }
  void setYChannel(const std::string &c) { yChannelSelector_ = c; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<std::string> filterIn1_;
  std::optional<std::string> filterIn2_;
  std::optional<std::string> filterOut_;
  std::optional<double>      scale_;
  std::optional<std::string> xChannelSelector_;
  std::optional<std::string> yChannelSelector_;
};

#endif
