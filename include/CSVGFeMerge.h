#ifndef CSVGFeMerge_H
#define CSVGFeMerge_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeMerge : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feMerge", CSVGObjTypeId::FE_MERGE)

  CSVGFeMerge(CSVG &svg);
  CSVGFeMerge(const CSVGFeMerge &merge);

  CSVGFeMerge *dup() const override;

  std::string getFilterIn() const { return filterIn_.getValue("FilterGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("FilterGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void filterImage(CSVGBuffer *outBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeMerge &filter);

 private:
  COptValT<std::string> filterIn_;
  COptValT<std::string> filterOut_;
};

#endif
