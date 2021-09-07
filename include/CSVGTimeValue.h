#ifndef CSVGTimeValue_H
#define CSVGTimeValue_H

#include <CSVGTmpl.h>

class CSVGTimeValue : public CSVGPrintBase<CSVGTimeValue> {
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

  void print(std::ostream &os) const {
    if      (type() == CSVGTimeValueType::HOURS)
      os << value() << "h";
    else if (type() == CSVGTimeValueType::MINUTES)
      os << value() << "min";
    else if (type() == CSVGTimeValueType::SECONDS)
      os << value() << "s";
    else if (type() == CSVGTimeValueType::MILLISECONDS)
      os << value() << "ms";
    else
      os << value();
  }

 public:
  CSVGTimeValueType type_;
  double            value_;
};

#endif
