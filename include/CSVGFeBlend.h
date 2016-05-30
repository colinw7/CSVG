#ifndef CSVGFeBlend_H
#define CSVGFeBlend_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeBlend : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feBlend", CSVGObjTypeId::FE_BLEND)

  CSVGFeBlend(CSVG &svg);
  CSVGFeBlend(const CSVGFeBlend &fe);

  CSVGFeBlend *dup() const override;

  CSVGBlendMode getMode() const { return mode_.getValue(CSVGBlendMode::NORMAL); }
  void setMode(CSVGBlendMode m) { mode_ = m; }

  std::string getFilterIn1() const { return filterIn1_.getValue("FilterGraphic"); }
  void setFilterIn1(const std::string &s) { filterIn1_ = s; }

  std::string getFilterIn2() const { return filterIn2_.getValue("FilterGraphic"); }
  void setFilterIn2(const std::string &s) { filterIn2_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("FilterGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void filterImage(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, CSVGBuffer *outBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeBlend &fe);

 private:
  COptValT<CSVGBlendMode> mode_;
  COptValT<std::string>   filterIn1_;
  COptValT<std::string>   filterIn2_;
  COptValT<std::string>   filterOut_;
};

#endif
