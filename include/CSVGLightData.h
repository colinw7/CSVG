#ifndef CSVGLightData_H
#define CSVGLightData_H

#include <CPoint3D.h>
#include <CRGBA.h>

struct CSVGLightData {
  CSVGObjTypeId ltype;
  CPoint3D      lpoint;
  CRGBA         lcolor;
  CPoint3D      lpointsAt;
  double        lexponent { 0 };
  double        lcone { 0 };
  double        lelevation { 0 };
  double        lazimuth { 0 };
  double        surfaceScale { 0 };
  bool          isDiffuse { false };
  double        diffuseConstant { 0 };
  bool          isSpecular { false };
  double        specConstant { 0 };
  double        specExponent { 0 };
};

#endif
