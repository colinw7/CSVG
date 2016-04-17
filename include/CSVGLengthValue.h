#ifndef CSVGLengthValue_H
#define CSVGLengthValue_H

#include <CSVGTypes.h>

class CSVGLengthValue {
 public:
  CSVGLengthValue(double value=0) :
   type_(CSVGLengthType::NONE), ivalue_(value), value_(value) {
  }

  CSVGLengthValue(CSVGLengthType type, double value) :
   type_(type), ivalue_(value), value_(value) {
  }

  CSVGLengthValue(CSVGLengthType type, double ivalue, double value) :
   type_(type), ivalue_(ivalue), value_(value) {
  }

  CSVGLengthType type() const { return type_; }

  double ivalue() const { return ivalue_; }
  double value () const { return value_ ; }

  double calcValue(double v) const {
    if (type_ == CSVGLengthType::PERCENT)
      return v*ivalue_/100.0;
    else
      return value_;
  }

 public:
  CSVGLengthType type_   { CSVGLengthType::NONE };
  double         ivalue_ { 0 };
  double         value_  { 0 };
};

#endif
