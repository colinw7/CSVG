#ifndef CSVGFeTurbulence_H
#define CSVGFeTurbulence_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeTurbulence : public CSVGFilterBase {
 public:
  CSVG_OBJECT_DEF("feTurbulence", CSVGObjTypeId::FE_TURBULENCE)

  CSVGFeTurbulence(CSVG &svg);
  CSVGFeTurbulence(const CSVGFeTurbulence &merge);

  CSVGFeTurbulence *dup() const override;

  std::string getType() const { return type_.getValue("turbulence"); }
  void setType(const std::string &s) { type_ = s; }

  bool isFractalNoise() const { return (getType() == "fractalNoise"); }
  void setFractalNoise(bool b) { setType(b ? "fractalNoise" : "turbulence"); }

  double getBaseFreq() const { return baseFreq_.getValue(0); }
  void setBaseFreq(double r) { baseFreq_ = r; }

  int getNumOctaves() const { return numOctaves_.getValue(1); }
  void setNumOctaves(int i) { numOctaves_ = i; }

  int getSeed() const { return seed_.getValue(0); }
  void setSeed(int i) { seed_ = i; }

  std::string getStitchTiles() const { return stitchTiles_.getValue("noStitch"); }
  void setStitchTiles(const std::string &s) { stitchTiles_ = s; }

  std::string getFilterIn() const { return filterIn_.getValue("FilterGraphic"); }
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const { return filterOut_.getValue("FilterGraphic"); }
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void filterImage(CSVGBuffer *inBuffer, CSVGBuffer *outBuffer);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeTurbulence &filter);

 private:
  COptValT<std::string> type_;
  COptValT<double>      baseFreq_;
  COptValT<int>         numOctaves_;
  COptValT<int>         seed_;
  COptValT<std::string> stitchTiles_;
  COptValT<std::string> filterIn_;
  COptValT<std::string> filterOut_;
};

#endif
