#ifndef CSVGInheritVal_H
#define CSVGInheritVal_H

template<typename T>
class CSVGInheritValT {
 public:
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

  const T &getValue() const { return value_; }
  void setValue(const T &value) { value_ = value; }

  bool isInherit() const { return inherit_; }
  void setInherit(bool b) { inherit_ = b; }

 private:
  T    value_;
  bool inherit_ { false };
};

#endif
