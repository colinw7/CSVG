#ifndef CRGB_NAME_H
#define CRGB_NAME_H

#include <CRGBA.h>
#include <string>
#include <map>

class CRGBName {
  typedef std::map<std::string, int> ColorMap;

 private:
  static ColorMap color_map_;
  static bool     color_map_set_;

 public:
  static bool lookup(const std::string &name, double *r, double *g, double *b, double *a=NULL);
  static bool lookup(const std::string &name, float *r, float *g, float *b, float *a=NULL);

  static CRGBA toRGBA(const std::string &name) {
    double r, g, b, a;

    lookup(name, &r, &g, &b, &a);

    return CRGBA(r, g, b, a);
  }
};

#endif
