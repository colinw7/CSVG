#ifndef CSVGStyleData_H
#define CSVGStyleData_H

#include <CSVGStroke.h>
#include <CSVGFill.h>
#include <CSVGObjectMarker.h>

class CSVGStyleData {
 public:
  CSVGStyleData(CSVG &svg, const std::string &id) :
   svg_(svg), id_(id), stroke_(svg), fill_(svg) {
  }

  void setValue(const std::string &name, const std::string &value);

  // stroke
  bool      getStrokeNoColorValid() const { return stroke_.getNoColorValid(); }
  bool      getStrokeNoColor     () const { return stroke_.getNoColor(); }
  bool      getStrokeColorValid  () const { return stroke_.getColorValid(); }
  CRGBA     getStrokeColor       () const { return stroke_.getColor(); }
  bool      getStrokeOpacityValid() const { return stroke_.getOpacityValid(); }
  double    getStrokeOpacity     () const { return stroke_.getOpacity(); }
  bool      getStrokeWidthValid  () const { return stroke_.getWidthValid(); }
  double    getStrokeWidth       () const { return stroke_.getWidth(); }
  bool      getStrokeDashValid   () const { return stroke_.getDashValid(); }
  CLineDash getStrokeDash        () const { return stroke_.getDash(); }

  // fill
  bool      getFillNoColorValid() const { return fill_.getNoColorValid(); }
  bool      getFillNoColor     () const { return fill_.getNoColor(); }
  bool      getFillColorValid  () const { return fill_.getColorValid(); }
  CRGBA     getFillColor       () const { return fill_.getColor(); }
  bool      getFillOpacityValid() const { return fill_.getOpacityValid(); }
  double    getFillOpacity     () const { return fill_.getOpacity(); }

 private:
  CSVG&            svg_;
  std::string      id_;
  CSVGStroke       stroke_;
  CSVGFill         fill_;
  CSVGObjectMarker marker_;
};

#endif
