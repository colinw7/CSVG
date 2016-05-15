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
  enum class Type {
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

  CScreenUnits(double v, Type type=Type::PX) :
   value_(v), type_(type) {
  }

  CScreenUnits(Type type, double v) :
   value_(v), type_(type) {
  }

  virtual ~CScreenUnits() { }

  double value() const { return value_; }
  Type   type () const { return type_ ; }

  CScreenUnits em(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::EM) return *this;

    return toPixel(rvalue)/CScreenUnitsMgrInst->emSize();
  }

  CScreenUnits ex(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::EX) return *this;

    return toPixel(rvalue)/CScreenUnitsMgrInst->exSize();
  }

  CScreenUnits px(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::PX) return *this;

    return toPixel(rvalue);
  }

  CScreenUnits pt(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::PT) return *this;
    if (type_ == Type::PC) return 12.0*(*this);

    return 72.0*toInch(rvalue);
  }

  CScreenUnits pc(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::PC) return *this;
    if (type_ == Type::PT) return (*this)/12.0;

    return 6.0*toInch(rvalue);
  }

  CScreenUnits cm(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::CM) return *this;
    if (type_ == Type::MM) return 10.0*(*this);

    return 10*toMm(rvalue);
  }

  CScreenUnits mm(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::MM) return *this;
    if (type_ == Type::CM) return (*this)/10.0;

    return toMm(rvalue);
  }

  CScreenUnits in(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::IN) return *this;

    return toInch(rvalue);
  }

  CScreenUnits percent(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::PERCENT) return *this;

    double value = 100.0*toPixel(rvalue).value()/rvalue.toPixel(rvalue).value();

    return CScreenUnits(value, Type::PERCENT);
  }

  std::string typeName() const {
    return typeName(type_);
  }

  static std::string typeName(Type type) {
    switch (type) {
      case Type::EM:      return "em";
      case Type::EX:      return "ex";
      case Type::PX:      return "px";
      case Type::PT:      return "pt";
      case Type::PC:      return "pc";
      case Type::CM:      return "cm";
      case Type::MM:      return "mm";
      case Type::IN:      return "in";
      case Type::PERCENT: return "%";
      default:            assert(false); return "";
    }
  }

  friend CScreenUnits operator*(const CScreenUnits &lhs, double rhs) {
    return CScreenUnits(lhs.value_*rhs, lhs.type_);
  }

  friend CScreenUnits operator*(double lhs, const CScreenUnits &rhs) {
    return CScreenUnits(rhs.value_*lhs, rhs.type_);
  }

  friend CScreenUnits operator/(const CScreenUnits &lhs, double rhs) {
    return CScreenUnits(lhs.value_/rhs, lhs.type_);
  }

 private:
  CScreenUnits toPixel(const CScreenUnits &rvalue=CScreenUnits()) const {
    if (type_ == Type::PERCENT)
      return rvalue.toPixel()*value_/100.0;

    double value1 = 0.0;

    switch (type_) {
      case Type::EM: value1 = value_*CScreenUnitsMgrInst->emSize(); break;
      case Type::EX: value1 = value_*CScreenUnitsMgrInst->exSize(); break;
      case Type::PX: value1 = value_; break;
      case Type::PT: value1 = (CScreenUnitsMgrInst->mmSize()*25.4*value_)/72.0; break;
      case Type::PC: value1 = (CScreenUnitsMgrInst->mmSize()*25.4*value_)/6.0; break;
      case Type::CM: value1 = (CScreenUnitsMgrInst->mmSize()*10.0*value_); break;
      case Type::MM: value1 = (CScreenUnitsMgrInst->mmSize()*value_); break;
      case Type::IN: value1 = (CScreenUnitsMgrInst->mmSize()*25.4*value_); break;
      default:       assert(false); break;
    }

    return CScreenUnits(value1, Type::PX);
  }

  CScreenUnits toMm(const CScreenUnits &rvalue=CScreenUnits()) const {
    return toPixel(rvalue)/CScreenUnitsMgrInst->mmSize();
  }

  CScreenUnits toInch(const CScreenUnits &rvalue=CScreenUnits()) const {
    return toMm(rvalue)/25.4;
  }

 private:
  double value_ { 0.0 };
  Type   type_  { Type::PX };
};

#endif
