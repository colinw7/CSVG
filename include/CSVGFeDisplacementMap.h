#ifndef CSVGFeDisplacementMap_H
#define CSVGFeDisplacementMap_H

#include <CSVGFilterBase.h>

class CSVGBuffer;

class CSVGFeDisplacementMap : public CSVGFilterBase {
 public:
  typedef std::vector<double> Reals;

 public:
  CSVG_OBJECT_DEF("feDisplacementMap", CSVGObjTypeId::FE_DISPLACEMENT_MAP)

  CSVGFeDisplacementMap(CSVG &svg);
  CSVGFeDisplacementMap(const CSVGFeDisplacementMap &merge);

  CSVGFeDisplacementMap *dup() const override;

  std::string getFilterIn1() const;
  void setFilterIn1(const std::string &s) { filterIn1_ = s; }

  std::string getFilterIn2() const;
  void setFilterIn2(const std::string &s) { filterIn2_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  double getScale() const { return scale_.getValue(1); }
  void setScale(double r) { scale_ = r; }

  std::string getXChannel() const { return xChannelSelector_.getValue("R"); }
  void setXChannel(const std::string &c) { xChannelSelector_ = c; }

  std::string getYChannel() const { return yChannelSelector_.getValue("R"); }
  void setYChannel(const std::string &c) { yChannelSelector_ = c; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeDisplacementMap &filter);

 private:
  COptString filterIn1_;
  COptString filterIn2_;
  COptString filterOut_;
  COptReal   scale_;
  COptString xChannelSelector_;
  COptString yChannelSelector_;
};

#endif
