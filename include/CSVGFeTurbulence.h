#ifndef CSVGFeTurbulence_H
#define CSVGFeTurbulence_H

#include <CSVGFilter.h>

class CSVGFeTurbulence : public CSVGFilter {
 public:
  CSVG_OBJECT_DEF("feTurbulence", CSVGObjTypeId::FE_TURBULENCE)

  CSVGFeTurbulence(CSVG &svg);
  CSVGFeTurbulence(const CSVGFeTurbulence &merge);

  CSVGFeTurbulence *dup() const override;

  std::string getType() const { return type_.getValue(""); }

  bool isFractalNoise() const { return (getType() == "fractalNoise"); }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeTurbulence &filter);

 private:
  COptValT<std::string> type_;
  COptValT<double>      baseFreq_;
  COptValT<int>         numOctaves_;
  COptValT<int>         seed_;
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
};

#endif
