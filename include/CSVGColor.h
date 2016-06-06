#ifndef CSVGColor_H
#define CSVGColor_H

#include <CRGBA.h>

class CSVGColor {
 public:
  enum class Type {
    NONE,
    CURRENT,
    RGBA,
    INHERIT
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
  bool isInherit() const { return type_ == Type::INHERIT; }

  void print(std::ostream &os) const {
    if      (type_ == Type::NONE   ) os << "none";
    else if (type_ == Type::CURRENT) os << "currentColor";
    else if (type_ == Type::INHERIT) os << "inherit";
    else                             os << rgba_.getRGB().stringEncode();
  }

  friend std::ostream &operator<<(std::ostream &os, const CSVGColor &color) {
    color.print(os);

    return os;
  }

 private:
  Type  type_ { Type::NONE };
  CRGBA rgba_;
};

#endif
