#ifndef CSVGFeGaussianBlur_H
#define CSVGFeGaussianBlur_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeGaussianBlur : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feGaussianBlur", CSVGObjTypeId::FE_GAUSSIAN_BLUR)

  CSVGFeGaussianBlur(CSVG &svg);
  CSVGFeGaussianBlur(const CSVGFeGaussianBlur &blur);

  CSVGFeGaussianBlur *dup() const override;

  std::string getFilterIn() const { return filterIn_.getValue("FilterGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("FilterGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  double getStdDev() const { return stdDev_.getValue(0); }
  void setStdDev(double d) { stdDev_ = d; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeGaussianBlur &filter);

 private:
  COptValT<std::string> filterIn_;
  COptValT<std::string> filterOut_;
  COptValT<double>      stdDev_;
};

#endif
