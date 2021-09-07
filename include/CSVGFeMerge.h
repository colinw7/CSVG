#ifndef CSVGFeMerge_H
#define CSVGFeMerge_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeMerge : public CSVGFilterBase, public CSVGPrintBase<CSVGFeMerge> {
 public:
  CSVG_OBJECT_DEF("feMerge", CSVGObjTypeId::FE_MERGE)

  CSVGFeMerge(CSVG &svg);
  CSVGFeMerge(const CSVGFeMerge &merge);

  CSVGFeMerge *dup() const override;

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptString filterIn_;
  COptString filterOut_;
};

#endif
