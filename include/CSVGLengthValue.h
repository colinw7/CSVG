#ifndef CSVGLengthValue_H
#define CSVGLengthValue_H

class CSVGLengthValue {
 public:
  enum class Type {
    NONE,
    EM,
    EX,
    PT,
    PC,
    CM,
    MM,
    IN,
    PX,
    PERCENT
  };

  CSVGLengthValue(double value=0) :
   type_(Type::NONE), ivalue_(value), value_(value) {
  }

  CSVGLengthValue(Type type, double value) :
   type_(type), ivalue_(value), value_(value) {
  }

  CSVGLengthValue(Type type, double ivalue, double value) :
   type_(type), ivalue_(ivalue), value_(value) {
  }

  Type   type  () const { return type_  ; }
  double ivalue() const { return ivalue_; }
  double value () const { return value_ ; }

 public:
  Type   type_;
  double ivalue_;
  double value_;
};

#endif
