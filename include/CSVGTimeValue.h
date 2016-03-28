#ifndef CSVGTimeValue_H
#define CSVGTimeValue_H

class CSVGTimeValue {
 public:
  enum class Type {
    NONE,
    HOURS,
    MINUTES,
    SECONDS,
    MILLISECONDS
  };

  CSVGTimeValue(double value=0) :
   type_(Type::NONE), value_(value) {
  }

  CSVGTimeValue(Type type, double value) :
   type_(type), value_(value) {
  }

  Type   type () const { return type_ ; }
  double value() const { return value_; }

  double getSeconds() const {
    if      (type_ == Type::HOURS       ) return value_*3600.0;
    else if (type_ == Type::MINUTES     ) return value_*60.0;
    else if (type_ == Type::SECONDS     ) return value_;
    else if (type_ == Type::MILLISECONDS) return value_/1000.0;
    else                                  return value_;
  }

 public:
  Type   type_;
  double value_;
};

#endif
