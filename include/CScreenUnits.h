#ifndef CScreenUnits_H
#define CScreenUnits_H

#include <cassert>
#include <string>

#define CScreenUnitsMgrInst CScreenUnitsMgr::instance()

class CScreenUnitsMgr {
 public:
  static CScreenUnitsMgr *instance() {
    static CScreenUnitsMgr *inst;

    if (! inst)
      inst = new CScreenUnitsMgr;

    return inst;
  }

  // em size in pixels
  double emSize() const { return emSize_; }
  void setEmSize(double r) { emSize_ = r; }

  // ex size in pixels
  double exSize() const { return exSize_; }
  void setExSize(double r) { exSize_ = r; }

  // pixels (dots) per inch
  double dpi() const { return dpi_; }
  void setDpi(double r) { dpi_ = r; }

  // mm size in pixels
  double mmSize() const { return dpi()/25.4; }

 private:
  CScreenUnitsMgr() { }
 ~CScreenUnitsMgr() { }

 private:
  double emSize_ { 1 };
  double exSize_ { 1 };
  double dpi_    { 75.0 };
};

//------

class CScreenUnits {
 public:
  enum class Units {
    EM,
    EX,
    PX,
    PT,
    PC,
    CM,
    MM,
    IN,
    PERCENT
  };

 public:
  CScreenUnits() { }

  CScreenUnits(double v, Units units=Units::PX) :
   value_(v), units_(units) {
  }

  CScreenUnits(Units units, double v) :
   value_(v), units_(units) {
  }

  virtual ~CScreenUnits() { }

  //---

  double value(Units units=Units::PX) const {
    assert(units_ == units);

    return value_;
  }

  Units units() const { return units_; }

  const CScreenUnits &setValue(double v, Units units=Units::PX) {
    assert(units_ == units);

    value_ = v;

    return *this;
  }

  const CScreenUnits &setUnits(Units units, const CScreenUnits &rvalue=CScreenUnits()) {
    if (units == units_) return *this;

    switch (units) {
      case Units::EM     : value_ = em     (rvalue).value_; break;
      case Units::EX     : value_ = ex     (rvalue).value_; break;
      case Units::PX     : value_ = px     (rvalue).value_; break;
      case Units::PT     : value_ = pt     (rvalue).value_; break;
      case Units::PC     : value_ = pc     (rvalue).value_; break;
      case Units::CM     : value_ = cm     (rvalue).value_; break;
      case Units::MM     : value_ = mm     (rvalue).value_; break;
      case Units::IN     : value_ = in     (rvalue).value_; break;
      case Units::PERCENT: value_ = percent(rvalue).value_; break;
      default: assert(false); break;
    }

    units_ = units;

    return *this;
  }

  int ivalue() const {
    assert(units_ == Units::PX);

    if (value_ >= 0)
      return int(value_ + 0.5);
    else
      return int(value_ - 0.5);
  }

  //---

  CScreenUnits em(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::EM) return *this;

    return CScreenUnits(toPixel(rvalue)/CScreenUnitsMgrInst->emSize(), Units::EM);
  }

  CScreenUnits ex(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::EX) return *this;

    return CScreenUnits(toPixel(rvalue)/CScreenUnitsMgrInst->exSize(), Units::EX);
  }

  CScreenUnits px(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::PX) return *this;

    return CScreenUnits(toPixel(rvalue), Units::PX);
  }

  CScreenUnits pt(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::PT) return *this;

    return CScreenUnits(72.0*toInch(rvalue), Units::PT);
  }

  CScreenUnits pc(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::PC) return *this;

    return CScreenUnits(6.0*toInch(rvalue), Units::PC);
  }

  CScreenUnits cm(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::CM) return *this;

    return CScreenUnits(toMm(rvalue)/10.0, Units::CM);
  }

  CScreenUnits mm(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::MM) return *this;

    return CScreenUnits(toMm(rvalue), Units::MM);
  }

  CScreenUnits in(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::IN) return *this;

    return CScreenUnits(toInch(rvalue), Units::IN);
  }

  CScreenUnits percent(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::PERCENT) return *this;

    double value = 100.0*toPixel(rvalue)/rvalue.toPixel(rvalue);

    return CScreenUnits(value, Units::PERCENT);
  }

  //---

  std::string typeName() const {
    return typeName(units_);
  }

  static std::string typeName(Units units) {
    switch (units) {
      case Units::EM:      return "em";
      case Units::EX:      return "ex";
      case Units::PX:      return "px";
      case Units::PT:      return "pt";
      case Units::PC:      return "pc";
      case Units::CM:      return "cm";
      case Units::MM:      return "mm";
      case Units::IN:      return "in";
      case Units::PERCENT: return "%";
      default: assert(false); return "";
    }
  }

  //---

  friend bool operator<(const CScreenUnits &lhs, const CScreenUnits &rhs) {
    return lhs.px().ivalue() < rhs.px().ivalue();
  }

  friend bool operator<=(const CScreenUnits &lhs, const CScreenUnits &rhs) {
    return lhs.px().ivalue() <= rhs.px().ivalue();
  }

  friend bool operator>(const CScreenUnits &lhs, const CScreenUnits &rhs) {
    return lhs.px().ivalue() > rhs.px().ivalue();
  }

  friend bool operator>=(const CScreenUnits &lhs, const CScreenUnits &rhs) {
    return lhs.px().ivalue() >= rhs.px().ivalue();
  }

  friend bool operator==(const CScreenUnits &lhs, const CScreenUnits &rhs) {
    return lhs.px().ivalue() == rhs.px().ivalue();
  }

  friend bool operator!=(const CScreenUnits &lhs, const CScreenUnits &rhs) {
    return lhs.px().ivalue() != rhs.px().ivalue();
  }

  friend CScreenUnits operator*(const CScreenUnits &lhs, double rhs) {
    return CScreenUnits(lhs.value_*rhs, lhs.units_);
  }

  friend CScreenUnits operator*(double lhs, const CScreenUnits &rhs) {
    return CScreenUnits(rhs.value_*lhs, rhs.units_);
  }

  friend CScreenUnits operator/(const CScreenUnits &lhs, double rhs) {
    return CScreenUnits(lhs.value_/rhs, lhs.units_);
  }

  //---

 private:
  double toPixel(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (units_ == Units::PERCENT)
      return rvalue.toPixel()*value_/100.0;

    double value1 = 0.0;

    switch (units_) {
      case Units::EM: value1 = value_*CScreenUnitsMgrInst->emSize(); break;
      case Units::EX: value1 = value_*CScreenUnitsMgrInst->exSize(); break;
      case Units::PX: value1 = value_; break;
      case Units::PT: value1 = (CScreenUnitsMgrInst->mmSize()*25.4*value_)/72.0; break;
      case Units::PC: value1 = (CScreenUnitsMgrInst->mmSize()*25.4*value_)/6.0; break;
      case Units::CM: value1 = (CScreenUnitsMgrInst->mmSize()*10.0*value_); break;
      case Units::MM: value1 = (CScreenUnitsMgrInst->mmSize()*value_); break;
      case Units::IN: value1 = (CScreenUnitsMgrInst->mmSize()*25.4*value_); break;
      default: assert(false); break;
    }

    return value1;
  }

  double toMm(const CScreenUnits &rvalue=CScreenUnits()) const {
    return toPixel(rvalue)/CScreenUnitsMgrInst->mmSize();
  }

  double toInch(const CScreenUnits &rvalue=CScreenUnits()) const {
    return toMm(rvalue)/25.4;
  }

 private:
  double value_ { 0.0 };
  Units  units_ { Units::PX };
};

#endif
