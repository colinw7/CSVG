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

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeComponentTransfer &fe);

 private:
  COptString filterIn_;
  COptString filterOut_;
};

#endif
