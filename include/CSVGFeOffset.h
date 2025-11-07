#ifndef CSVGFeOffset_H
#define CSVGFeOffset_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeOffset : public CSVGFilterBase, public CSVGPrintBase<CSVGFeOffset> {
 public:
  CSVG_OBJECT_DEF("feOffset", CSVGObjTypeId::FE_OFFSET)

  CSVGFeOffset(CSVG &svg);
  CSVGFeOffset(const CSVGFeOffset &offset);

  CSVGFeOffset *dup() const override;

  bool hasFilterIn() const { return !!filterIn_; }
  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  bool hasFilterOut() const { return !!filterOut_; }
  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  double getDX() const { return dx_.value_or(0); }
  void setDX(double d) { dx_ = d; }

  double getDY() const { return dy_.value_or(0); }
  void setDY(double d) { dy_ = d; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  bool getSubRegion(CBBox2D &bbox) const override;

  void calcDelta(double &dx, double &dy) const;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<std::string> filterIn_;
  std::optional<std::string> filterOut_;
  std::optional<double>      dx_;
  std::optional<double>      dy_;
  bool                       drawing_ { false };
};

#endif
