#ifndef CSVGStyleData_H
#define CSVGStyleData_H

#include <CSVGStroke.h>
#include <CSVGFill.h>
#include <CSVGObjectMarker.h>
#include <map>

class CSVGStyleData {
 public:
  typedef std::map<std::string, std::string> NameValues;

 public:
  CSVGStyleData(CSVG &svg, const std::string &id) :
   svg_(svg), id_(id), stroke_(svg), fill_(svg) {
  }

  void setValue(const std::string &name, const std::string &value);

  const NameValues &nameValues() const { return nameValues_; }

  // stroke
  bool           getStrokeColorValid  () const { return stroke_.getColorValid(); }
  CSVGColor      getStrokeColor       () const { return stroke_.getColor(); }
  bool           getStrokeOpacityValid() const { return stroke_.getOpacityValid(); }
  double         getStrokeOpacity     () const { return stroke_.getOpacity(); }
  bool           getStrokeWidthValid  () const { return stroke_.getWidthValid(); }
  double         getStrokeWidth       () const { return stroke_.getWidth(); }
  bool           getStrokeDashValid   () const { return stroke_.getDashValid(); }
  CSVGStrokeDash getStrokeDash        () const { return stroke_.getDash(); }

  // fill
  bool      getFillColorValid  () const { return fill_.getColorValid(); }
  CSVGColor getFillColor       () const { return fill_.getColor(); }
  bool      getFillOpacityValid() const { return fill_.getOpacityValid(); }
  double    getFillOpacity     () const { return fill_.getOpacity(); }

  // marker
  CSVGObject *getMarkerStart() const { return marker_.getStart(); }
  CSVGObject *getMarkerMid  () const { return marker_.getMid  (); }
  CSVGObject *getMarkerEnd  () const { return marker_.getEnd  (); }

 private:
  CSVG&            svg_;
  NameValues       nameValues_;
  std::string      id_;
  CSVGStroke       stroke_;
  CSVGFill         fill_;
  CSVGObjectMarker marker_;
};

#endif
