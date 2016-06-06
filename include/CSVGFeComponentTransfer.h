#ifndef CSVGFeComponentTransfer_H
#define CSVGFeComponentTransfer_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeComponentTransfer : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feComponentTransfer", CSVGObjTypeId::FE_COMPONENT_TRANSFER)

  CSVGFeComponentTransfer(CSVG &svg);
  CSVGFeComponentTransfer(const CSVGFeComponentTransfer &fe);

  CSVGFeComponentTransfer *dup() const override;

  std::string getFilterIn() const { return filterIn_.getValue("FilterGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("FilterGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeComponentTransfer &fe);

 private:
  COptString filterIn_;
  COptString filterOut_;
};

#endif
