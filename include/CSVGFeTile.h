#ifndef CSVGFeTile_H
#define CSVGFeTile_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeTile : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feTile", CSVGObjTypeId::FE_TILE)

  CSVGFeTile(CSVG &svg);
  CSVGFeTile(const CSVGFeTile &fe);

  CSVGFeTile *dup() const override;

  std::string getFilterIn() const { return filterIn_.getValue("FilterGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("FilterGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeTile &fe);

 private:
  COptString filterIn_;
  COptString filterOut_;
};

#endif
