#ifndef CSVGTmpl_H
#define CSVGTmpl_H

#include <iostream>

// implement != using == function
template<typename T>
class CSVGEqBase {
 public:
  friend bool operator!=(const CSVGEqBase &lhs, const CSVGEqBase &rhs) {
    return ! operator==(static_cast<const T &>(lhs), static_cast<const T &>(rhs)); }
};

// implement << using print function
template<typename T>
class CSVGPrintBase {
 public:
  friend std::ostream &operator<<(std::ostream &os, const CSVGPrintBase &base) {
    static_cast<const T &>(base).print(os);
    return os;
  }
};

#endif
