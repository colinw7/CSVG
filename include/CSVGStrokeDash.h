#ifndef CSVG_STROKE_DASH_H
#define CSVG_STROKE_DASH_H

#include <CSVGTmpl.h>

#include <CScreenUnits.h>
#include <CLineDash.h>
#include <optional>

class CSVGStrokeDash : public CSVGPrintBase<CSVGStrokeDash> {
 public:
  using Dashes = std::vector<CScreenUnits>;

 public:
  CSVGStrokeDash(const Dashes &dashes=Dashes()) : dashes_(dashes) { }

  bool isSolid() const { return solid_; }
  void setSolid(bool b) { solid_ = b; }

  const Dashes &dashes() const { return dashes_; }
  void setDashes(const Dashes &v) { dashes_ = v; }

  bool isNone() const { return ! isSolid() && dashes_.empty(); }

  bool hasOffset() const { return !!offset_; }
  CScreenUnits offset() const { return offset_.value_or(CScreenUnits(0)); }
  void setOffset(const CScreenUnits &v) { offset_ = v; }

  void addDash(const CScreenUnits &dash) { dashes_.push_back(dash); }

  CLineDash getLineDash(double l=0) const;
  void setLineDash(const CLineDash &d);

  void printDashes(std::ostream &os) const {
    int i = 0;

    for (const auto &d : dashes_) {
      if (i > 0) os << " ";

      os << d;

      ++i;
    }
  }

  void print(std::ostream &os) const {
    if      (isSolid())
      os << "solid";
    else if (isNone())
      os << "none";
    else {
      printDashes(os);

      os << offset_.value();
    }
  }

  void printType(std::ostream &os) const {
    if      (isSolid())
      os << "solid";
    else if (isNone())
      os << "none";
    else
      printDashes(os);
  }

  void printOffset(std::ostream &os) const {
    os << offset_.value();
  }

 private:
  bool                        solid_ { true };
  Dashes                      dashes_;
  std::optional<CScreenUnits> offset_;
};

#endif
