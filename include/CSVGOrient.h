#ifndef CSVGOrient_H
#define CSVGOrient_H

#include <CAngle.h>

class CSVGOrient {
 public:
  explicit CSVGOrient(const CAngle &angle) :
    angle_(angle) {
  }

  CSVGOrient() { }

  const CAngle &angle() const { return angle_; }
  void setAngle(const CAngle &v) { angle_ = v; }

  bool isAuto() const { return isAuto_; }
  void setIsAuto(bool b) { isAuto_ = b; }

 private:
  CAngle angle_;
  bool   isAuto_ = false;
};

#endif
