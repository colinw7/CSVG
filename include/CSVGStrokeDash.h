#ifndef CSVG_STROKE_DASH_H
#define CSVG_STROKE_DASH_H

#include <CScreenUnits.h>
#include <CLineDash.h>
#include <COptVal.h>

class CSVGStrokeDash {
 public:
  using Dashes = std::vector<CScreenUnits>;

 public:
  CSVGStrokeDash(const Dashes &dashes=Dashes()) : dashes_(dashes) { }

  bool isSolid() const { return solid_; }
  void setSolid(bool b) { solid_ = b; }

  const Dashes &dashes() const { return dashes_; }
  void setDashes(const Dashes &v) { dashes_ = v; }

  bool isNone() const { return ! isSolid() && dashes_.empty(); }

  bool hasOffset() const { return offset_.isValid(); }
  CScreenUnits offset() const { return offset_.getValue(CScreenUnits(0)); }
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
    printDashes(os);

    os << offset_;
  }

  friend std::ostream &operator<<(std::ostream &os, const CSVGStrokeDash &dash) {
    dash.print(os);

    return os;
  }

 private:
  bool                   solid_ { true };
  Dashes                 dashes_;
  COptValT<CScreenUnits> offset_;
};

#endif
