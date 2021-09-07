#ifndef CSVGFeMorphology_H
#define CSVGFeMorphology_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeMorphology : public CSVGFilterBase, public CSVGPrintBase<CSVGFeMorphology> {
 public:
  CSVG_OBJECT_DEF("feMorphology", CSVGObjTypeId::FE_MORPHOLOGY)

  CSVGFeMorphology(CSVG &svg);
  CSVGFeMorphology(const CSVGFeMorphology &merge);

  CSVGFeMorphology *dup() const override;

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  CSVGMorphologyOperator getOperator() const {
    return operator_.getValue(CSVGMorphologyOperator::ERODE); }
  void setOperator(CSVGMorphologyOperator o) { operator_ = o; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptString                       filterIn_;
  COptValT<CSVGMorphologyOperator> operator_;
  COptString                       radius_;
};

#endif
