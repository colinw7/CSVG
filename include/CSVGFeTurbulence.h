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

  double getBaseFreqX(double r=0) const { return baseFreqX_.getValue(r); }
  void setBaseFreqX(double r) { baseFreqX_ = r; }

  double getBaseFreqY(double r=0) const { return baseFreqY_.getValue(r); }
  void setBaseFreqY(double r) { baseFreqY_ = r; }

  int getNumOctaves() const { return numOctaves_.getValue(1); }
  void setNumOctaves(int n) { numOctaves_ = n; }

  double getSeed() const { return seed_.getValue(0); }
  void setSeed(double r) { seed_ = r; }

  std::string getStitchTiles() const { return stitchTiles_.getValue("noStitch"); }
  void setStitchTiles(const std::string &s) { stitchTiles_ = s; }

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  COptString getNameValue(const std::string &name) const override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeTurbulence &filter);

 private:
  COptString type_;
  COptReal   baseFreqX_;
  COptReal   baseFreqY_;
  COptInt    numOctaves_;
  COptReal   seed_;
  COptString stitchTiles_;
  COptString filterIn_;
  COptString filterOut_;
};

#endif
