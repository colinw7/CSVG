#ifndef CSVGFeTurbulence_H
#define CSVGFeTurbulence_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeTurbulence : public CSVGFilterBase, public CSVGPrintBase<CSVGFeTurbulence> {
 public:
  CSVG_OBJECT_DEF("feTurbulence", CSVGObjTypeId::FE_TURBULENCE)

  CSVGFeTurbulence(CSVG &svg);
  CSVGFeTurbulence(const CSVGFeTurbulence &merge);

  CSVGFeTurbulence *dup() const override;

  std::string getType() const { return type_.value_or("turbulence"); }
  void setType(const std::string &s) { type_ = s; }

  bool isFractalNoise() const { return (getType() == "fractalNoise"); }
  void setFractalNoise(bool b) { setType(b ? "fractalNoise" : "turbulence"); }

  double getBaseFreqX(double r=0) const { return baseFreqX_.value_or(r); }
  void setBaseFreqX(double r) { baseFreqX_ = r; }

  double getBaseFreqY(double r=0) const { return baseFreqY_.value_or(r); }
  void setBaseFreqY(double r) { baseFreqY_ = r; }

  int getNumOctaves() const { return numOctaves_.value_or(1); }
  void setNumOctaves(int n) { numOctaves_ = n; }

  double getSeed() const { return seed_.value_or(0); }
  void setSeed(double r) { seed_ = r; }

  std::string getStitchTiles() const { return stitchTiles_.value_or("noStitch"); }
  void setStitchTiles(const std::string &s) { stitchTiles_ = s; }

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  std::optional<std::string> getNameValue(const std::string &name) const override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<std::string> type_;
  std::optional<double>      baseFreqX_;
  std::optional<double>      baseFreqY_;
  std::optional<int>         numOctaves_;
  std::optional<double>      seed_;
  std::optional<std::string> stitchTiles_;
  std::optional<std::string> filterIn_;
  std::optional<std::string> filterOut_;
};

#endif
