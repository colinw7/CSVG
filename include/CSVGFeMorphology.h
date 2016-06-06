#ifndef CSVGFeMorphology_H
#define CSVGFeMorphology_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeMorphology : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feMorphology", CSVGObjTypeId::FE_MORPHOLOGY)

  CSVGFeMorphology(CSVG &svg);
  CSVGFeMorphology(const CSVGFeMorphology &merge);

  CSVGFeMorphology *dup() const override;

  std::string getFilterIn() const { return filterIn_.getValue("FilterGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  CSVGMorphologyOperator getOperator() const {
    return operator_.getValue(CSVGMorphologyOperator::ERODE); }
  void setOperator(CSVGMorphologyOperator o) { operator_ = o; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void filterImage(CSVGBuffer *inBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeMorphology &filter);

 private:
  COptString                       filterIn_;
  COptValT<CSVGMorphologyOperator> operator_;
  COptString                       radius_;
};

#endif
