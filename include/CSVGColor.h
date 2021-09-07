#ifndef CSVGColor_H
#define CSVGColor_H

#include <CSVGTmpl.h>
#include <CRGBA.h>

class CSVGColor : public CSVGEqBase<CSVGColor>, public CSVGPrintBase<CSVGColor> {
 public:
  enum class Type {
    NONE,
    CURRENT,
    RGBA
  };

 public:
  CSVGColor(Type type=Type::NONE) :
   type_(type) {
  }

  CSVGColor(const CRGBA &rgba) :
   type_(Type::RGBA), rgba_(rgba) {
  }

  const Type &type() const { return type_; }
  void setType(const Type &v) { type_ = v; }

  const CRGBA &rgba() const { assert(type_ == Type::RGBA); return rgba_; }
  void setRgba(const CRGBA &v) { rgba_ = v; }

  bool isNone   () const { return type_ == Type::NONE;    }
  bool isCurrent() const { return type_ == Type::CURRENT; }
  bool isRGBA   () const { return type_ == Type::RGBA;    }

  friend bool operator==(const CSVGColor &lhs, const CSVGColor &rhs) {
    return lhs.rgba() == rhs.rgba();
  }

  void print(std::ostream &os) const {
    if      (type_ == Type::NONE   ) os << "none";
    else if (type_ == Type::CURRENT) os << "currentColor";
    else                             os << rgba_.getRGB().stringEncode();
  }

 private:
  Type  type_ { Type::NONE };
  CRGBA rgba_;
};

#endif
