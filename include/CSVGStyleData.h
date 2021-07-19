#ifndef CSVGStyleData_H
#define CSVGStyleData_H

#include <CSVGStroke.h>
#include <CSVGFill.h>
#include <CSVGObjectMarker.h>
#include <map>

class CSVGStyleData {
 public:
  using NameValues = std::map<std::string, std::string>;
  using Color      = CSVGInheritValT<CSVGColor>;
  using Opacity    = CSVGInheritValT<double>;
  using FillType   = CSVGInheritValT<CFillType>;
  using Width      = CSVGInheritValT<double>;
  using LineCap    = CSVGInheritValT<CLineCapType>;
  using LineJoin   = CSVGInheritValT<CLineJoinType>;
  using MiterLimit = CSVGInheritValT<double>;
  using LineDash   = CSVGInheritValT<CSVGStrokeDash>;

 public:
  CSVGStyleData(CSVG &svg, const std::string &id) :
   svg_(svg), id_(id), stroke_(svg), fill_(svg) {
  }

  void setValue(const std::string &name, const std::string &value);

  const NameValues &nameValues() const { return nameValues_; }

  // stroke
  bool           getStrokeColorValid     () const { return stroke_.getColorValid(); }
  Color          getStrokeColor          () const { return stroke_.getColor(); }
  bool           getStrokeOpacityValid   () const { return stroke_.getOpacityValid(); }
  Opacity        getStrokeOpacity        () const { return stroke_.getOpacity(); }
  bool           getStrokeRuleValid      () const { return stroke_.getRuleValid(); }
  FillType       getStrokeRule           () const { return stroke_.getRule(); }
  bool           getStrokeUrlValid       () const { return stroke_.getUrlValid(); }
  std::string    getStrokeUrl            () const { return stroke_.getUrl(); }
  bool           getStrokeFillObjectValid() const { return stroke_.getFillObjectValid(); }
  CSVGObject*    getStrokeFillObject     () const { return stroke_.getFillObject(); }
  bool           getStrokeWidthValid     () const { return stroke_.getWidthValid(); }
  Width          getStrokeWidth          () const { return stroke_.getWidth(); }
  bool           getStrokeDashValid      () const { return stroke_.getDashValid(); }
  LineDash       getStrokeDash           () const { return stroke_.getDash(); }
  bool           getStrokeLineCapValid   () const { return stroke_.getLineCapValid(); }
  LineCap        getStrokeLineCap        () const { return stroke_.getLineCap(); }
  bool           getStrokeLineJoinValid  () const { return stroke_.getLineJoinValid(); }
  LineJoin       getStrokeLineJoin       () const { return stroke_.getLineJoin(); }
  bool           getStrokeMiterLimitValid() const { return stroke_.getMiterLimitValid(); }
  MiterLimit     getStrokeMiterLimit     () const { return stroke_.getMiterLimit(); }

  // fill
  bool        getFillColorValid      () const { return fill_.getColorValid(); }
  Color       getFillColor           () const { return fill_.getColor(); }
  bool        getFillOpacityValid    () const { return fill_.getOpacityValid(); }
  Opacity     getFillOpacity         () const { return fill_.getOpacity(); }
  bool        getFillRuleValid       () const { return fill_.getRuleValid(); }
  FillType    getFillRule            () const { return fill_.getRule(); }
  bool        getFillUrlValid        () const { return fill_.getUrlValid(); }
  std::string getFillUrl             () const { return fill_.getUrl(); }
  bool        getFillFillObjectValid () const { return fill_.getFillObjectValid(); }
  CSVGObject* getFillFillObject      () const { return fill_.getFillObject(); }

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
