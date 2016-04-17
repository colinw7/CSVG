#ifndef CSVGTimeValue_H
#define CSVGTimeValue_H

class CSVGTimeValue {
 public:
  CSVGTimeValue(double value=0) :
   type_(CSVGTimeValueType::NONE), value_(value) {
  }

  CSVGTimeValue(CSVGTimeValueType type, double value) :
   type_(type), value_(value) {
  }

  CSVGTimeValueType type() const { return type_; }

  double value() const { return value_; }

  double getSeconds() const {
    if      (type_ == CSVGTimeValueType::HOURS       ) return value_*3600.0;
    else if (type_ == CSVGTimeValueType::MINUTES     ) return value_*60.0;
    else if (type_ == CSVGTimeValueType::SECONDS     ) return value_;
    else if (type_ == CSVGTimeValueType::MILLISECONDS) return value_/1000.0;
    else                                               return value_;
  }

 public:
  CSVGTimeValueType type_;
  double            value_;
};

#endif
