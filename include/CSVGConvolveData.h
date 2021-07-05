#ifndef CSVGConvolveData_H
#define CSVGConvolveData_H

#include <vector>
#include <string>

struct CSVGConvolveData {
  using Reals = std::vector<double>;

  double      xorder           { 3 };
  double      yorder           { 3 };
  Reals       kernelMatrix     { };
  double      divisor          { 1 };
  double      bias             { 0 };
  double      targetX          { 0 };
  double      targetY          { 0 };
  std::string edgeMode         { 1 };
  std::string kernelUnitLength { 1 };
  bool        preserveAlpha    { 1 };
};

#endif
