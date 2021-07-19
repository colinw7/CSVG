#ifndef CSVGInheritVal_H
#define CSVGInheritVal_H

#include <iostream>
#include <cassert>

template<typename T>
class CSVGInheritValT {
 public:
  static CSVGInheritValT inherit() {
    CSVGInheritValT val; val.setInherit(true); return val;
  }

  CSVGInheritValT() :
   value_() {
  }

  explicit CSVGInheritValT(const T &value) :
   value_(value) {
  }

  CSVGInheritValT(const CSVGInheritValT &rhs) :
   value_(rhs.value_), inherit_(rhs.inherit_) {
  }

  CSVGInheritValT &operator=(const CSVGInheritValT &rhs) {
    value_   = rhs.value_;
    inherit_ = rhs.inherit_;

    return *this;
  }

  CSVGInheritValT &operator=(const T &value) {
    setValue(value);

    return *this;
  }

  const T &getValue() const { assert(! isInherit()); return value_; }
  void setValue(const T &value) { value_ = value; inherit_ = false; }

  bool isInherit() const { return inherit_; }
  void setInherit(bool b) { inherit_ = b; }

  friend std::ostream &operator<<(std::ostream &os, const CSVGInheritValT &v) {
    if (v.isInherit())
      os << "inherit";
    else
      os << v.getValue();

    return os;
  }

 private:
  T    value_;
  bool inherit_ { false };
};

#endif
